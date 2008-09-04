// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
//
// This file is part of the Olena Library.  This library is free
// software; you can redistribute it and/or modify it under the terms
// of the GNU General Public License version 2 as published by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this library; see the file COPYING.  If not, write to
// the Free Software Foundation, 51 Franklin Street, Fifth Floor,
// Boston, MA 02111-1307, USA.
//
// As a special exception, you may use this file as part of a free
// software library without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to
// produce an executable, this file does not by itself cause the
// resulting executable to be covered by the GNU General Public
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef MLN_CORE_P_IMAGE2D_HH
# define MLN_CORE_P_IMAGE2D_HH


/*! \file mln/core/p_image2d.hh
 *
 * \brief Definition of a point set class based on a image of booleans.
 */

# include <mln/core/internal/site_set_base.hh>
# include <mln/core/alias/box2d.hh>
# include <mln/core/image/image2d.hh>
# include <mln/core/image/sub_image.hh>

# include <mln/accu/bbox.hh>
# include <mln/geom/ncols.hh>
# include <mln/accu/bbox.hh>
# include <mln/level/fill.hh>

# include <mln/level/memset_.hh>

namespace mln
{


  // Fwd decls.
  template <typename P> struct p_image2d_fwd_piter_;
  template <typename P> struct p_image2d_bkd_piter_;

  template <typename P>
  class p_image2d : public internal::site_set_base_<P, p_image2d<P> >
  {
  public:

    typedef image2d<bool> image_type;

    /// Forward Site_Iterator associated type.
    typedef p_image2d_fwd_piter_<P> fwd_piter;

    /// Backward Site_Iterator associated type.
    typedef p_image2d_bkd_piter_<P> bkd_piter;

    /// Constructor.
    p_image2d(int nrows, int ncols);
    p_image2d(const box2d& b);

    /// Insert a point \p p.
    p_image2d<P>& insert(const P p);
    template <typename S>
    p_image2d<P>& insert(const Site_Set<S>& set);

    /// Remove a point \p p.
    p_image2d<P>& remove(const P p);
    template <typename S>
    p_image2d<P>& remove(const Site_Set<S>& set);

    /// Give the number of points.
    unsigned npoints() const;

    /// Test is \p p belongs to this point set.
    bool has(const P& p) const;

    /// Test if the set is empty.
    bool is_empty() const;

    /// Give the exact bounding box.
    const box_<mln_point(P)>& bbox();;

    /// Clear this set.
    void clear();

    /// Hook to the image2d
    const image2d<bool>& image() const;
    image2d<bool>& image_non_const();
  private:
    image2d<bool> points_;
    unsigned npoints_;
    accu::bbox<P> bb_;
    bool bb_need_update_;
  };


# ifndef MLN_INCLUDE_ONLY

  template <typename P>
  p_image2d<P>::p_image2d(int nrows, int ncols)
    : points_(nrows, ncols, 0),
      npoints_(0),
      bb_need_update_(false)
  {
    level::fill(points_, false);
  }

  template <typename P>
  p_image2d<P>::p_image2d(const box2d& b)
    : points_(b),
      npoints_(0),
      bb_need_update_(false)
  {
    level::fill(points_, false);
  }


  template <typename P>
  p_image2d<P>&
  p_image2d<P>::insert(const P p)
  {
    if (points_(p) == false)
    {
      bb_.take(p);
      points_(p) = true;
      npoints_++;
    }
    return *this;
  }

  template <typename P>
  template <typename S>
  p_image2d<P>&
  p_image2d<P>::insert(const Site_Set<S>& set_)
  {
    const S& set = exact(set_);
    mln_fwd_piter(S) p(set);
    for_all(p)
      if (this->points_.has(p))
	this->insert(p);
    return *this;
  }

  template <typename P>
  p_image2d<P>&
  p_image2d<P>::remove(const P p)
  {
    if (points_(p) == true)
    {
      points_(p) = false;
      npoints_--;

      if (npoints_ == 0)
      {
	bb_.init();
	bb_need_update_ = false;
      }
      else
	bb_need_update_ = true;

    }
    return *this;
  }

  template <typename P>
  template <typename S>
  p_image2d<P>&
  p_image2d<P>::remove(const Site_Set<S>& set_)
  {
    const S& set = exact(set_);
    if (this->is_empty())
      return *this;
    mln_fwd_piter(S) p(set);
    for_all(p)
      if (this->points_.has(p))
	this->remove(p);

    return *this;
  }

  template <typename P>
  unsigned
  p_image2d<P>::npoints() const
  {
    return npoints_;
  }

  template <typename P>
  bool
  p_image2d<P>::has(const P& p) const
  {
    return points_(p);
  }

  template <typename P>
  bool
  p_image2d<P>::is_empty() const
  {
    return npoints_ == 0;
  }


  template <typename P>
  inline
  const box_<mln_point(P)>&
  p_image2d<P>::bbox()
  {
    mln_precondition(npoints() != 0);
    if (bb_need_update_)
    {
      bb_.init();
      mln_fwd_piter(p_image2d<P>) p(*this);
      for_all(p)
	bb_.take(p);
      bb_need_update_ = false;
    }

    return bb_.to_result();
  }


  template <typename P>
  void
  p_image2d<P>::clear()
  {
    if (npoints_ == 0)
      return;

    unsigned bb_nrows  = geom::nrows(bb_.to_result());
    unsigned ima_nrows = geom::nrows(points_);

    if (bb_nrows * 3 < ima_nrows * 2)
    {
      unsigned bb_ncols = geom::ncols(bb_.to_result());
      mln_line_piter_(image2d<bool>) p(bb_.to_result());
      for_all(p)
      {
	level::memset_(points_, p, false, bb_ncols);
      }
    }
    else
      level::fill(inplace(points_), false);

    npoints_ = 0;
    bb_.init();
    bb_need_update_ = false;
  }

  template <typename P>
  const image2d<bool>&
  p_image2d<P>::image() const
  {
    return points_;
  }

  template <typename P>
  image2d<bool>&
  p_image2d<P>::image_non_const()
  {
    return points_;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


# include <mln/core/p_image2d_piter.hh>
# include <mln/core/p_image2d_pixter.hh>


#endif // ! MLN_CORE_P_IMAGE2D_HH

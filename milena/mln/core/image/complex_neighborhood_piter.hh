// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_CORE_IMAGE_COMPLEX_NEIGHBORHOOD_PITER_HH
# define MLN_CORE_IMAGE_COMPLEX_NEIGHBORHOOD_PITER_HH

/// \file   mln/core/image/complex_neighborhood_piter.hh
/// \brief  Definition of a site iterator on a complex neighborhood.

# include <mln/core/internal/site_relative_iterator_base.hh>

// FIXME: These might be factor-able.

/* FIXME: Do we really want to inherit from
   internal::site_relative_iterator_base?  I might duplicate things,
   since most of the implementation of this iterator is delegated to
   the underlying complex iter.  Moreover, change_target_() is
   useless, and center_at() ``hides'' an existing method in (one of)
   the super class(es) which is not sound, IMHO.  Think about
   introducing base class replacement.  */


namespace mln
{

  /*------------------------------------------.
  | complex_neighborhood_fwd_piter<I, P, N>.  |
  `------------------------------------------*/

  /// \brief Forward iterator on complex neighborhood.
  template <typename I, typename P, typename N>
  class complex_neighborhood_fwd_piter
    : public internal::site_relative_iterator_base< N,
						    complex_neighborhood_fwd_piter<I, P, N> >
  {
    typedef complex_neighborhood_fwd_piter<I, P, N> self_;
    typedef internal::site_relative_iterator_base< N, self_ > super_;

  public:
    /// The Point_Site type.
    typedef mln_psite(N) psite;

    // FIXME: Dummy typedef.
//     typedef void dpoint;
    // FIXME: Dummy typedef.
//     typedef void mesh;

  public:
    /// Construction.
    /// \{
    complex_neighborhood_fwd_piter();
    template <typename Pref>
    complex_neighborhood_fwd_piter(const Neighborhood<N>& nbh,
				   const Pref& p_ref);
    /// \}

    /// Manipulation.
    /// \{
    /// Test if the iterator is valid.
    bool is_valid_() const;
    /// Invalidate the iterator.
    void invalidate_();

    /// Start an iteration.
    void do_start_();
    /// Go to the next point.
    void do_next_();

    /// Set the reference psite.
    /* FIXME: Careful, this method overrides the (non virtual) method
       internal::site_relative_iterator_base<S, E>::center_at.  See
       FIXME above.  */
    template <typename Pref>
    void center_at(const Pref& c);
    /// Compute the current psite.
    psite compute_p_() const;
    /// \}

  private:
    /// The type of the underlying complex iterator.
    typedef typename N::complex_fwd_iter iter;
    /// The underlying complex iterator.
    iter iter_;
  };


  /// Print an mln::complex_neighborhood_fwd_piter.
  template <typename I, typename P, typename N>
  std::ostream&
  operator<<(std::ostream& ostr,
	     const complex_neighborhood_fwd_piter<I, P, N>& p);


  /*------------------------------------------.
  | complex_neighborhood_bkd_piter<I, P, N>.  |
  `------------------------------------------*/

  /// \brief Backward iterator on complex neighborhood.
  template <typename I, typename P, typename N>
  class complex_neighborhood_bkd_piter
    : public internal::site_relative_iterator_base< N,
						    complex_neighborhood_bkd_piter<I, P, N> >
  {
    typedef complex_neighborhood_bkd_piter<I, P, N> self_;
    typedef internal::site_relative_iterator_base< N, self_ > super_;

  public:
    /// The Point_Site type.
    typedef mln_psite(N) psite;

    // FIXME: Dummy typedef.
//     typedef void dpoint;
    // FIXME: Dummy typedef.
//     typedef void mesh;

  public:
    /// Construction.
    /// \{
    complex_neighborhood_bkd_piter();
    template <typename Pref>
    complex_neighborhood_bkd_piter(const Neighborhood<N>& nbh,
				   const Pref& p_ref);
    /// \}

    /// Manipulation.
    /// \{
    /// Test if the iterator is valid.
    bool is_valid_() const;
    /// Invalidate the iterator.
    void invalidate_();

    /// Start an iteration.
    void do_start_();
    /// Go to the next point.
    void do_next_();

    /// Set the reference psite.
    /* FIXME: Careful, this method overrides the (non virtual) method
       internal::site_relative_iterator_base<S, E>::center_at.  See
       FIXME above.  */
    template <typename Pref>
    void center_at(const Pref& c);
    /// Compute the current psite.
    psite compute_p_() const;
    /// \}

  private:
    /// The type of the underlying complex iterator.
    typedef typename N::complex_bkd_iter iter;
    /// The underlying complex iterator.
    iter iter_;
  };


  /// Print an mln::complex_neighborhood_bkd_piter.
  template <typename I, typename P, typename N>
  std::ostream&
  operator<<(std::ostream& ostr,
	     const complex_neighborhood_bkd_piter<I, P, N>& p);



# ifndef MLN_INCLUDE_ONLY

  /*------------------------------------------.
  | complex_neighborhood_fwd_piter<I, P, N>.  |
  `------------------------------------------*/

  template <typename I, typename P, typename N>
  inline
  complex_neighborhood_fwd_piter<I, P, N>::complex_neighborhood_fwd_piter()
  {
  }

  template <typename I, typename P, typename N>
  template <typename Pref>
  inline
  complex_neighborhood_fwd_piter<I, P, N>::complex_neighborhood_fwd_piter(const Neighborhood<N>& nbh,
									  const Pref& p_ref)
  {
    this->change_target(exact(nbh));
    center_at(p_ref);
    mln_postcondition(!this->is_valid());
  }

  template <typename I, typename P, typename N>
  inline
  bool
  complex_neighborhood_fwd_piter<I, P, N>::is_valid_() const
  {
    return iter_.is_valid();
  }

  template <typename I, typename P, typename N>
  inline
  void
  complex_neighborhood_fwd_piter<I, P, N>::invalidate_()
  {
    iter_.invalidate();
  }

  template <typename I, typename P, typename N>
  inline
  void
  complex_neighborhood_fwd_piter<I, P, N>::do_start_()
  {
    iter_.start();
  }

  template <typename I, typename P, typename N>
  inline
  void
  complex_neighborhood_fwd_piter<I, P, N>::do_next_()
  {
    iter_.next();
  }

  template <typename I, typename P, typename N>
  template <typename Pref>
  inline
  void
  complex_neighborhood_fwd_piter<I, P, N>::center_at(const Pref& c)
  {
    super_::center_at(c);
    iter_.center_at(this->center().face());
  }

  template <typename I, typename P, typename N>
  inline
  mln_psite(N)
  complex_neighborhood_fwd_piter<I, P, N>::compute_p_() const
  {
    return psite(this->center().site_set(), iter_);
  }


  template <typename I, typename P, typename N>
  inline
  std::ostream&
  operator<<(std::ostream& ostr,
	     const complex_neighborhood_fwd_piter<I, P, N>& p)
  {
    return ostr << p.unproxy_();
  }


  /*------------------------------------------.
  | complex_neighborhood_bkd_piter<I, P, N>.  |
  `------------------------------------------*/

  template <typename I, typename P, typename N>
  inline
  complex_neighborhood_bkd_piter<I, P, N>::complex_neighborhood_bkd_piter()
  {
  }

  template <typename I, typename P, typename N>
  template <typename Pref>
  inline
  complex_neighborhood_bkd_piter<I, P, N>::complex_neighborhood_bkd_piter(const Neighborhood<N>& nbh,
									  const Pref& p_ref)
  {
    this->change_target(exact(nbh));
    center_at(p_ref);
    mln_postcondition(!this->is_valid());
  }

  template <typename I, typename P, typename N>
  inline
  bool
  complex_neighborhood_bkd_piter<I, P, N>::is_valid_() const
  {
    return iter_.is_valid();
  }

  template <typename I, typename P, typename N>
  inline
  void
  complex_neighborhood_bkd_piter<I, P, N>::invalidate_()
  {
    iter_.invalidate();
  }

  template <typename I, typename P, typename N>
  inline
  void
  complex_neighborhood_bkd_piter<I, P, N>::do_start_()
  {
    iter_.start();
  }

  template <typename I, typename P, typename N>
  inline
  void
  complex_neighborhood_bkd_piter<I, P, N>::do_next_()
  {
    iter_.next();
  }

  template <typename I, typename P, typename N>
  template <typename Pref>
  inline
  void
  complex_neighborhood_bkd_piter<I, P, N>::center_at(const Pref& c)
  {
    super_::center_at(c);
    iter_.center_at(this->center().face());
  }

  template <typename I, typename P, typename N>
  inline
  mln_psite(N)
  complex_neighborhood_bkd_piter<I, P, N>::compute_p_() const
  {
    return psite(this->center().site_set(), iter_);
  }


  template <typename I, typename P, typename N>
  inline
  std::ostream&
  operator<<(std::ostream& ostr,
	     const complex_neighborhood_bkd_piter<I, P, N>& p)
  {
    return ostr << p.unproxy_();
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln

#endif // ! MLN_CORE_IMAGE_COMPLEX_NEIGHBORHOOD_PITER_HH

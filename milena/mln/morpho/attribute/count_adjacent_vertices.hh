// Copyright (C) 2007, 2008, 2009 EPITA Research and Development Laboratory
// (LRDE)
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
#ifndef MLN_MORPHO_ATTRIBUTE_COUNT_ADJACENT_VERTICES_HH_
# define MLN_MORPHO_ATTRIBUTE_COUNT_ADJACENT_VERTICES_HH_

/// \file mln/accu/count_adjacent_vertices.hh
///
/// Define an accumulator that counts the vertices adjacent to a
/// set of p_edges psites.

# include <mln/accu/internal/base.hh>
# include <mln/pw/image.hh>
# include <mln/util/pix.hh>

namespace mln
{

  // Forward declaration.
  namespace morpho {
    namespace attribute {
      template <typename I>
      struct count_adjacent_vertices;
    }
  }


  // Traits.

  namespace trait
  {

    template <typename I>
    struct accumulator_< morpho::attribute::count_adjacent_vertices<I> >
    {
      typedef accumulator::has_untake::no    has_untake;
      typedef accumulator::has_set_value::no has_set_value;
      typedef accumulator::has_stop::no      has_stop;
      typedef accumulator::when_pix::use_p   when_pix;
    };

  } // end of namespace mln::trait


  namespace morpho
  {

    namespace attribute
    {

      /// Count_Adjacent_Vertices accumulator class.
      ///
      /// The parameter \p I is the image type on which the accumulator
      /// of pixels is built.

      template <typename F, typename S>
      struct count_adjacent_vertices< pw::image<F, S> >
	: public accu::internal::base< unsigned , count_adjacent_vertices< pw::image<F, S> > >
      {
	typedef pw::image<F, S> I;
	typedef mln_psite(I)    argument;

	count_adjacent_vertices();

	/// Manipulators.
	/// \{
	void init();

	void take(const argument& px);
	void take(const count_adjacent_vertices<I>& other);

	void take_as_init(const argument& px);
	/// \}

	/// Get the value of the accumulator.
	unsigned to_result() const;

	/// Check whether this accu is able to return a result.
	bool is_valid() const;


      protected:
	/// Update the value of the counter.
	void update_ ();

	/// The value of the counter.
	unsigned count__;
	/// The set of adjacent vertices.
	std::set<unsigned> vertices_;
      };



# ifndef MLN_INCLUDE_ONLY

      template <typename F, typename S>
      inline
      count_adjacent_vertices< pw::image<F, S> >::count_adjacent_vertices()
      {
	init();
      }

      template <typename F, typename S>
      inline
      void
      count_adjacent_vertices< pw::image<F, S> >::init()
      {
	vertices_.clear();
	update_();
      }

      template <typename F, typename S>
      inline
      void
      count_adjacent_vertices< pw::image<F,S> >::take(const argument& p)
      {
	vertices_.insert(p.v1());
	vertices_.insert(p.v2());
	update_();
      }

      template <typename F, typename S>
      inline
      void
      count_adjacent_vertices< pw::image<F,S> >::take(const count_adjacent_vertices< pw::image <F,S> >& other)
      {
	vertices_.insert (other.vertices_.begin(), other.vertices_.end());
	update_();
      }

      template <typename F, typename S>
      inline
      void
      count_adjacent_vertices< pw::image<F,S> >::take_as_init(const argument& px)
      {
	vertices_.clear();
	take(px);
      }

      template <typename F, typename S>
      inline
      unsigned
      count_adjacent_vertices< pw::image<F,S> >::to_result() const
      {
	return count__;
      }

      template <typename F, typename S>
      inline
      void
      count_adjacent_vertices< pw::image<F,S> >::update_()
      {
	count__ = vertices_.size();
      }

      template <typename F, typename S>
      inline
      bool
      count_adjacent_vertices< pw::image<F,S> >::is_valid() const
      {
	return true;
      }


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::morpho::attribute

  } // end of namespace mln::morpho

} // end of namespace mln

#endif /* !MLN_MORPHO_ATTRIBUTE_COUNT_ADJACENT_VERTICES_HH_ */
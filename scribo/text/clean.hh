// Copyright (C) 2009 EPITA Research and Development Laboratory
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

#ifndef SCRIBO_TEXT_CLEAN_HH
# define SCRIBO_TEXT_CLEAN_HH

/// \file scribo/text/clean.hh
///
/// Improve the quality of a text area.

# include <mln/core/concept/weighted_window.hh>
# include <mln/morpho/skeleton_constrained.hh>
# include <mln/topo/skeleton/is_simple_point.hh>
# include <mln/topo/skeleton/crest.hh>

# include <mln/logical/not.hh>

//# include <mln/debug/filename.hh>
//# include <mln/io/pbm/save.hh>

namespace scribo
{

  namespace text
  {

    template <typename I, typename W>
    mln_concrete(I)
    clean(const Image<I>& input_, const Weighted_Window<W>& dmap_win_);


# ifndef MLN_INCLUDE_ONLY


//    static int plop = 0;

    template <typename I, typename W>
    mln_concrete(I)
    clean(const Image<I>& input_, const Weighted_Window<W>& dmap_win_)
    {
      trace::entering("scribo::text::clean");

      const I& input = exact(input_);
      const W& dmap_win = exact(dmap_win_);
      mlc_bool(mln_site_(I)::dim == 2)::check();
      mlc_equal(mln_value(I),bool)::check();
      mln_precondition(input.is_valid());
      mln_precondition(dmap_win.is_valid());

      mln_ch_value(I,unsigned)
        dmap = transform::distance_front(logical::not_(input), c8(),
                                         dmap_win,
                                         mln_max(unsigned));

      I constraint = topo::skeleton::crest(input, dmap, c8());
      mln_postcondition(constraint.is_valid());

      I skeleton =
        morpho::skeleton_constrained(input, c8(),
                                     topo::skeleton::is_simple_point<I,neighb2d>,
                                     extend(constraint, false), dmap);

      win::rectangle2d disk(3,3);
      I output = morpho::dilation(skeleton, disk);

//      if (plop < 10)
//      {
//        io::pbm::save(input, mln::debug::filename("input.pbm"));
//        io::pbm::save(skeleton, mln::debug::filename("skeleton.pbm"));
//        io::pbm::save(output, mln::debug::filename("dil_skel.pbm"));
//        ++plop;
//      }

      trace::exiting("scribo::text::clean");
      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::text

} // end of namespace scribo

#endif // ! SCRIBO_TEXT_CLEAN_HH
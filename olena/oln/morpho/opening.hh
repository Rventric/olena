// Copyright (C) 2007 EPITA Research and Development Laboratory
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

#ifndef	OLN_MORPHO_OPENING_HH
# define OLN_MORPHO_OPENING_HH

#include <oln/morpho/elementary_erosion.hh>
#include <oln/morpho/elementary_dilation.hh>

namespace oln
{

  namespace morpho
  {

    // Fwd decl.

    template <typename I, typename W>
    oln_plain(I)
    opening(const Image<I>& input, const Window<W>& win);


# ifndef OLN_INCLUDE_ONLY

    namespace impl
    {

      // Generic version.

      template <typename I, typename W>
      oln_plain(I)
      opening_(const Image<I>&  input,
	       const Window<W>& win)
      {
	oln_plain(I) = elementary_dilation(input, win);
	return elementary_erosion(tmp, win); // FIXME : inverse(win).
      }

      // FIXME: Add a fast version.

    } // end of namespace oln::morpho::impl


    // Facade.

    template <typename I, typename W>
    oln_plain(I)
    opening(const Image<I>& input, const Window<W>& win)
    {
      return impl::opening_(exact(input), exact(win));
    }

# endif // ! OLN_INCLUDE_ONLY

  } // end of namespace oln::morpho

} // end of namespace oln


#endif // ! OLN_MORPHO_OPENING_HH

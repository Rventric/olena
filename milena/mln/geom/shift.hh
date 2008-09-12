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

#ifndef MLN_GEOM_SHIFT_HH
# define MLN_GEOM_SHIFT_HH

/*! \file mln/geom/shift.hh
 *
 * \brief Define a function which shifts a window with a delta-point.
 */

# include <mln/core/window.hh>
# include <mln/convert/to.hh>



namespace mln
{

  namespace geom
  {

    /// Shift a window \p win with a delta-point \p dp.
    template <typename W>
    window<mln_dpoint(W)>
    shift(const Window<W>& win, const mln_dpoint(W)& dp);


# ifndef MLN_INCLUDE_ONLY

    template <typename W>
    inline
    window<mln_dpoint(W)>
    shift(const Window<W>& win, const mln_dpoint(W)& dp)
    {
      typedef mln_point(W) P;
      window<mln_dpoint(W)> tmp;
      mln_qiter(W) q(win, P::origin);
      for_all(q)
	tmp.insert(convert::to(q) + dp);
      return tmp;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::geom

} // end of namespace mln


#endif // ! MLN_GEOM_SHIFT_HH

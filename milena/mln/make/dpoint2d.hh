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

#ifndef MLN_MAKE_DPOINT2D_HH
# define MLN_MAKE_DPOINT2D_HH

/*! \file mln/make/dpoint2d.hh
 *
 * \brief Routine to construct an mln::dpoint2d.
 */

# include <mln/core/alias/dpoint2d.hh>


namespace mln
{

  namespace make
  {

    /*! \brief Create an mln::dpoint2d.
     *
     * \param[in] row Row coordinate.
     * \param[in] col Column coordinate.
     *
     * \return A 2D dpoint.
     */
    mln::dpoint2d dpoint2d(def::coord row, def::coord col);


# ifndef MLN_INCLUDE_ONLY

    inline
    mln::dpoint2d dpoint2d(def::coord row, def::coord col)
    {
      mln::dpoint2d tmp;
      tmp[0] = row;
      tmp[1] = col;
      return tmp;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::make

} // end of namespace mln


#endif // ! MLN_MAKE_DPOINT2D_HH

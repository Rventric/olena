// Copyright (C) 2001, 2002, 2003, 2004  EPITA Research and Development Laboratory
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
// the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
// Boston, MA 02110-1301, USA.
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

#ifndef OLENA_MORPHO_MINMAX_HH
# define OLENA_MORPHO_MINMAX_HH

# include <oln/arith/ops.hh>
# include <oln/arith/logic.hh>

namespace oln {
  namespace morpho {

    /*!
    **  Before olena v0.6, bin type was represented by "true" and
    **  "false", so it did not make sense to implement a min and max for
    **  it. However, morphological operatators defined on binary image
    **  usually needs min and max operators, this is why it was previouly
    **  implemented here.\n
    **
    **  But since v0.6 and higher, bin type is represented by '0' and
    **  '1', so it has min and max operators. Thus arith::min and
    **  arith::max should be used instead of obsolete these morpho::min
    **  and morpho::max.\n
    **
    **  These operators are kept here for compatibility with older
    **  versions.
    */

    using arith::min;
    using arith::max;

  } // morpho
} // oln

#endif /* OLENA_MORPHO_MINMAX_HH */

// Copyright (C) 2005 EPITA Research and Development Laboratory
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
// the Free Software Foundation, 59 Temple Place - Suite 330, Boston,
// MA 02111-1307, USA.
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

#ifndef OLENA_CORE_1D_GRID1D_HH
# define OLENA_CORE_1D_GRID1D_HH

# include <mlc/value.hh>
# include <oln/core/abstract/grid.hh>


namespace oln {

  // fwd decls
  struct grid1d;
  struct point1d;
  struct dpoint1d;
  struct size1d;
  struct coord_t;

  // super type
  template <>
  struct set_super_type < grid1d > { typedef abstract::grid< grid1d > ret; };

  // props
  template <>
  struct set_props < category::grid, grid1d >
  {
    typedef point1d  point_type;
    typedef dpoint1d dpoint_type;
    typedef coord_t  coord_type;
    typedef size1d   size_type;
    typedef mlc::value<unsigned,1> dimvalue_type;
  };



  struct grid1d : public abstract::grid< grid1d >
  {
  protected:
    grid1d()
    {}
  };

} // end of namespace oln



#endif // ! OLENA_CORE_1D_GRID1D_HH

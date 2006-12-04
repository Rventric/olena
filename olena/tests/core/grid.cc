// Copyright (C) 2006 EPITA Research and Development Laboratory
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

/// Test grids.

#include <cassert>
#include <oln/core/1d/aliases.hh>
#include <oln/core/2d/aliases.hh>
#include <oln/core/3d/aliases.hh>
#include <oln/core/gen/grid.hh>

int
main()
{
  // The extra pairs of parenthesis around mlc_value are needed to
  // prevent the assert macro from interpreting the arguments of
  // mlc_value as its own.
  assert((mlc_value(oln_vtype_(oln::grid1d, dimvalue))) == 1);
  assert((mlc_value(oln_vtype_(oln::grid2d, dimvalue))) == 2);
  assert((mlc_value(oln_vtype_(oln::grid3d, dimvalue))) == 3);
}

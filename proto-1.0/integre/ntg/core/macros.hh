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

#ifndef INTEGRE_CORE_MACROS_HH
# define INTEGRE_CORE_MACROS_HH

# define ntg_max_val(T) ntg::props<ntg_category_type(T),T>::max_val

# define ntg_depth(T) ntg::props<ntg_category_type(T),T>::depth

# define ntg_comp_type(T) typename ntg::props<ntg_category_type(T),T>::comp_type

# define ntg_io_type(T) typename ntg::props<ntg_category_type(T),T>::io_type

#endif // ! INTEGRE_CORE_MACROS_HH

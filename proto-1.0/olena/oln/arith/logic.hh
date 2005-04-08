// Copyright (C) 2005  EPITA Research and Development Laboratory
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

#ifndef OLENA_LOGIC_OPS_HH
# define OLENA_LOGIC_OPS_HH

# include <oln/core/abstract/image.hh>
# include <oln/core/abstract/image_typeness.hh>
# include <oln/core/pw/all.hh>


/// Operator 'and' between 2 binary images.

template <typename L, typename R>
oln::image_from_pw< oln::pw::cmp< oln::pw::image<L>,
				  oln::pw::image<R>, oln::pw::internal::and_ > >
operator && (const oln::abstract::binary_image<L>& lhs,
	     const oln::abstract::binary_image<R>& rhs)
{
  return oln::for_all_p(oln::p_value(lhs) && oln::p_value(rhs));
}


/// Operator 'or' between 2 binary images.

template <typename L, typename R>
oln::image_from_pw< oln::pw::cmp< oln::pw::image<L>,
				  oln::pw::image<R>, oln::pw::internal::or_ > >
operator || (const oln::abstract::binary_image<L>& lhs,
	     const oln::abstract::binary_image<R>& rhs)
{
  return oln::for_all_p(oln::p_value(lhs) || oln::p_value(rhs));
}


/// Unary operator 'not' on a binary image.

template <typename I>
oln::image_from_pw< oln::pw::not_< oln::pw::image<I> > >
operator ! (const oln::abstract::binary_image<I>& rhs)
{
  return oln::for_all_p(!oln::p_value(rhs));
}



#endif // ! OLENA_LOGIC_OPS_HH

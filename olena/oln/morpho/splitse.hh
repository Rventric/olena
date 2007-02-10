// Copyright (C) 2001, 2002, 2004  EPITA Research and Development Laboratory
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

#ifndef OLENA_MORPHO_SPLITSE_HH
# define OLENA_MORPHO_SPLITSE_HH

# include <oln/basics.hh>

namespace oln {
  namespace morpho {

    /*!
    ** \brief Get a sub part of a structuring element.
    **
    ** \param E Exact type of the structuring element.
    **
    ** \arg se The structuring element.
    **
    ** A point p take part of the new structuring element if it exists
    ** a i  that belongs to [[0..dim-1]] like  p(i) < 0 and  for all j
    ** that belongs to [[0..i-1]] p(j) = 0.
    **
    */
    template<class E>
    mlc_exact_type(E)
      get_plus_se_only(const abstract::struct_elt<E>& se)
    {
      oln_iter_type(E) dp(se);
      E out;

      for_all (dp)
	{
	  unsigned n;
	  for (n = 0; n < E::dim; ++n)
	    if (dp.cur().nth(n) < 0) {
	      out.add(dp);
	      break;
	    } else if (dp.cur().nth(n) > 0) {
	      break;
	    }
	}
      return out;
    }

    /*!
    ** \brief Get a sub part of a structuring element.
    **
    ** \param E Exact type of the structuring element.
    **
    ** \arg se The structuring element.
    **
    ** A point p take part of the new structuring element if it exists
    ** a i  that belongs to [[0..dim-1]] like  p(i) < 0 and  for all j
    ** that  belongs to  [[0..i-1]] p(j)  =  0 or  if for  all i  that
    ** belongs to [[0..dim-1]] p(i) = 0.
    **
    */
    template<class E>
    mlc_exact_type(E)
      get_plus_se_p(const abstract::struct_elt<E>& se)
    {
      oln_iter_type(E) dp(se);
      E out;

      for_all (dp)
	{
	  unsigned n;
	  for (n = 0; n < E::dim; ++n)
	    if (dp.cur().nth(n) < 0) {
	      out.add(dp);
	      break;
	    } else if (dp.cur().nth(n) > 0) {
	      break;
	    }
	  // All p.nth(n) are 0.
	  if (n == E::dim)
	    out.add(dp);
	}
      return out;
    }

    /*!
    ** \brief Get a sub part of a structuring element.
    **
    ** \param E Exact type of the structuring element.
    **
    ** \arg se The structuring element.
    **
    ** A point p take part of the new structuring element if it exists
    ** a i  that belongs to [[0..dim-1]] like  p(i) > 0 and  for all j
    ** that  belongs to  [[0..i-1]] p(j)  =  0.
    **
    */
    template<class E>
    mlc_exact_type(E)
      get_minus_se_only(const abstract::struct_elt<E>& se)
    {
      oln_iter_type(E) dp(se);
      E out;

      for_all (dp)
	{
	  unsigned n;
	  for (n = 0; n < E::dim; ++n)
	    if (dp.cur().nth(n) > 0) {
	      out.add(dp);
	      break;
	    } else if (dp.cur().nth(n) < 0) {
	      break;
	    }
	}
      return out;
    }

    /*!
    ** \brief Get a sub part of a structuring element.
    **
    ** \param E Exact type of the structuring element.
    **
    ** \arg se The structuring element.
    **
    ** A point p take part of the new structuring element if it exists
    ** a i  that belongs to [[0..dim-1]] like  p(i) > 0 and  for all j
    ** that  belongs to  [[0..i-1]] p(j)  =  0 or  if for  all i  that
    ** belongs to [[0..dim-1]] p(i) = 0.
    **
    */
    template<class E>
    mlc_exact_type(E)
      get_minus_se_p(const abstract::struct_elt<E>& se)
    {
      oln_iter_type(E) dp(se);
      E out;

      for_all (dp)
	{
	  unsigned n;
	  for (n = 0; n < E::dim; ++n)
	    if (dp.cur().nth(n) > 0) {
	      out.add(dp);
	      break;
	    } else if (dp.cur().nth(n) < 0) {
	      break;
	    }
	  // All p.nth(n) are 0.
	  if (n == E::dim)
	    out.add(dp);
	}
      return out;
    }

  } // morpho
} // oln

#endif // OLENA_MORPHO_SPLITSE_HH

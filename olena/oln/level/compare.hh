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

#ifndef OLN_LEVEL_COMPARE_HH
# define OLN_LEVEL_COMPARE_HH

# include <oln/core/concept/image.hh>
# include <oln/core/gen/pset_compare.hh>
# include <oln/core/gen/literal.hh>

// FIXME: Rename this file as "comparison.hh".



namespace oln
{

  // Fwd decls.

  template <typename L, typename R>
  bool operator == (const Image<L>& lhs, const Image<R>& rhs);

  template <typename I>
  bool operator == (const Image<I>& lhs, const literal_<oln_value(I)>& val);
  
  template <typename L, typename R>
  bool operator < (const Image<L>& lhs, const Image<R>& rhs);

  template <typename I>
  bool operator < (const Image<I>& lhs, const literal_<oln_value(I)>& val);
  
  template <typename L, typename R>
  bool operator <= (const Image<L>& lhs, const Image<R>& rhs);

  template <typename I>
  bool operator <= (const Image<I>& lhs, const literal_<oln_value(I)>& val);



# ifndef OLN_LNCLUDE_ONLY

  namespace impl
  {

    //  Image L  ==  Image R
    // ----------------------

    // Generic version.

    template <typename L, typename R>
    bool op_eq_(const Image<L>& lhs, const Image<R>& rhs)
    {
      precondition(lhs.points() == rhs.points());
      oln_piter(L) p(lhs.points());
      for_all(p)
	if (lhs(p) != rhs(p))
	  return false;
      return true;
    }


    //  Image I  ==  Value V
    // ----------------------

    // Generic version.

    template <typename I>
    bool op_eq_(const Image<I>& lhs, const literal_<oln_value(I)>& val)
    {
      oln_piter(I) p(lhs.points());
      for_all(p)
	if (lhs(p) != val)
	  return false;
      return true;
    }


    //  Image L  <  Image R
    // ---------------------

    // Generic version.

    template <typename L, typename R>
    bool op_less_(const Image<L>& lhs, const Image<R>& rhs)
    {
      precondition(lhs.points() == rhs.points());
      oln_piter(L) p(lhs.points());
      for_all(p)
	if (lhs(p) >= rhs(p))
	  return false;
      return true;
    }


    //  Image I  <  Value V
    // ---------------------

    // Generic version.

    template <typename I>
    bool op_less_(const Image<I>& lhs, const literal_<oln_value(I)>& val)
    {
      oln_piter(I) p(lhs.points());
      for_all(p)
	if (lhs(p) >= val)
	  return false;
      return true;
    }


    //  Image L  <=  Image R
    // ----------------------

    // Generic version.

    template <typename L, typename R>
    bool op_leq_(const Image<L>& lhs, const Image<R>& rhs)
    {
      precondition(lhs.points() == rhs.points());
      oln_piter(L) p(lhs.points());
      for_all(p)
	if (lhs(p) > rhs(p))
	  return false;
      return true;
    }


    //  Image I  <=  Value V
    // ----------------------

    // Generic version.

    template <typename I>
    bool op_leq_(const Image<I>& lhs, const literal_<oln_value(I)>& val)
    {
      oln_piter(I) p(lhs.points());
      for_all(p)
	if (lhs(p) > val)
	  return false;
      return true;
    }


  } // end of namespace oln::level::impl


  // Facades.

  template <typename L, typename R>
  bool operator == (const Image<L>& lhs, const Image<R>& rhs)
  {
    assert_same_grid_<L, R>::check();
    return impl::op_eq_(exact(lhs), exact(rhs));
  }

  template <typename I>
  bool operator == (const Image<I>& lhs, const literal_<oln_value(I)>& val)
  {
    return impl::op_eq_(exact(lhs), val);
  }

  template <typename L, typename R>
  bool operator < (const Image<L>& lhs, const Image<R>& rhs)
  {
    assert_same_grid_<L, R>::check();
    return impl::op_less_(exact(lhs), exact(rhs));
  }

  template <typename I>
  bool operator < (const Image<I>& lhs, const literal_<oln_value(I)>& val)
  {
    return impl::op_less_(exact(lhs), val);
  }

  template <typename L, typename R>
  bool operator <= (const Image<L>& lhs, const Image<R>& rhs)
  {
    assert_same_grid_<L, R>::check();
    return impl::op_leq_(exact(lhs), exact(rhs));
  }

  template <typename I>
  bool operator <= (const Image<I>& lhs, const literal_<oln_value(I)>& val)
  {
    return impl::op_leq_(exact(lhs), val);
  }

# endif // ! OLN_INCLUDE_ONLY

} // end of namespace oln


#endif // ! OLN_LEVEL_COMPARE_HH

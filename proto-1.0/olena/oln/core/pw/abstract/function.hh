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

#ifndef OLENA_CORE_PW_ABSTRACT_FUNCTION_HH
# define OLENA_CORE_PW_ABSTRACT_FUNCTION_HH

# include <mlc/any.hh>
# include <oln/core/abstract/image.hh>
# include <oln/core/typedefs.hh>


# define oln_pw_type_of(PointWiseType, Alias) \
mlc_type_of(oln, oln::category::pw, PointWiseType, Alias)


namespace oln {


  namespace category
  {
    struct pw; // means "point-wise"
  }


  template <>
  struct set_default_props < category::pw >
  {
    typedef mlc::undefined_type point_type;
    typedef mlc::undefined_type value_type;
    typedef mlc::undefined_type size_type;
  };


  template <typename PWF>
  struct get_props < category::pw, PWF >
  {
    typedef oln_pw_type_of(PWF, point) point_type;
    typedef oln_pw_type_of(PWF, value) value_type;
    typedef oln_pw_type_of(PWF, size)  size_type;

    static void echo(std::ostream& ostr)
    {
      ostr << "props_of( oln::category::pw, " << mlc_to_string(PWF) << " ) =" << std::endl
	   << "{" << std::endl
	   << "\t point_type = " << mlc_to_string(point_type) << std::endl
	   << "\t value_type = " << mlc_to_string(value_type) << std::endl
	   << "\t size_type  = " << mlc_to_string(size_type)  << std::endl
	   << "}" << std::endl;
    }
  };


  namespace pw {

    // fwd decls
    namespace abstract {
      template <typename E> struct function;
    }
    template <typename L, typename R> struct minus;
    template <typename T> struct literal;
    template <typename F> struct not_;


    namespace abstract {

      template <typename E>
      struct function : public mlc::any<E>
      {
	typedef oln_pw_type_of(E, point) point_type;
	typedef oln_pw_type_of(E, value) value_type;
	typedef oln_pw_type_of(E, size)  size_type;

	const size_type& size() const
	{
	  return this->exact().impl_size();
	}

	const value_type operator()(const point_type& p) const
	{
	  return this->exact().impl_get(p);
	}

	bool hold(const point_type& p) const
	{
	  return this->exact().impl_hold(p);
	}

	bool hold_large(const point_type& p) const
	{
	  return this->exact().impl_hold_large(p);
	}

	minus<literal<value_type>, E> operator-() const;
	not_<E> operator!() const;

      protected:
	function() {}

	~function()
	{
	  { // impl_size
	    typedef const size_type& (E::*meth)() const;
	    meth adr = &E::impl_size;
	    adr = 0;
	  }
	  { // impl_get
	    typedef const value_type (E::*meth)(const point_type&) const;
	    meth adr = &E::impl_get;
	    adr = 0;
	  }
	  { // impl_hold
	    typedef bool (E::*meth)(const point_type&) const;
	    meth adr = &E::impl_hold;
	    adr = 0;
	  }
	  { // impl_hold_large
	    typedef bool (E::*meth)(const point_type&) const;
	    meth adr = &E::impl_hold_large;
	    adr = 0;
	  }
	}

      };


    } // end of namespace oln::pw::abstract

  } // end of namespace oln::pw

} // end of namespace oln



#endif // ! OLENA_CORE_PW_ABSTRACT_FUNCTION_HH

// Copyright (C) 2001, 2002, 2003  EPITA Research and Development Laboratory
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

#ifndef OLENA_CONVERT_CONVERSION_HH
# define OLENA_CONVERT_CONVERSION_HH

# include <mlc/type.hh>
# include <oln/core/image.hh>
# include <oln/core/compose.hh>
# include <ntg/utils/debug.hh>
# include <oln/convert/abstract/conversion.hh> 
# include <functional>

namespace oln {
  namespace convert {

    /* ConvOutput queries the output type of conversion ConvType for
       an input of type InputType.  This comes handy when computing
       the return type of a function which takes a conversion function
       in argument.  */
    template<class ConvType, class Base, class InputType>
    struct convoutput
    {
      typedef typename abstract::conversion<ConvType, Base>::template output<InputType>::ret ret;
    };

    namespace internal {
      /* Compose a conversion C and an adaptable unary function UF,
	 producing an adaptable unary function.  */
      template <class C, class UF>
      struct compconv1_ :
	public std::unary_function <typename UF::argument_type,
	  typename C::template output<typename UF::argument_type>::ret>
      {
	typedef compconv1_ self;

	typename self::result_type
	operator()(typename self::argument_type arg) const {
	  return _conv(_func(arg));
	}

	compconv1_(const C& conv, const UF& func)
	  : _conv(conv), _func(func)
	{}
      private:
	const C _conv;
	const UF _func;
      };
      
      /* Compose a conversion C and an adaptable binary function BF,
	 producing an adaptable binary function.  */
      template <class C, class BF>
      struct compconv2_ :
	public std::binary_function <typename BF::first_argument_type,
          typename BF::second_argument_type,
          typename C::template output<typename BF::result_type>::ret>
      {
	typedef compconv2_ self;

	typename self::result_type
	operator()(typename self::first_argument_type arg1,
		   typename self::second_argument_type arg2) const {
	  return _conv(_func(arg1, arg2));
	}

	compconv2_(const C &conv, const BF &func)
	  : _conv(conv), _func(func)
	{}
      private:
	const C _conv;
	const BF _func;
      };

    } // end of internal

    /* Friendly procedure that build an internal::compconv1_ with
       type deduction.  */
    template <class C, class B, class UF>
    internal::compconv1_<C, UF>
    compconv1(const abstract::conversion<C, B>& conv, const UF &func) {
      return internal::compconv1_<C, UF>(conv.exact(), func);
    }

    /* Likewise for compconv2_.  */
    template <class C, class B, class BF>
    internal::compconv2_<C, BF>
    compconv2(const abstract::conversion<C, B>& conv, const BF &func) {
      return internal::compconv2_<C, BF>(conv.exact(), func);
    }


    /* The core oln::apply function, cannot apply all conversion function,
       because they do not all define result_type.  So we define another
       apply function here, to apply conversions.  */
    template<class C, class B, class I> inline
    typename mute<I, typename convoutput<C, B, Value(I)>::ret>::ret
    apply(const abstract::conversion<C, B>& conv, const ::oln::abstract::image<I>& input)
    {
      /* CONV can now be wrapped as an Adaptable Unary Function
	 because we know the input type.  Composing CONV with the
	 identity for the input type will cause such wrapping to
	 happen.  */
      return apply(compconv1(conv, f_identity<Value(I)>()), input);
    }

  } // convert

  /* Export conversion and convouput into oln:: to simplify the
     writing of processings.  */
  using convert::convoutput;

} // oln


#endif // OLENA_CONVERT_CONVERSION_HH

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

#ifndef METALIC_MATH_HH
# define METALIC_MATH_HH

# include <mlc/bool.hh>

namespace mlc
{

  namespace internal
  {

    // Helper structs for pow2sup below.

    template<unsigned N> struct is_pow2 { typedef false_type ret; };
    template<> struct is_pow2<8>  { typedef true_type ret; };
    template<> struct is_pow2<16> { typedef true_type ret; };
    template<> struct is_pow2<32> { typedef true_type ret; };
    template<> struct is_pow2<64> { typedef true_type ret; };

    template<unsigned N, class> struct find_pow2sup;
    template<unsigned N> struct find_pow2sup<N,true_type> {
      enum { value = N };
    };
    template<unsigned N> struct find_pow2sup<N,false_type> {
      enum { value = find_pow2sup< N+1,
	     typename is_pow2<N+1>::ret >::value };
    };

  } // end of internal

  // Smaller power of 2 greater than N.

  template<unsigned N>
  struct pow2sup {
    enum {
      value =
      internal::find_pow2sup< N,
      typename internal::is_pow2<N>::ret >::value
    };
    ~pow2sup() { mlc::value<bool, (N < 32)>::ensure(); }
  };

  // Various tests on N (actually, we tests only oddness.)

  template<unsigned N>
  struct utest {
    static void ensure_odd()   { mlc::value<bool, (N/2 == (N+1)/2)>::ensure(); }
  };

} // end of namespace mlc

#endif  // METALIC_MATH_HH

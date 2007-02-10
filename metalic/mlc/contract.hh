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

#ifndef METALIC_CONTRACT_HH
# define METALIC_CONTRACT_HH

# include <mlc/config/system.hh>

# ifdef NDEBUG

#  define assertion(expr)         ((void) 0)
#  define invariant(expr)         ((void) 0)
#  define precondition(expr)      ((void) 0)
#  define postcondition(expr)     ((void) 0)

# else // NDEBUG

#  include <iostream>
#  include <cstdlib>
#  include <sstream>
#  include <stdexcept>
#  include <cassert>

namespace mlc 
{

#  ifdef OLN_EXCEPTIONS

  // FIXME: this fails if several sources files are compiled.
  // The code on this function should go in a .so file.

  // FIXME: this fails if several sources files are compiled.

  inline void FailedCondition__( const char* condType,
				 const char* condText,
				 const char* fileName 
#   ifndef RUNNING_TESTS
				 ,int fileLine
#   endif
				 )
  {
    // put a breakpoint _here_ at debug-time
    std::ostringstream err;

    err << fileName << ':'
#   ifndef RUNNING_TESTS
	<< fileLine
#   endif
	<< ": "
            << condType << " `"
	<< condText << "' failed.";
#   ifndef RUNNING_TESTS
    throw std::runtime_error(err.str());
#   else
    std::cerr << err.str();
    exit(1);
#   endif
  }

#  else // no OLN_EXCEPTIONS

  inline void FailedCondition__( const char* condType,
				 const char* condText,
				 const char* fileName
#   ifndef RUNNING_TESTS
				 ,int fileLine
#   endif
				 )
{
  // put a breakpoint _here_ at debug-time
  std::cerr << fileName << ':'
#   ifndef RUNNING_TESTS
            << fileLine
#   endif
	    << ": "
            << condType << " `"
            << condText << "' failed." << std::endl;
#   ifndef RUNNING_TESTS
  abort();
#   else
  exit(1);
#   endif
}

#  endif // OLN_EXCEPTIONS

} // end of mlc

#  ifndef RUNNING_TESTS
#   define TestCondition__(condType,expr) \
  ((void) ((expr) ? 0 : (::mlc::FailedCondition__( #condType, #expr, \
                                                   __FILE__, __LINE__ ), 0)))
#  else
#   define TestCondition__(condType,expr) \
  ((void) ((expr) ? 0 : (::mlc::FailedCondition__( #condType, #expr, \
                                                   __FILE__ ), 0)))
#  endif

// FIXME: assertions should use extern functions defined in a
// separated object file. This saves a LOT of compilation
// time. assert() is used temporarily to fix this problem.

#  ifdef OLN_EXCEPTIONS
#   define assertion(expr)         TestCondition__(Assertion,expr)
#   define invariant(expr)         TestCondition__(Invariant,expr)
#   define precondition(expr)      TestCondition__(Precondition,expr)
#   define postcondition(expr)     TestCondition__(Postcondition,expr)
#  else
#   define assertion(expr)         assert(expr)
#   define invariant(expr)         assert(expr)
#   define precondition(expr)      assert(expr)
#   define postcondition(expr)     assert(expr)
#  endif

# endif // NDEBUG

#endif // ! METALIC_CONTRACT_HH

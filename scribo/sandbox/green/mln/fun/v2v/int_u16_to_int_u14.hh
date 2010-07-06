// Copyright (C) 2007 EPITA Research and Development Laboratory (LRDE)
// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#ifndef MLN_FUN_V2V_INT_U16_TO_INT_U14_HH
#define MLN_FUN_V2V_INT_U16_TO_INT_U14_HH

#include <mln/value/int_u16.hh>
#include <mln/value/int_u.hh>

/// \file
///
/// \brief Convert int_u16 value to int_u14.

namespace mln
{

  namespace fun
  {
    
    namespace v2v
    {

      /// \brief Convert int_u16 value to int_u14.
      ///
      /// \ingroup modfunv2v

      struct int_u16_to_int_u14 : Function_v2v< int_u16_to_int_u14 >
      {
	typedef value::int_u16   argument;
	typedef value::int_u<14> result;
	
	result operator()(const argument& c) const
	{
	  result res(c / 4);
	  
	  return res;
	}
      };

    }
    
  }

}


#endif // ! MLN_FUN_V2V_INT_U16_TO_INT_U14_HH
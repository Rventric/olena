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

#ifndef OLENA_TOPO_COMBINATORIAL_MAP_INTERNAL_BETA_HH
# define OLENA_TOPO_COMBINATORIAL_MAP_INTERNAL_BETA_HH

# include <oln/topo/combinatorial-map/internal/anyfunc.hh>

namespace oln {

  namespace topo {

    namespace combinatorial_map {

      namespace internal {

	template<class U, class V = U>
	class beta : public anyfunc<U, V, beta<U, V> >
	{
	public:
	  static std::string
	  name() 
	  { 
	    return "beta";
	  }

	  beta(unsigned n) : anyfunc< U, V, beta<U, V> >(n) {}

	  void 
	  assign_(const U & i, const V & e)
	  {
	    f_[i] = e;
	  }

	  void
	  erase_(const U & i)
	  {
	    f_[i] = 0;
	  }
	};

      } // end of namespace internal
      
    } // end of namespace combinatorial_map
    
  } // end of namespace topo
  
} // end of namespace oln

#endif // ! OLENA_TOPO_COMBINATORIAL_MAP_INTERNAL_BETA_HH

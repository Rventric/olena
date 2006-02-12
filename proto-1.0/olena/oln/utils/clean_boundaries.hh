// Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006  EPITA Research and Development Laboratory
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

#ifndef OLN_UTILS_CLEAN_BOUNDARIES_HH
# define OLN_UTILS_CLEAN_BOUNDARIES_HH

# include <oln/core/abstract/image.hh>
# include <oln/core/1d/image1d.hh>
# include <oln/core/2d/image2d.hh>

namespace oln {

  namespace utils {

    namespace impl {

      template <typename T>
      void clean_boundaries_(image1d<T>& ima)
      {
	ima(-1) = ima(0);
	ima(ima.size().nindices()) = ima(ima.size().nindices() - 1);
      }

      template <typename T>
      void clean_boundaries_(image2d<T>& ima)
      {
	for (int j = 0; j < ima.size().ncols(); j++)
	  {
	    ima(-1, j) = ima(0, j);
	    ima(ima.size().nrows(), j) = ima(ima.size().nrows() - 1, j);
	  }
	for (int j = 0; j < ima.size().nrows(); j++)
	  {
	    ima(j, -1) = ima(j, 0);
	    ima(j, ima.size().ncols()) = ima(j, ima.size().ncols() - 1);
	  }
      }


    } // end of namespace oln::utils::impl


    // set outsides boundaries of the image at the boundary value.
    template <typename I>
    void clean_boundaries(abstract::image<I>& input)
    {
      if (input.size().border() != 0)
	impl::clean_boundaries_(input.exact());
    }

  } // end of namespace oln::utils

} // end of namespace oln

#endif // ! OLN_UTILS_CLEAN_BOUNDARIES_HH

// Copyright (C) 2001, 2002, 2004, 2005, 2006 EPITA Research and Development Laboratory
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

#ifndef OLENA_MORPHO_GRADIENT_HH
# define OLENA_MORPHO_GRADIENT_HH

# include <oln/morpho/dilation.hh>
# include <oln/morpho/erosion.hh>


namespace oln {

  namespace morpho {

    template<typename T>
    image2d<T> gradient_morpho(const image2d<T>& input)
    {
      image2d<T> dil = dilation(input, win_c4p());
      image2d<T> ero = erosion(input, win_c4p());
      image2d<T> res(input.size());
      oln_iter_type(image2d<T>) p(input);
      for_all(p)
	{
	  res[p] = dil[p] - ero[p];
	}
      return res;
    }

  } // end of namespace oln::morpho

} // end of namespace oln

#endif // OLENA_MORPHO_GRADIENT_HH

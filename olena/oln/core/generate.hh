// Copyright (C) 2002, 2003  EPITA Research and Development Laboratory
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

#ifndef OLENA_CORE_GENERATE_HH
# define OLENA_CORE_GENERATE_HH

# include <mlc/is_a.hh>

# include <mlc/contract.hh>
# include <oln/core/abstract/image.hh>
# include <oln/core/abstract/image_size.hh>
# include <oln/core/abstract/iter.hh>
# include <oln/core/macros.hh>

namespace oln {

  // FIXME: uncomment when ready
#if 0

  // generate (generator)

  template<class AdaptableGen, class I> 
  inline typename image_for_dim<mlc::exact<I>::ret::dim>::with_type<typename AdaptableGen::result_type>::ret
  generate(AdaptableGen f, const image_size<I>& size)
  {
    typename image_for_dim<mlc::exact<I>::ret::dim>::with_type<typename AdaptableGen::result_type>::ret output(size.exact());
    oln_iter_type(typename image_for_dim<mlc::exact<I>::ret::dim>::with_type<typename AdaptableGen::result_type>::ret) p(output);
    for_all(p) output[p] = f();
    return output;
  }

  template<class AdaptableGen, class I> 
  inline abstract::image<I>
  generate(AdaptableGen f, abstract::image<I>& input)
  {
    oln_iter_type(I) p(input);
    for_all(p) input[p] = f();
    return input;
  }
#endif

} // end of oln

#endif // OLENA_CORE_GENERATE_HH

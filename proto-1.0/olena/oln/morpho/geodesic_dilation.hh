// Copyright (C) 2001-2005  EPITA Research and Development Laboratory
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

#ifndef OLENA_MORPHO_GEODESIC_DILATION_HH
# define OLENA_MORPHO_GEODESIC_DILATION_HH

# include <mlc/is_a.hh>

# include <oln/basics.hh>
# include <oln/utils/record.hh>
# include <oln/ops/cmp.hh>
# include <oln/level/arith.hh>
# include <oln/level/logic.hh>
# include <oln/morpho/elementary_dilation.hh>


// Soille 2nd ed. (6.1.1, p.183)


namespace oln {

  namespace morpho {


    // Fwd decl of the facade for geodesic dilation of size n (default is n = 1).

    template <typename I, typename II>
    oln_type_of(I, concrete)
    geodesic_dilation(const abstract::image<I>&  marker,
		      const abstract::image<II>& mask,
		      unsigned n);



    namespace impl {


      // On sets.

      template <typename I, typename II>
      oln_type_of(I, concrete)
      geodesic_dilation_(const abstract::binary_image<I>&  marker,
			 const abstract::binary_image<II>& mask,
			 unsigned n)
      {
	oln_type_of(I, concrete) dil("dil"), work("work"), output("output");

	work = marker;
	for (unsigned i = 0; i < n; ++i)
	  {
	    dil = elementary_dilation(work);
	    work = level::f_and(dil, mask); // FIXME: should be as simple as "dil and mask"...
	  }
	output = work;

	return output;
      }

      // On functions.

      template <typename I, typename II>
      oln_type_of(I, concrete)
      geodesic_dilation_(const abstract::not_binary_image<I>&  marker,
			 const abstract::not_binary_image<II>& mask,
			 unsigned n)
      {
	oln_type_of(I, concrete) dil("dil"), work("work"), output("output");

	work = marker;
	for (unsigned i = 0; i < n; ++i)
	  {
	    dil = elementary_dilation(work);
	    work = level::inf(dil, mask);
	  }
	output = work;

	return output;
      }

    } // end of namespace oln::morpho::impl



    // Facade for geodesic dilation of size n (default is n = 1).


    template <typename I, typename II>
    oln_type_of(I, concrete)
    geodesic_dilation(const abstract::image<I>&  marker,
		      const abstract::image<II>& mask,
		      unsigned n = 1)
    {
      mlc::eq<oln_type_of(I, grid), oln_type_of(II, grid)>::ensure();
      mlc_is_a(I, abstract::image_with_nbh)::ensure();

      entering("morpho::geodesic_dilation");
      registering(marker, "marker");
      registering(mask,   "mask");

      precondition(marker.size() == mask.size()
		   and marker <= mask);
      precondition(n >= 1);

      oln_type_of(I, concrete) output;
      output = impl::geodesic_dilation_(marker.exact(), mask.exact(), n);
      
      exiting("morpho::geodesic_dilation");
      return output;
    }


  } // end of namespace oln::morpho

} // end of namespace oln


#endif // ! OLENA_MORPHO_GEODESIC_DILATION_HH

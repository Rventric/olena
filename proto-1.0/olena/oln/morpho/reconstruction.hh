// Copyright (C) 2001, 2002, 2004, 2005  EPITA Research and Development Laboratory
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
// As a special exception, you may use this filek as part of a free
// software library without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to
// produce an executable, this file does not by itself cause the
// resulting executable to be covered by the GNU General Public
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef OLENA_MORPHO_RECONSTRUCTION_HH
# define OLENA_MORPHO_RECONSTRUCTION_HH

# include <oln/morpho/reconstruction_by_dilation.hh>
# include <oln/morpho/reconstruction_by_erosion.hh>

namespace oln {


  namespace morpho {


    namespace impl {

      // Generic implementation of reconstruction (routine).

      template<typename Op, typename A, typename I1, typename I2>
      oln_type_of(I1, concrete)
      reconstruction_(const abstract::image_with_nbh<I1>& marker,
		      const abstract::image<I2>&          mask)
      {
	reconstruction<Op, A, I1, I2> tmp(marker, mask);
	// tmp.entering(); FIXME: something like that ?
	tmp.run();
	// tmp.exiting(); FIXME: something like that ?
	return tmp.get_output();
      }

    } // end of namespace impl

    /// Generic reconstruction (facade).

    template<typename Op, typename I1, typename I2, typename A>
    oln_type_of(I1, concrete)
    reconstruction(const tag::oper<Op>&			oper_,
		   const abstract::image_with_nbh<I1>&	marker,
		   const abstract::image<I2>&		mask,
		   const tag::algo<A>&			algo_)
    {
      mlc::eq<oln_type_of(I1, grid), oln_type_of(I2, grid)>::ensure();
      precondition(marker.size() == mask.size());

      entering("morpho::reconstruction");

      oln_type_of(I1, concrete) output("output");
//       output = impl::reconstruction_(oper_, marker.exact(), mask.exact(), algo_);

      exiting("morpho::reconstruction");
      return output;

    }

    // by dilation

    template<typename I1, typename I2, typename A>
    oln_type_of(I1, concrete)
    reconstruction_by_dilation(const abstract::image_with_nbh<I1>&	marker,
			       const abstract::image<I2>&		mask,
			       const tag::algo<A>&			algo_)
    {
      mlc::eq<oln_type_of(I1, grid), oln_type_of(I2, grid)>::ensure();
      precondition(marker.size() == mask.size());

      return reconstruction(tag::by_dilation_type(), marker, mask, algo_);
    }

    template<typename I1, typename I2>
    oln_type_of(I1, concrete)
    reconstruction_by_dilation(const abstract::image_with_nbh<I1>& marker,
			       const abstract::image<I2>&          mask)
    {
      mlc::eq<oln_type_of(I1, grid), oln_type_of(I2, grid)>::ensure();
      precondition(marker.size() == mask.size());

      return reconstruction(tag::by_dilation_type(), marker,
			    mask, tag::hybrid_type());
    }

    // by erosion

    template<typename I1, typename I2, typename A>
    oln_type_of(I1, concrete)
    reconstruction_by_erosion(const abstract::image_with_nbh<I1>&	marker,
			       const abstract::image<I2>&		mask,
			       const tag::algo<A>&			algo_)
    {
      mlc::eq<oln_type_of(I1, grid), oln_type_of(I2, grid)>::ensure();
      precondition(marker.size() == mask.size());

      return reconstruction(tag::by_erosion_type(), marker, mask, algo_);
    }

    template<typename I1, typename I2>
    oln_type_of(I1, concrete)
    reconstruction_by_erosion(const abstract::image_with_nbh<I1>& marker,
			       const abstract::image<I2>&          mask)
    {
      mlc::eq<oln_type_of(I1, grid), oln_type_of(I2, grid)>::ensure();
      precondition(marker.size() == mask.size());

      return reconstruction(tag::by_erosion_type(), marker,
			    mask, tag::hybrid_type());
    }

  } // end of namespace oln::morpho

} // end of namespace oln

#endif // ! OLENA_MORPHO_RECONSTRUCTION_HH

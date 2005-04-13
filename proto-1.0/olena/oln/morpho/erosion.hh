// Copyright (C) 2001, 2002, 2003, 2004, 2005  EPITA Research and Development Laboratory
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

#ifndef OLENA_MORPHO_EROSION_HH
# define OLENA_MORPHO_EROSION_HH

# include <mlc/cmp.hh>

# include <oln/core/abstract/image_operator.hh>
# include <oln/morpho/stat.hh>


namespace oln {

  namespace morpho {


    namespace proc {

      /// Erosion as a procedure (do not use it; prefer morpho::erosion).

      template<typename I, typename W>
      oln_type_of(I, concrete) erosion(const abstract::image<I>& input,
				       const abstract::window<W>& win)
      {
	oln_type_of(I, concrete) output(input.size());
	oln_type_of(I, fwd_piter) p(input.size());
	for_all (p)
	  output[p] = morpho::min(input, p, win);
	return output;
      }

    } // end of namespace oln::morpho::proc



    /*!
    ** \brief Perform a morphological erosion.
    **
    **   Compute the morphological erosion of input using win
    **   as structuring element.
    **
    **   On grey-scale  images, each point is replaced  by the minimum
    **   value  of  its neighbors,  as  indicated  by  win.  On  binary
    **   images, a  logical and  is performed between  neighbors.  The
    **   morpho::fast  version  of   this  function  use  a  different
    **   algorithm:  an histogram  of  the value  of the  neighborhood
    **   indicated by win is updated  while iterating over all point of
    **   the image.   Doing so is  more efficient when  the structuring
    **   element is large.
    **
    ** \param I Exact type of the input image.
    ** \param W Exact type of the structuring element.
    **
    **
    ** \arg input Input image.
    ** \arg win Structuring element to use.
    **
    ** \code
    ** #include <oln/basics2d.hh>
    ** #include <oln/morpho/erosion.hh>
    ** #include <oln/level/compare.hh>
    ** #include <ntg/all.hh>
    ** int main()
    ** {
    **   typedef oln::image2d<ntg::bin>	im_type;
    **
    **   im_type im1(oln::load(IMG_IN "object.pbm"));
    **   save(oln::morpho::erosion(im1, oln::win_c8p()),
    **        IMG_OUT "oln_morpho_erosion.pbm");
    ** }
    ** \endcode
    **
    ** \image html object_pbm.png
    ** \image latex object_pbm.png
    ** =>
    ** \image html oln_morpho_erosion.png
    ** \image latex oln_morpho_erosion.png
    **
    ** \see oln::morpho::fast::erosion()
    */

    // fwd decl
    template <typename I, typename W> struct erosion_ret;

  }

  // super_type
  template <typename I, typename W>
  struct set_super_type< morpho::erosion_ret<I,W> >
  {
    typedef abstract::image_unary_operator<oln_type_of(I, concrete), I, morpho::erosion_ret<I,W> > ret;
  };




  namespace morpho {

    /// Erosion return.

    template <typename I, typename W>
    struct erosion_ret : public abstract::image_unary_operator<oln_type_of(I, concrete), I, erosion_ret<I,W> >
    {
      typedef abstract::image_unary_operator<oln_type_of(I, concrete), I, erosion_ret<I,W> > super_type;
      typedef typename super_type::output_type output_type;

      const W win;

      erosion_ret(const abstract::image<I>& input,
		  const abstract::window<W>& win) :
	super_type(input),
	win(win.exact())
	{
	}

    };



    namespace impl {

      /// Erosion generic implementation.

      template <typename I, typename W>
      struct generic_erosion : public erosion_ret<I,W>
      {
	typedef erosion_ret<I,W> super_type;
	typedef typename super_type::output_type output_type;

	generic_erosion(const abstract::image<I>& input,
			const abstract::window<W>& win) :
	  super_type(input, win)
	{
	}

	void impl_run()
	{
	  mlc::eq<oln_type_of(I, size), oln_wn_type_of(W, size)>::ensure();

	  output_type tmp(this->input.size()); // FIXME: trick
	  this->output = tmp;

	  oln_type_of(I, fwd_piter) p(this->input.size());
	  for_all (p)
	    this->output[p] = morpho::min(this->input, p, this->win);
	}
      };

    } // end of namespace oln::morpho::impl


    /// Erosion generic routine.

    template<typename I, typename W>
    erosion_ret<I,W> erosion(const abstract::image<I>& input,
			     const abstract::window<W>& win)
    {
      impl::generic_erosion<I,W> tmp(input, win);
      tmp.run();
      return tmp;
    }


  } // end of namespace oln::morpho

} // end of namespace oln


#endif // ! OLENA_MORPHO_EROSION_HH

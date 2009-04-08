// Copyright (C) 2009 EPITA Research and Development Laboratory
// (LRDE)
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
// the Free Software Foundation, 51 Franklin Street, Fifth Floor,
// Boston, MA 02111-1307, USA.
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


#ifndef SCRIBO_BINARIZATION_SIMPLE_HH
# define SCRIBO_BINARIZATION_SIMPLE_HH

/// \file scribo/binarization/simple.hh
///
/// Simple binarization of a gray-level document.

# include <mln/core/image/image1d.hh>
# include <mln/core/alias/neighb1d.hh>

# include <mln/core/image/image2d.hh>
# include <mln/core/alias/neighb2d.hh>

# include <mln/core/routine/duplicate.hh>
# include <mln/core/image/image_if.hh>
# include <mln/pw/all.hh>

# include <mln/histo/compute.hh>
# include <mln/debug/histo.hh>
# include <mln/convert/from_to.hh>

# include <mln/morpho/elementary/gradient_external.hh>
# include <mln/morpho/closing/height.hh>
# include <mln/morpho/closing/volume.hh>
# include <mln/morpho/watershed/flooding.hh>

# include <mln/linear/gaussian_1d.hh>
# include <mln/labeling/regional_minima.hh>
# include <mln/labeling/compute.hh>
# include <mln/value/label_8.hh>
# include <mln/accu/center.hh>



namespace scribo
{

  namespace binarization
  {

    using namespace mln;

    /// Simple binarization of a gray-level document.
    /*!
     *
     * \param[in] input_ A binary image.
     * \param[in] nbh_ The neighborhood used for labeling image components.
     * \param[out] nlines The number of lines found.
     * \param[in] vwin Window used to extract the vertical lines in a morphological
     *		       opening
     * \param[in] hwin Window used to extract the horizontal lines in a morphological
     *		       opening
     *
     * \return pair of array of bounding boxes. The first array holds the
     *		vertical lines bounding boxes and the second one the
     *		horizontal lines bounding boxes.
     */
    template <typename I>
    mln_ch_value(I, bool)
    simple(const Image<I>& input);



# ifndef MLN_INCLUDE_ONLY

    template <typename I>
    inline
    mln_ch_value(I, bool)
    simple(const Image<I>& input_)
    {
      trace::entering("scribo::binarization::simple");

      const I& input = exact(input_);
      mln_precondition(input.is_valid());

      const float sigma = 5; // FIXME: hard-coded!

      typedef mln_value(I) V;
      histo::array<V> h;

      {
	mln_concrete(I) g;
	g = morpho::elementary::gradient_external(input, c4());
	g = morpho::closing::height(g, c4(), 5);  // FIXME: hard-coded!

	unsigned nbasins;
	mln_ch_value(I, unsigned) w = morpho::watershed::flooding(g, c4(), nbasins);
	h = histo::compute(input | (pw::value(w) == pw::cst(0)));
      }


      util::array<point1d> c;
      value::label_8 n;

      {
	image1d<unsigned> h_, hs_;
	image1d<value::label_8> l;

	convert::from_to(h, h_);
	hs_ = linear::gaussian_1d(h_, sigma, 0);
	l = labeling::regional_minima(hs_, c2(), n);
	
	if (n < 3)
	  {
	    std::cerr << "This method has not worked properly!" << std::endl;

	    debug::histo(h, "tmp_h.txt");
	    
	    std::ofstream file("tmp_hs.txt");
	    mln_piter(box1d) p(h_.domain());
	    for_all(p)
	      file << p.ind() << ' ' << hs_(p) << std::endl;
	    file.close();

	    std::abort();
	  }

	accu::center<point1d, point1d> a;
	c = labeling::compute(a, l, n);
	c[0] = point1d(0); // Force a neutral value for the non-label value (0).
      }

      int threshold;

      {
	std::vector<int> v;
	for (unsigned i = 0; i <= n; ++i)
	  v.push_back(c[i].ind());
	std::sort(v.begin(), v.end());
	threshold = v[n.prev()];

	// std::cout << "threshold = " << threshold << std::endl;
      }

      mln_ch_value(I, bool) output;
      output = duplicate((pw::value(input) > pw::cst(threshold)) | input.domain());

      trace::exiting("scribo::binarization::simple");
      return output;
    }

# endif // !MLN_INCLUDE_ONLY

  } // end of namespace scribo::binarization

} // end of namespace scribo


#endif // ! SCRIBO_BINARIZATION_SIMPLE_HH
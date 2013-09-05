// Copyright (C) 2009, 2011, 2012, 2013 EPITA Research and Development
// Laboratory (LRDE)
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

#ifndef MLN_WORLD_BINARY_2D_SUBSAMPLE_HH
# define MLN_WORLD_BINARY_2D_SUBSAMPLE_HH

/// \file
///
/// Subsample a Boolean image.
///
/// FIXME: not enough generic. Does not work on image having
///	   the top left corner different from (0,0).
/// FIXME: To be removed ?

# include <mln/core/image/image2d.hh>
# include <mln/core/alias/dpoint2d.hh>
# include <mln/data/convert.hh>
# include <mln/value/int_u8.hh>


namespace mln
{

  namespace world
  {

    namespace binary_2d
    {

      /// \brief Subsample a Boolean image.
      ///
      /// Subsample a binary image by a factor \p n and return a gray
      /// level image by averaging.
      ///
      /// \[
      /// out(p) = \frac{1}{n^2} \sum_{\epsilon=1}^n input(n.(p+pmin) +
      /// \langle \epsilon, 1\rangle )
      /// \]
      ///
      /// Note that even if the input image does not have its origin
      /// at (0,0), the output image does so.
      ///
      /// \param[in] input A binary image.
      /// \param[in] n	   Linear subsampling coefficient.
      ///
      /// \return A gray level image.
      image2d<value::int_u8>
      subsample(const image2d<bool>& input, unsigned n);


# ifndef MLN_INCLUDE_ONLY

      inline
      image2d<value::int_u8>
      subsample(const image2d<bool>& input, unsigned n)
      {
	mln_trace("world::binary_2d::subsample");

	mln_precondition(input.is_valid());

	using value::int_u8;

	if (n == 0)
	  return data::convert(int_u8(), input);


	const unsigned nrows = input.nrows() / n;
	const unsigned ncols = input.ncols() / n;

	algebra::vec<2, unsigned int> vmin;
	algebra::vec<2, unsigned int> vmax;
	vmin[0] = 0;
	vmin[1] = 0;
	vmax[0] = nrows - 1;
	vmax[1] = ncols - 1;
	point2d pmin(vmin);
	point2d pmax(vmax);

	image2d<int_u8> output(box<point2d>(pmin, pmax));
	dpoint2d dp_row(1, 0);
	const unsigned delta_row = input.delta_index(dp_row);

	const bool* origin = &input(input.domain().pmin());
	const bool* line_ptr = origin;
	for (unsigned row = 0; row < nrows; ++row, line_ptr += delta_row * n)
	{
	  const bool* ptr = line_ptr;
	  for (unsigned col = 0; col < ncols; ++col, ptr += n)
	  {
	    unsigned count = 0;
	    for (unsigned i = 0; i < n; ++i)
	      for (unsigned j = 0; j < n; ++j)
	      {
		if (ptr[delta_row * i + j])
		  ++count;
	      }
	    output.at_(row, col) = count * 255 / n / n;
	  }
	}

	return output;
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::world::binary_2d

  } // end of namspace mln::world

} // end of namespace mln

#endif // ! MLN_WORLD_BINARY_2D_SUBSAMPLE_HH

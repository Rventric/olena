// Copyright (C) 2011, 2013 EPITA Research and Development Laboratory (LRDE)
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

#ifndef SCRIBO_ESTIM_COMPONENT_FEATURES_HH
# define SCRIBO_ESTIM_COMPONENT_FEATURES_HH

# include <mln/core/image/image2d.hh>
# include <mln/core/alias/neighb2d.hh>
# include <mln/topo/skeleton/crest.hh>
# include <mln/topo/skeleton/is_simple_point.hh>
# include <mln/transform/distance_front.hh>
# include <mln/logical/not.hh>
# include <mln/make/w_window2d_int.hh>
# include <mln/value/int_u8.hh>
# include <mln/arith/revert.hh>
# include <mln/morpho/skeleton_constrained.hh>
# include <mln/core/routine/extend.hh>
# include <mln/labeling/blobs.hh>
# include <mln/accu/stat/mean.hh>
# include <mln/make/dual_neighb.hh>


# include <mln/data/compute.hh>
# include <mln/io/pbm/save.hh>
# include <mln/io/pgm/save.hh>
# include <mln/util/timer.hh>

# include <scribo/core/component_set.hh>
# include <scribo/estim/font_color.hh>
# include <scribo/estim/font_boldness.hh>

#include <mln/data/convert.hh>


namespace scribo
{

  namespace estim
  {

    using namespace mln;

    /// \brief Compute components features.
    ///
    /// \sa component_features_data
    //
    template <typename I, typename J, typename L>
    component_set<L>
    components_features(const Image<I>& input, const Image<J>& bin_input,
			const component_set<L>& components);


# ifndef MLN_INCLUDE_ONLY


    template <typename I, typename J, typename L>
    component_set<L>
    components_features(const Image<I>& input_, const Image<J>& bin_input_,
			const component_set<L>& components)
    {
      mln_trace("scribo::estim::components_features");

      const I& input = exact(input_);
      const J& bin_input = exact(bin_input_);

      mln_precondition(input.is_valid());
      mln_precondition(bin_input.is_valid());

      component_set<L> output = components.duplicate();

      component_features_data features;
      features.valid = true;

      typedef mln_concrete(J) B;
      B skel;
      L skel_lbl;
      mln_value(L) nlabels;

      {
	int psi = 7;
	int vals[] = { 0, 9, 0, 9, 0,
		       9, 6, 4, 6, 9,
		       0, 4, 0, 4, 0,
		       9, 6, 4, 6, 9,
		       0, 9, 0, 9, 0 };

	typedef mln_ch_value(I,value::int_u8) dist_t;
	dist_t
	  dist = transform::distance_front(logical::not_(bin_input), c8(),
					   mln::make::w_window2d_int(vals),
					   mln_max(value::int_u8));
	dist_t dist_map = arith::revert(dist);
	B K = topo::skeleton::crest(bin_input, dist, c8(), psi);

	typedef
	  neighb<win::multiple_size<2u, window2d, pw::value_<J> > >
	  nbh_t;
	nbh_t nbh = mln::make::dual_neighb(bin_input, c8(), c4());

	skel =
	  morpho::skeleton_constrained(bin_input, nbh,
				       topo::skeleton::is_simple_point<nbh_t>(nbh),
				       K, dist_map);

      }

      const L& lbl = output.labeled_image();

      for_all_comps(c, output)
      {
	// Estimate component color
	features.color = data::compute(accu::meta::stat::mean(),
				       ((input | output(c).bbox())
					| (pw::value(lbl) == pw::cst(c)
					   && pw::value(skel))));


	// Estimate component boldness
	features.boldness = internal::boldness_from_lbl(
	  (lbl | output(c).bbox()) | (pw::value(lbl) == pw::cst(c)),
	  (skel | output(c).bbox()) | (pw::value(lbl) == pw::cst(c)));

	output(c).update_features(features);
      }

      return output;
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::estim

} // end of namespace scribo

# endif // ! SCRIBO_ESTIM_COMPONENT_FEATURES_HH

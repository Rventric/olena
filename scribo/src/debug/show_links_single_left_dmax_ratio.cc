// Copyright (C) 2009, 2010, 2011, 2013 EPITA Research and Development
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

#include <iostream>

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>

#include <mln/data/convert.hh>

#include <mln/value/rgb8.hh>
#include <mln/value/label_16.hh>
#include <mln/literal/colors.hh>

#include <mln/io/pbm/load.hh>
#include <mln/io/ppm/save.hh>

#include <mln/draw/line.hh>

#include <scribo/primitive/extract/components.hh>
#include <scribo/primitive/link/with_single_left_link_dmax_ratio.hh>
#include <scribo/primitive/link/internal/link_single_dmax_ratio_base.hh>
#include <scribo/primitive/link/compute.hh>

#include <scribo/draw/bounding_boxes.hh>

#include <scribo/debug/usage.hh>


namespace scribo
{

  template <typename I, typename L>
  struct single_left_link_debug_functor
    : primitive::link::internal::link_single_dmax_ratio_base<L,
							     primitive::link::internal::dmax_default,
							     single_left_link_debug_functor<I, L> >
  {
    typedef single_left_link_debug_functor<I, L> self_t;
    typedef primitive::link::internal::dmax_default dmax_fun_t;
    typedef
    primitive::link::internal::link_single_dmax_ratio_base<L,
							   dmax_fun_t,
							   self_t> super_;

  public:
    typedef mln_site(L) P;

    single_left_link_debug_functor(const I& input,
				   const component_set<L>& components,
				   float dmax_ratio)
      : super_(components, anchor::Horizontal, dmax_fun_t(dmax_ratio))
    {
      output_ = data::convert(input, value::rgb8());
      scribo::draw::bounding_boxes(output_, components, literal::blue);
      mln_postcondition(output_.is_valid());
    }

    void validate_link_(unsigned current_object,
			const P& start_point,
			const P& p,
			anchor::Type anchor)
    {
      mln::draw::line(output_, start_point, p, literal::green);

      super_::validate_link_(current_object, start_point, p, anchor);
    }



    void invalidate_link_(unsigned current_object,
			  const P& start_point,
			  const P& p,
			  anchor::Type anchor)
    {
      if (output_.domain().has(p))
	mln::draw::line(output_, start_point, p, literal::red);
      else
      {
	P tmp = p;
	++tmp.col();
	mln::draw::line(output_, start_point, tmp, literal::red);
      }

      super_::invalidate_link_(current_object, start_point, p, anchor);
    }



    void compute_next_site_(P& p)
    {
      --p.col();
    }


    image2d<value::rgb8> output_;
  };


}


const char *args_desc[][2] =
{
  { "input.pbm", "A binary image. True for objects and False for the "
    "background." },
  { "max_nbh_dist_ratio", "Size ratio defining the maximum lookup distance."
    "(common value : 3)" },
  {0, 0}
};


int main(int argc, char* argv[])
{
  using namespace scribo;
  using namespace scribo::primitive::internal;
  using namespace mln;

  if (argc != 4)
    return scribo::debug::usage(argv,
				"Show sucessful/unsuccessful left links "
				"between components.",
				"input.pbm max_nbh_dist_ratio output.ppm",
				args_desc);

  typedef image2d<bool> I;
  I input;
  io::pbm::load(input, argv[1]);

  // Finding objects.
  value::label_16 nbboxes;
  typedef image2d<value::label_16> L;
  component_set<L> components
    = scribo::primitive::extract::components(input, c8(), nbboxes);

  // Write debug image.
  single_left_link_debug_functor<I, L>
    functor(input, components, atof(argv[2]));
  primitive::link::compute(functor, anchor::MassCenter);

  io::ppm::save(functor.output_, argv[3]);
}

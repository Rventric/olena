/// \file
/// \brief First generic routine.

// \{
#include <mln/core/image/image2d.hh>
#include <mln/core/image/dmorph/image_if.hh>
#include <mln/core/alias/neighb2d.hh>

#include <mln/data/fill.hh>

#include <mln/labeling/blobs.hh>
#include <mln/labeling/compute.hh>

#include <mln/data/compare.hh>

#include <mln/util/array.hh>

#include <mln/value/label_8.hh>

#include <mln/accu/math/count.hh>

#include <mln/pw/all.hh>

#include <tests/data.hh>
#include <doc/tools/sample_utils.hh>

namespace mln
{

  template <typename I, typename N>
  mln_concrete(I)
  my_algorithm(const Image<I>& ima_,
	       const Neighborhood<N>& nbh_)
  {
    trace::entering("my_algorithm");

    const I& ima = exact(ima_);
    const N& nbh = exact(nbh_);
    mln_precondition(ima.is_valid());
    mln_precondition(nbh.is_valid());

    typedef value::label_8 V;
    V nlabels;
    mln_ch_value(I,V) lbl = labeling::blobs(ima, nbh, nlabels);
    util::array<unsigned>
      count = labeling::compute(accu::meta::math::count(),
				lbl,
				nlabels);

    mln_concrete(I) output;
    initialize(output, ima);
    data::fill(output, literal::one);

    for (unsigned i = 1; i <= nlabels; ++i)
      if (count[i] < 10u)
	data::fill((output | (pw::value(lbl) == pw::cst(i))).rw(),
		   literal::zero);

    trace::exiting("my_algorithm");
    return output;
  }

} // end of namespace mln
// \}

namespace sandbox
{

  using namespace mln;

  // \{
  template <typename I, typename N>
  mln_concrete(I)
  my_algorithm(const Image<I>& ima_,
	       const Neighborhood<N>& nbh_)
  // \}
  {
    // \{
    trace::entering("my_algorithm");
    // \}

    // \{
    const I& ima = exact(ima_);
    const N& nbh = exact(nbh_);
    mln_precondition(ima.is_valid());
    mln_precondition(nbh.is_valid());
    // \}

    // \{
    typedef value::label_8 V;
    V nlabels;
    mln_ch_value(I,V) lbl = labeling::blobs(ima, nbh, nlabels);
    util::array<unsigned>
      count = labeling::compute(accu::meta::math::count(),
				lbl,
				nlabels);
    // \}

    // \{
    mln_concrete(I) output;
    initialize(output, ima);
    data::fill(output, literal::one);
    // \}

    // \{
    for (unsigned i = 1; i <= nlabels; ++i)
      if (count[i] < 10u)
	data::fill((output | (pw::value(lbl) == pw::cst(i))).rw(),
		   literal::zero);
    // \}

    // \{
    trace::exiting("my_algorithm");
    return output;
    // \}
  }

} // end of namespace mln

int main()
{
  mln::image2d<bool> test(2,2);
  mln_assertion(sandbox::my_algorithm(test, mln::c4())
		  == mln::my_algorithm(test, mln::c4()));
}

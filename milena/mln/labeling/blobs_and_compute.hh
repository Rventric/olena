// Copyright (C) 2009, 2010, 2012, 2013 EPITA Research and Development
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

#ifndef MLN_LABELING_BLOBS_AND_COMPUTE_HH
# define MLN_LABELING_BLOBS_AND_COMPUTE_HH

/// \file
///
/// Label an image and compute given accumulators.


# include <mln/core/concept/image.hh>
# include <mln/core/concept/neighborhood.hh>

# include <mln/canvas/labeling/blobs.hh>
# include <mln/labeling/compute.hh>

# include <mln/util/couple.hh>

namespace mln
{

  namespace labeling
  {

    /*! \brief Label an image and compute given accumulators.

      \param[in]     input   A binary image.
      \param[in]     nbh     A neighborhood used for labeling.
      \param[in,out] nlabels The number of labels found.
      \param[in]     accu    An accumulator to be computed while labeling.
      \param[in] background_value The value to be considered as
                                  background and not to be labeled.

      \return The labeled image, computed attributes for each regions
              and an array of the accumulators used to compute the
              attributes.

      \ingroup labeling
     */
    template <typename I, typename N, typename L, typename A>
    util::couple<mln_ch_value(I,L),
		 util::couple<util::array<mln_result(A)>,
			      util::array<A> > >
    blobs_and_compute(const Image<I>& input, const Neighborhood<N>& nbh,
		      L& nlabels, const Accumulator<A>& accu,
		      const mln_value(I)& background_value);

    /*! \brief Label an image and compute given accumulators.

      background_value is set to literal::zero.

      \ingroup labeling
     */
    template <typename I, typename N, typename L, typename A>
    util::couple<mln_ch_value(I,L),
		 util::couple<util::array<mln_result(A)>,
			      util::array<A> > >
    blobs_and_compute(const Image<I>& input, const Neighborhood<N>& nbh,
		      L& nlabels, const Accumulator<A>& accu);


# ifndef MLN_INCLUDE_ONLY


    namespace internal
    {

      /*!
	\internal
	\brief Functor not computing anything.
	To be passed to the labeling  blobs canvas.
      */
      template <typename I, typename L, typename A>
      struct compute_functor
      {
	typedef mln_result(A) accu_result;
	typedef mln_argument(A) accu_argument;
	typedef util::couple<util::array<accu_result>,
			     util::array<A> > result;

	compute_functor(const mln_value(L)& nlabels, const I& input,
			const mln_value(I)& background_value)
	  : nlabels_(nlabels), input_(input),
	    background_value_(background_value)
	{
	}

	inline
	void init()
	{
	  accus_.reserve(1000);
	  accus_.append(A());
	}

	inline
	mln_value(I) neutral_value() const
	{
	  return background_value_;
	}

	inline
	bool handles(const mln_value(I)& v)
	{
	  return v != background_value_;
	}

	inline
	void new_label(const mln_value(L)& l)
	{
	  current_lbl_ = l;
	  accus_.append(A());
	}

	inline
	void process_p(const mln_site(L)& p)
	{
	  process__(accu_argument(), p);
	}

	inline
	void process_n(const mln_site(L)& n)
	{
	  process__(accu_argument(), n);
	}

	inline
	void finalize()
	{
	  convert::from_to(accus_, result_);
	}


	// Fastest interface

	inline
	void process_p_(const unsigned p)
	{
	  process__(accu_argument(), p);
	}

	inline
	void process_n_(const unsigned n)
	{
	  process__(accu_argument(), n);
	}


      private:
	inline
	void process__(const mln_psite(L)&, const mln_site(L)& p)
	{
	  accus_[current_lbl_].take(p);
	}

	inline
	void process__(const mln_value(L)&, const mln_site(L)&)
	{
	  accus_[current_lbl_].take(current_lbl_);
	}

	inline
	void process__(const mln_psite(L)&, const unsigned p)
	{
	  accus_[current_lbl_].take(input_.point_at_offset(p));
	}

	inline
	void process__(const mln_value(L)&, const unsigned)
	{
	  accus_[current_lbl_].take(current_lbl_);
	}


	template <typename V>
	inline
	void process__(const V&, const mln_site(L)&)
	{
	  mlc_abort(V)::check();
	}

      public:
	util::array<mln_result(A)> result_;
	util::array<A> accus_;
	mln_value(L) current_lbl_;
	mln_value(L) nlabels_;
	const I input_;
	mln_value(I) background_value_;
      };

    } // end of namespace mln::labeling::internal



    // Facade.


    template <typename I, typename N, typename L, typename A>
    util::couple<mln_ch_value(I,L),
		 util::couple<util::array<mln_result(A)>,
			      util::array<A> > >
    blobs_and_compute(const Image<I>& input, const Neighborhood<N>& nbh,
		      L& nlabels, const Accumulator<A>& accu,
		      const mln_value(I)& background_value)
    {
      mln_trace("labeling::blobs_and_compute");

      (void) accu;
      mln_precondition(exact(input).is_valid());
      mln_precondition(exact(nbh).is_valid());

      typedef mln_ch_value(I,L) out_t;
      typedef internal::compute_functor<I,out_t,A> func_t;
      func_t functor(nlabels, exact(input), background_value);
      out_t
	output = canvas::labeling::blobs(input, nbh, nlabels, functor);

      util::couple<out_t, typename func_t::result>
	result = make::couple(output,
			      make::couple(functor.result_, functor.accus_));

      return result;
    }


    template <typename I, typename N, typename L, typename A>
    util::couple<mln_ch_value(I,L),
		 util::couple<util::array<mln_result(A)>,
			      util::array<A> > >
    blobs_and_compute(const Image<I>& input, const Neighborhood<N>& nbh,
		      L& nlabels, const Accumulator<A>& accu)
    {
      return blobs_and_compute(input, nbh, nlabels, accu,
			       static_cast<mln_value(I)>(literal::zero));
    }


# endif // ! MLN_INCLUDE_ONLY


  } // end of namespace mln::labeling

} // end of namespace mln


#endif // ! MLN_LABELING_BLOBS_AND_COMPUTE_HH

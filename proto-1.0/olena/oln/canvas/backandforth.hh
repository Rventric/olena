// Copyright (C) 2005  EPITA Research and Development Laboratory
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

#ifndef OLENA_CANVAS_BACKANDFORTH_HH
# define OLENA_CANVAS_BACKANDFORTH_HH

# include <mlc/any.hh>
# include <mlc/contract.hh>
# include <oln/core/abstract/image.hh>
# include <oln/core/abstract/piter.hh>


namespace oln {


  namespace canvas
  {

    template <typename I, typename E>
    struct back_and_forth_until_convergence : public mlc::any<E>
    {
      // Abstract methods.

      // Initialize 'work'.
      void init()
      {
	this->exact().impl_init();
      }

      // Do something like 'work -> output'.
      void bkd_loop_body()
      {
	this->exact().impl_bkd_loop_body();
      }

      // Do something like 'work -> output'.
      void fwd_loop_body()
      {
	this->exact().impl_fwd_loop_body();
      }

      // Check stability from input, output, and work.
      bool is_stable() const
      {
	return this->exact().impl_is_stable();
      }

      // Do something like 'work := output'.
      void re_loop()
      {
	return this->exact().impl_re_loop();
      }

      // Concrete method.
      void run()
      {
	bool stability;
	init();
	for (;;)
	  {
	    // back
	    for_all_p (bkd_p)
	      bkd_loop_body();

	    // forth
	    for_all_p (fwd_p)
	      fwd_loop_body();

	    // stability check
	    stability = is_stable();
	    if (stability)
	      return;

	    // prepare a new loop iteration
	    re_loop();
	  }
      }


    protected:

      // Ctor.
      back_and_forth_until_convergence(const abstract::image<I>& input) :
	bkd_p(input.size()),
	fwd_p(input.size())
      {
      }

      // Attributes.
      oln_type_of(I, bkd_piter) bkd_p;
      oln_type_of(I, fwd_piter) fwd_p;

      // Check for impls..
      ~back_and_forth_until_convergence()
      {
	mlc_check_method_impl(E, void, init,          , );
	mlc_check_method_impl(E, void, bkd_loop_body, , );
	mlc_check_method_impl(E, void, fwd_loop_body, , );
	mlc_check_method_impl(E, bool, is_stable,     , const);
	mlc_check_method_impl(E, void, re_loop,       , );
      }

    };


    template <typename I, typename E>
    struct back_and_forth : public mlc::any<E>
    {
      // Abstract methods.

      // Initialize 'work'.
      void init()
      {
	this->exact().impl_init();
      }

      // Do something like 'work -> output'.
      void bkd_loop_body()
      {
	this->exact().impl_bkd_loop_body();
      }

      // Do something like 'work -> output'.
      void fwd_loop_body()
      {
	this->exact().impl_fwd_loop_body();
      }

      // Concrete method.
      void run()
      {
	init();

	// forth
	for_all_p (fwd_p)
	  fwd_loop_body();

	// back
	for_all_p (bkd_p)
	  bkd_loop_body();

      }


    protected:

      // Ctor.
      back_and_forth(const abstract::image<I>& input) :
	bkd_p(input.size()),
	fwd_p(input.size())
      {
      }

      // Attributes.
      oln_type_of(I, bkd_piter) bkd_p;
      oln_type_of(I, fwd_piter) fwd_p;

      // Check for impls..
      ~back_and_forth()
      {
	mlc_check_method_impl(E, void, init,          , );
	mlc_check_method_impl(E, void, bkd_loop_body, , );
	mlc_check_method_impl(E, void, fwd_loop_body, , );
      }

    };

  } // end of namespace oln::canvas


} // end of namespace oln


#endif // ! OLENA_CANVAS_BACKANDFORTH_HH

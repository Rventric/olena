// Copyright (C) 2008 EPITA Research and Development Laboratory
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

#include <iomanip>

#include <mln/core/image2d.hh>
#include <mln/core/neighb2d.hh>
#include <mln/core/p_array.hh>
#include <mln/core/clone.hh>
#include <mln/core/image_if_value.hh>
#include <mln/core/sub_image.hh>
#include <mln/core/p_queue_fast.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/rgb8.hh>

#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/io/ppm/save.hh>

#include <mln/level/fill.hh>
#include <mln/level/compare.hh>
#include <mln/debug/println.hh>
#include <mln/labeling/regional_minima.hh>
#include <mln/accu/bbox.hh>
#include <mln/geom/bbox.hh>
#include <mln/pw/all.hh>

#include <mln/literal/black.hh>
#include <mln/literal/white.hh>
#include <mln/literal/colors.hh>

#include <mln/util/tree.hh>

#include <sstream>


namespace mln
{

  namespace my
  {

# define fllt_tree(P, V)  mln::util::tree< fllt_node_elt<P, V> >
# define fllt_node(P, V)  mln::util::tree_node< fllt_node_elt<P, V> >
# define fllt_branch(P, V)  mln::util::branch< fllt_node_elt<P, V> >
# define fllt_branch_iter_ind(P, V)  mln::util::branch_iter_ind< fllt_node_elt<P, V> >

    template <typename P, typename V>
    struct fllt_node_elt
    {
      V	value;
      p_array<P> points;
      p_array<P> holes;
      /// Tell if his parent if brighter or not.  Nb : if the parent
      /// if brighter, the node come from the lower level set
      bool brighter;
      unsigned npoints;
    };

    template <typename N_t, typename G>
    void update_gN(const N_t& N, G& gN)
    {
      for (unsigned g = 0; g < 256; ++g)
	if (N[g]->npoints() != 0)
	  {
	    gN = g;
	    return;
	  }
      // if N is empty, gN is the max value.
      gN = 255;
    }


    template <typename N_t>
    void print_N(const N_t& N)
    {
      for (unsigned i = 0; i < 256; ++i)
	{
	  if (N[i]->npoints() == 0)
	    continue;
	  std::cout << i << ": " << *N[i] << std::endl;
	}
    }

    template <typename N_t>
    void clear_N(N_t& N)
    {
      for (unsigned i = 0; i < 256; ++i)
	N[i]->clear();
    }



    template <typename T>
    image2d<T> enlarge(const image2d<T>& input, unsigned coef)
    {
      unsigned
	nrows_ = coef * geom::nrows(input),
	ncols_ = coef * geom::ncols(input);
      image2d<T> output(nrows_, ncols_);
      for (int row = 0; row < nrows_; ++row)
	for (int col = 0; col < ncols_; ++col)
	  output.at(row, col) = input.at(row / coef, col / coef);
      return output;
    }


    void save_u(const image2d<value::int_u8>& u,
		const image2d<int>& is,
		box2d R_box,
		int in_R)
    {
      static int id = 0;
      std::stringstream filename;
      filename << "fllt_trace_" << std::setw(5) << std::setfill('0')
	       << std::right << id++ << ".ppm";

      image2d<value::int_u8> out = clone(u);

      mln_assertion(R_box.is_valid());
      mln_piter_(box2d) p(R_box);
      for_all(p)
	if (is(p) == in_R)
	  out(p) = 255;
// 	else if (is(p) == in_N)
// 	  out(p) = literal::green;

      io::pgm::save(out, filename.str());
      //io::pgm::save(out, filename.str());
    }

    template <typename I>
    void save(const I& is, unsigned in_N, unsigned in_R, const std::string& name = "")
    {
      static unsigned counter = 0;
      using value::rgb8;

      image2d<rgb8> temp(is.domain());
      level::fill(temp, literal::black);

      mln_piter(I) p(is.domain());
      for_all(p)
	if (is(p) == in_R)
	  temp(p) = literal::red;
	else if (is(p) == in_N) // N
	  temp(p) = literal::green;
	else if (is(p) < in_N)
	  temp(p) = literal::blue;
	else
	  temp(p) = literal::white;

      if (name == "")
	{
	  std::stringstream filename;
	  filename << "./temp_" << ++counter << ".ppm";
	  io::ppm::save(my::enlarge(temp, 10), filename.str());
	}
      else
	io::ppm::save(temp, name);
    }


    template <typename I, typename V>
    void blob(const I& is,
	      p_array<mln_point(I)>* N[256],
	      unsigned in_N,
	      fllt_node(mln_point(I), V)* current_cc)
    {
      typedef p_array<mln_point(I)> arr_t;

//       std::cout << ">>>>>>>enter blob." << std::endl;
      bool flower = true;
      unsigned ncc = 0;
      static image2d<unsigned> is_labeled(is.domain());
      static unsigned label = 0;

      if (label == 0)
      {
	level::fill(is_labeled, 0);
	label++;
      }

      typedef mln_psite(I) P;
      P cur;
      mln_niter(neighb2d) n(c8(), cur);
      p_queue_fast<P> qu;
      p_array<P>& holes = current_cc->elt().holes;


      for (unsigned i = 0; i < 256; ++i)
      {
	mln_piter(arr_t) p(*N[i]);
	for_all(p)
	{
	  mln_assertion(is(p) == in_N);
	  if (is_labeled(p) != label)
	  {
	    if (flower == false)
	      holes.append(p);
	    else
	      flower = false;
	    qu.push(p);
	    is_labeled(p) = label;
	    do
	    {
	      cur = qu.front();
	      qu.pop();
	      for_all(n) if (is.has(n))
		if (is(n) == in_N && is_labeled(n) != label)
		{
		  qu.push(n);
		  is_labeled(n) = label;
		}
	    }
	    while (! qu.is_empty());
	  }
	}
      }

      ++label;

//       if (holes.size() == 2)
// 	std::cout << holes[0] << holes[1] << std::endl;
//       std::cout << " <<<<<<<exiting blob." << std::endl;
    }


    template <typename I, typename Nbh>
    fllt_tree(mln_point(I), mln_value(I))&
      fllt(const Image<I>& input_, const Neighborhood<Nbh>& nbh_)
    {
      typedef fllt_node(mln_point(I), mln_value(I)) node_type;
      typedef fllt_tree(mln_point(I), mln_value(I)) tree_type;

      const I& input = exact(input_);
      const Nbh& nbh = exact(nbh_);

      unsigned l = 0, l_max;
      mln_ch_value(I, unsigned) reg_min = labeling::regional_minima(input, nbh, l_max);
      std::vector<bool> tag(l_max + 1, false);
      tag[0] = true;

      // Variables.
      I u = mln::clone(input);
      mln_point(I) x0;
      mln_value(I) g, gN;
      mln_fwd_piter(I) p(input.domain());
      p.start();


      node_type* current_cc;
      unsigned in_N = 1, in_R = 2;

      image2d<int> deja_vu(input.domain().to_larger(1));
      level::fill(deja_vu, 0);

      typedef p_array<mln_point(I)> arr_t;
      arr_t* A = new arr_t();
      arr_t* N[256];
      for (unsigned i = 0; i < 256; ++i)
	N[i] = new arr_t();
      accu::bbox<mln_point(I)> N_box;

      unsigned n_step_1 = 0, n_step_3 = 0, n_comps = 0, n_holes = 0;

      // Step 1.
    step_1:
      {
	while (tag[reg_min(p)] && p.is_valid())
	  p.next();
	if (p.is_valid())
	  tag[reg_min(p)] = true; // To be processed.
	else
	  goto the_end;

	++n_step_1;
	x0 = p;
	g = input(x0);
	current_cc = new node_type();
      }

      // Step 2.
    step_2:
      {
	in_N += 2;
	in_R = in_N + 1;
	// R <- 0 and N <- 0
	clear_N(N);
	N_box.init();

	// A <- { x0 }
	A->clear();
	A->append(x0);
	N_box.take(x0);

	deja_vu(x0) = in_R;
      }

      // Step 3.
    step_3:
      {
	++n_step_3;

	mln_piter(arr_t) a(*A);
	mln_niter(Nbh) x(nbh, a);


	// R <- R U A
	if (A->npoints() == 0)
	  goto the_end;

	// N <- N U { x in nbh of A and not in R }
	for_all(a)
	  for_all(x)
	  {
  	    if (deja_vu.has(x) && deja_vu(x) < in_N)
	      {
		if (u.has(x))
		{
		  N[u(x)]->append(x);
		  N_box.take(x);
		}
		deja_vu(x) = in_N;
	      }
	  }
	// gN = min u(x) for all x in N
	update_gN(N, gN);

	// FIXME: update the number of CC of the border of R
      }

      // Step 4.
    step_4:
      {
	// a)
	if (g < gN)
	  {
	    g = gN;

	    ++n_comps;
	    blob(deja_vu, N, in_N, current_cc);
	    node_type* parent = current_cc;

    	    n_holes += current_cc->elt().holes.npoints();
	    save_u(u, deja_vu, N_box, in_R);

	    arr_t* tmp = A;
	    A = N[g];
	    N[g] = tmp;
	    N[g]->clear();
	    mln_piter(arr_t) a(*A);
	    for_all(a)
	    {
	      mln_invariant(deja_vu(a) == in_N);
	      deja_vu(a) = in_R;
	      current_cc->elt().points.append(a);
	      // N_box is not re-computed so that we save time;
	      // N_box is always growing while looping from step 3.
	    }
	    goto step_3;
	  }
	// b)
	else if (g == gN)
	  {
	    arr_t* tmp = A;
	    A = N[g];
	    N[g] = tmp;
	    N[g]->clear();
	    mln_piter(arr_t) a(*A);
	    for_all(a)
	    {
	      mln_invariant(deja_vu(a) == in_N);
	      deja_vu(a) = in_R;
	      current_cc->elt().points.append(a);
	    }
	    goto step_3;
	  }
	// c)
	else
	  {
	    // FIXME: IDEA: this change might be performed while R is constructed(?)
	    mln_piter(I) r(N_box);
	    for_all(r)
	      if (deja_vu(r) == in_R)
		u(r) = g;

	    goto step_1;
	  }
      }

    the_end:
      std::cout << n_step_1 << ' ' << n_step_3 << std::endl;
      std::cout << "n comps = " << n_comps << "  n holes = " << n_holes << std::endl;

      return *new tree_type(current_cc);
    }

  } // end of namespace mln::my

} // end of namespace mln


int main(int argc, char* argv[])
{
  if (argc != 2)
    {
      std::cerr << "usage: " << argv[0] << " filename" << std::endl;
      return 1;
    }

  using namespace mln;
  using value::int_u8;

  image2d<int_u8> lena;
  io::pgm::load(lena, argv[1]);

  my::fllt(lena, c4());
  io::pgm::save(lena, "./out.pgm");

}

// 16891 99970
// ./a.out ../../img/lena.pgm  1.17s user 0.06s system 93% cpu 1.318 total
// matt @ ..k/lrde/oln/trunk/milena/sandbox/geraud $ time ./a.out ../../img/lena.pgm                                                                                                                          12:50:12
// 16891 99970
// ./a.out ../../img/lena.pgm  1.22s user 0.02s system 87% cpu 1.413 total
// matt @ ..k/lrde/oln/trunk/milena/sandbox/geraud $ time ./a.out ../../img/lena.pgm                                                                                                                          12:50:16
// 16891 99970
// ./a.out ../../img/lena.pgm  1.14s user 0.09s system 92% cpu 1.336 total
// matt @ ..k/lrde/oln/trunk/milena/sandbox/geraud $ time ./a.out ../../img/lena.pgm                                                                                                                          12:50:18
// 16891 99970
// ./a.out ../../img/lena.pgm  1.20s user 0.04s system 92% cpu 1.329 total

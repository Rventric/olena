// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef APPS_GRAPH_MORPHO_MORPHO_HH
# define APPS_GRAPH_MORPHO_MORPHO_HH

/** \file apps/graph-morpho/morpho.hh
    \brief Morphological operators on graphs (1-complexes).

    Reference:

      Jean Cousty, Laurent Najman and Jean Serra.  Some morphological
      operators in graph spaces.  In: Proceedings of the Ninth
      International Symposium on Mathematical Morphology (ISMM), 2009,
      Groningen, The Netherlands.  */

# include <mln/core/concept/image.hh>

# include <mln/core/routine/duplicate.hh>

# include <mln/core/site_set/p_n_faces_piter.hh>
# include <mln/core/image/complex_neighborhoods.hh>
# include <mln/core/image/complex_neighborhood_piter.hh>


/*----------------------------.
| Vertices-edges combinator.  |
`----------------------------*/

/// Combine the vertices and the edges of two images to create a new
/// graph image (``operator'' \f$\ovee\f$).
template <typename I>
inline
mln_concrete(I)
combine(const mln::Image<I>& vertices_, const mln::Image<I>& edges_)
{
  const I vertices = mln::exact(vertices_);
  const I edges = mln::exact(edges_);
  mln_precondition(vertices.domain() == edges.domain());

  mln_concrete(I) output;
  mln::initialize(output, vertices);
  mln::p_n_faces_fwd_piter<I::dim, mln_geom(I)> v(output.domain(), 0);
  for_all(v)
    output(v) = vertices(v);
  mln::p_n_faces_fwd_piter<I::dim, mln_geom(I)> e(output.domain(), 1);
  for_all(e)
    output(e) = edges(e);
  return output;
}


/*-------------------------.
| Dilations and erosions.  |
`-------------------------*/

/* FIXME: By constraining the domain of the input and passing the
   neighborhood, one should be able to use a truly generic dilation
   (resp. erosion), or even use Milena's standard morpho::dilation
   (resp. morpho::erosion).

   It'd be convenient to write something like this:

      dilation(ima | vertices);

   We can /actually/ write this, but `vertices' has to be a predicate
   on sites (p2b function), which is not efficient, since both
   vertices and edges will be browsed.

   It would be very nice if `vertices' could be an actual site set,
   so that `ima | vertices' creates a morpher smart enough to
   browse /only/ vertices.  */

/// Dilation from edges to vertices (\f$\delta^\bullet\f$).
template <typename I>
inline
mln_concrete(I)
dilation_e2v(const mln::Image<I>& input_)
{
  const I& input = mln::exact(input_);
  mln_concrete(I) output;
  mln::initialize(output, input);
  // Iterator on vertices.
  mln::p_n_faces_fwd_piter<I::dim, mln_geom(I)> v(input.domain(), 0);
  // Vertex-to-edges neighborhood.
  typedef mln::complex_higher_neighborhood<I::dim, mln_geom(I)> v2e_t;
  const v2e_t v2e;
  mln_niter(v2e_t) e(v2e, v);
  for_all(v)
  {
    output(v) = false;
    for_all(e)
      if (input(e))
	{
	  output(v) = true;
	  break;
	}
  }
  return output;
}

/// Erosion from vertices to edges (\f$\epsilon^\times\f$).
template <typename I>
inline
mln_concrete(I)
erosion_v2e(const mln::Image<I>& input_)
{
  const I& input = mln::exact(input_);
  mln_concrete(I) output;
  mln::initialize(output, input);
  // Iterator on edges.
  mln::p_n_faces_fwd_piter<I::dim, mln_geom(I)> e(input.domain(), 1);
  // Edge-to-vertices neighborhood.
  typedef mln::complex_lower_neighborhood<I::dim, mln_geom(I)> e2v_t;
  const e2v_t e2v;
  mln_niter(e2v_t) v(e2v, e);
  for_all(e)
  {
    output(e) = true;
    for_all(v)
      if (!input(v))
	{
	  output(e) = false;
	  break;
	}
  }
  return output;
}

/// Erosion from edges to vertices (\f$\epsilon^\bullet\f$).
template <typename I>
inline
mln_concrete(I)
erosion_e2v(const mln::Image<I>& input_)
{
  const I& input = mln::exact(input_);
  mln_concrete(I) output;
  mln::initialize(output, input);
  // Iterator on vertices.
  mln::p_n_faces_fwd_piter<I::dim, mln_geom(I)> v(input.domain(), 0);
  // Vertex-to-edges neighborhood.
  typedef mln::complex_higher_neighborhood<I::dim, mln_geom(I)> v2e_t;
  const v2e_t v2e;
  mln_niter(v2e_t) e(v2e, v);
  for_all(v)
  {
    output(v) = true;
    for_all(e)
      if (!input(e))
	{
	  output(v) = false;
	  break;
	}
  }
  return output;
}

/// Dilation from vertices to edges (\f$\delta^\times\f$).
template <typename I>
inline
mln_concrete(I)
dilation_v2e(const mln::Image<I>& input_)
{
  const I& input = mln::exact(input_);
  mln_concrete(I) output;
  mln::initialize(output, input);
  // Iterator on edges.
  mln::p_n_faces_fwd_piter<I::dim, mln_geom(I)> e(input.domain(), 1);
  // Edge-to-vertices neighborhood.
  typedef mln::complex_lower_neighborhood<I::dim, mln_geom(I)> e2v_t;
  const e2v_t e2v;
  mln_niter(e2v_t) v(e2v, e);
  for_all(e)
  {
    output(e) = false;
    for_all(v)
      if (input(v))
	{
	  output(e) = true;
	  break;
	}
  }
  return output;
}


/// Vertex dilation (\f$delta\f$).
template <typename I>
inline
mln_concrete(I)
dilation_vertex(const mln::Image<I>& input)
{
  return dilation_e2v(dilation_v2e(input));
}

/// Vertex erosion (\f$epsilon\f$).
template <typename I>
inline
mln_concrete(I)
erosion_vertex(const mln::Image<I>& input)
{
  return erosion_e2v(erosion_v2e(input));
}


/// Edge dilation (\f$Delta\f$).
template <typename I>
inline
mln_concrete(I)
dilation_edge(const mln::Image<I>& input)
{
  return dilation_v2e(dilation_e2v(input));
}

/// Edge erosion (\f$Epsilon\f$).
template <typename I>
inline
mln_concrete(I)
erosion_edge(const mln::Image<I>& input)
{
  return erosion_v2e(erosion_e2v(input));
}


/// Graph dilation (\f$delta \ovee Delta\f$).
template <typename I>
inline
mln_concrete(I)
dilation_graph(const mln::Image<I>& input)
{
  return combine(dilation_vertex(input), dilation_edge(input));
}

/// Graph erosion (\f$epsilon \ovee Epsilon\f$).
template <typename I>
inline
mln_concrete(I)
erosion_graph(const mln::Image<I>& input)
{
  return combine(erosion_vertex(input), erosion_edge(input));
}


/*-------------------------.
| Additional adjunctions.  |
`-------------------------*/

template <typename I>
inline
mln_concrete(I)
alpha1(const mln::Image<I>& input)
{
  mln_concrete(I) vertices;
  mln::initialize(vertices, input);
  mln::data::fill(vertices, true);
  return combine(vertices, input);
}

template <typename I>
inline
mln_concrete(I)
beta1(const mln::Image<I>& input)
{
  return combine(dilation_e2v(input), input);
}

template <typename I>
inline
mln_concrete(I)
alpha2(const mln::Image<I>& input)
{
  return combine(input, erosion_v2e(input));
}

template <typename I>
inline
mln_concrete(I)
beta2(const mln::Image<I>& input)
{
  mln_concrete(I) edges;
  mln::initialize(edges, input);
  mln::data::fill(edges, false);
  return combine(input, edges);
}

template <typename I>
inline
mln_concrete(I)
alpha3(const mln::Image<I>& input)
{
  return combine(erosion_e2v(input), erosion_v2e(erosion_e2v(input)));
}

template <typename I>
inline
mln_concrete(I)
beta3(const mln::Image<I>& input)
{
  return combine(dilation_e2v(dilation_v2e(input)), dilation_v2e(input));
}


/*------------------------.
| Openings and closings.  |
`------------------------*/

/// Vertex opening (\f$\gamma_1\f$).
template <typename I>
inline
mln_concrete(I)
opening_vertex(const mln::Image<I>& input)
{
  return dilation_vertex(erosion_vertex(input));
}

/// Vertex closing (\f$\phi_1\f$).
template <typename I>
inline
mln_concrete(I)
closing_vertex(const mln::Image<I>& input)
{
  return erosion_vertex(dilation_vertex(input));
}


/// Edge opening (\f$\Gamma_1\f$).
template <typename I>
inline
mln_concrete(I)
opening_edge(const mln::Image<I>& input)
{
  return dilation_edge(erosion_edge(input));
}

/// Edge closing (\f$\Phi_1\f$).
template <typename I>
inline
mln_concrete(I)
closing_edge(const mln::Image<I>& input)
{
  return erosion_edge(dilation_edge(input));
}


/// Graph opening (\f${\gamma \ovee \Gamma}_1\f$).
template <typename I>
inline
mln_concrete(I)
opening_graph(const mln::Image<I>& input)
{
  return combine(opening_vertex(input), opening_edge(input));
}

/// Graph closing (\f${\phi \ovee \Phi}_1\f$).
template <typename I>
inline
mln_concrete(I)
closing_graph(const mln::Image<I>& input)
{
  return combine(closing_vertex(input), closing_edge(input));
}


/*----------------------------------.
| Half-openings and half-closings.  |
`----------------------------------*/

/// Vertex half-opening (\f$\gamma_{1/2}\f$).
template <typename I>
inline
mln_concrete(I)
half_opening_vertex(const mln::Image<I>& input)
{
  return dilation_e2v(erosion_v2e(input));
}

/// Vertex half-closing (\f$\phi_{1/2}\f$).
template <typename I>
inline
mln_concrete(I)
half_closing_vertex(const mln::Image<I>& input)
{
  return erosion_e2v(dilation_v2e(input));
}


/// Edge half-opening (\f$\Gamma_{1/2}\f$).
template <typename I>
inline
mln_concrete(I)
half_opening_edge(const mln::Image<I>& input)
{
  return dilation_v2e(erosion_e2v(input));
}

/// Edge half-closing (\f$\Phi_{1/2}\f$).
template <typename I>
inline
mln_concrete(I)
half_closing_edge(const mln::Image<I>& input)
{
  return erosion_v2e(dilation_e2v(input));
}


/// Graph half-opening (\f${\gamma \ovee \Gamma}_{1/2}\f$).
template <typename I>
inline
mln_concrete(I)
half_opening_graph(const mln::Image<I>& input)
{
  return combine(half_opening_vertex(input), half_opening_edge(input));
}

/// Graph half-closing (\f${\phi \ovee \Phi}_{1/2}\f$).
template <typename I>
inline
mln_concrete(I)
half_closing_graph(const mln::Image<I>& input)
{
  return combine(half_closing_vertex(input), half_closing_edge(input));
}


/*-------------------------------------------------------.
| Parameterized openings and closings (granulometries).  |
`-------------------------------------------------------*/

/// Opening (\f${\gamma \ovee \Gamma}_{\lambda/2}\f$).
template <typename I>
inline
mln_concrete(I)
opening(const mln::Image<I>& input, unsigned lambda)
{
  unsigned i = lambda / 2;
  unsigned j = lambda % 2;
  mln_concrete(I) output = mln::duplicate(input);
  for (unsigned m = 0; m < i; ++m)
    output = erosion_graph(output);
  for (unsigned m = 0; m < j; ++m)
    output = half_opening_graph(output);
  for (unsigned m = 0; m < i; ++m)
    output = dilation_graph(output);
  return output;
}

/// Opening (\f${\phi \ovee \Phi}_{\lambda/2}\f$).
template <typename I>
inline
mln_concrete(I)
closing(const mln::Image<I>& input, unsigned lambda)
{
  unsigned i = lambda / 2;
  unsigned j = lambda % 2;
  mln_concrete(I) output = mln::duplicate(input);
  for (unsigned m = 0; m < i; ++m)
    output = dilation_graph(output);
  for (unsigned m = 0; m < j; ++m)
    output = half_closing_graph(output);
  for (unsigned m = 0; m < i; ++m)
    output = erosion_graph(output);
  return output;
}

/*-------------------------------.
| Alternate Sequential Filters.  |
`-------------------------------*/

/// Alternate Sequential Filter (ASF) (\f${ASF}_{\lambda/2}\f$).
template <typename I>
inline
mln_concrete(I)
asf(const mln::Image<I>& input, unsigned lambda)
{
  mln_concrete(I) output = mln::duplicate(input);
  for (unsigned m = 1; m <= lambda; ++m)
    output = opening(closing(output, m), m);
  return output;
}

#endif // ! APPS_GRAPH_MORPHO_MORPHO_HH
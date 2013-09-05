// Copyright (C) 2011 EPITA Research and Development Laboratory (LRDE)
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

/// \file
/// \brief A program computing a binary skeleton of a 3D image,
/// preserving 1D isthmuses.

#include <fstream>

#include <mln/core/image/image3d.hh>
#include <mln/core/alias/neighb3d.hh>
#include <mln/make/box3d.hh>

#include <mln/core/image/vmorph/fun_image.hh>
#include <mln/fun/v2b/lnot.hh>

#include <mln/border/fill.hh>

#include <mln/labeling/blobs.hh>

#include <mln/topo/skeleton/breadth_first_thinning.hh>

#include <mln/topo/is_simple_point3d.hh>
#include <mln/topo/detach_point.hh>
#include <mln/topo/is_not_1d_isthmus.hh>

#include "apps/data.hh"

#include "image3d-skel.hh"


// FIXME: Have this program take a 3D image as input (instead of
// hard-coding it here).

int main()
{
  using namespace mln;

  typedef image3d<bool> I;
  typedef neighb3d N;

  border::thickness = 1;

//   I whole_input = load_pgm_3d(MLN_IMG_DIR "/bunny.pgm");
//   // FIXME: Work on a small image in a first time, then process the
//   // whole image.
//   I input = subsampling_3d(whole_input, 4);

  // Input image.
  //
  // Use a smaller half-length than the default one (100) as this
  // program does not use the fast version of the simple 3D point
  // criterion.
  I input = make_triple_torus(20);

  std::cerr << input.domain() << std::endl;
  save_raw_3d(input, "image3d-skel-with-1d-isthmuses-input.raw");
  save_vtk_polyhedrons(input, "image3d-skel-with-1d-isthmuses-input.vtk");

  // FIXME: Debug.
  unsigned n_fg_comps;
  labeling::blobs(input, c26(), n_fg_comps);
  unsigned n_bg_comps;
  labeling::blobs((fun::v2b::lnot<bool>() << input), c6(), n_bg_comps);
  std::cerr << "n_fg_comps = " << n_fg_comps << std::endl;
  std::cerr << "n_bg_comps = " << n_bg_comps << std::endl;

  // FIXME: Use a dual neighborhood instead?

  // Foreground neighborhood.
  N nbh_fg = c26();
  // Background neighborhood.
  N nbh_bg = c6();

  // Simplicity criterion functor.
  topo::is_simple_point3d<I, N> is_simple(nbh_fg, nbh_bg);
  // Simple point detach procedure.
  topo::detach_point<I> detach;
  // Constraint: do not remove 1D isthmuses.
  /* FIXME: This criterion is static, e.g. it is attached to INPUT
     (fixed), not OUTPUT (changing during the thinning).  This is not
     as good as a dynamic critertion (attached to OUTPUT).  Maybe it
     is time to introduce/use `breadth_first_thinning_inplace'.  */
  topo::is_not_1d_isthmus<I, N> constraint(c6(), c26(), input);

  I output = topo::skeleton::breadth_first_thinning(input, nbh_fg,
						    is_simple,
						    detach,
						    constraint);
  save_raw_3d(output, "image3d-skel-with-1d-isthmuses-skel.raw");
  save_vtk_polyhedrons(output, "image3d-skel-with-1d-isthmuses-skel.vtk");
}

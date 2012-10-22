// Copyright (C) 2012 EPITA Research and Development Laboratory (LRDE)
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

#include <mln/core/image/image2d.hh>
#include <mln/world/k1/immerse.hh>
#include <mln/data/compare.hh>

int main()
{
  using namespace mln;

  int ivals[][2] = {
    {1, 2},
    {3, 4}
  };
  image2d<int> ima = make::image(ivals);

  int vals[][5] = {
    {0, 0, 0, 0, 0},
    {0, 1, 0, 2, 0},
    {0, 0, 0, 0, 0},
    {0, 3, 0, 4, 0},
    {0, 0, 0, 0, 0}
  };
  image2d<int> ref = make::image(vals, point2d(-1,-1));

  int fvals[][5] = {
    {5, 5, 5, 5, 5},
    {5, 1, 5, 2, 5},
    {5, 5, 5, 5, 5},
    {5, 3, 5, 4, 5},
    {5, 5, 5, 5, 5}
  };
  image2d<int> ref_fill = make::image(fvals, point2d(-1,-1));

  {
    image2d<int> immersed = world::k1::immerse(ima);
    mln_assertion(immersed.domain() == ref_fill.domain());
    mln_assertion(immersed(point2d(0,0)) == 1);
    mln_assertion(immersed(point2d(0,2)) == 2);
    mln_assertion(immersed(point2d(2,0)) == 3);
    mln_assertion(immersed(point2d(2,2)) == 4);
  }

  {
    image2d<long> immersed = world::k1::immerse(ima, long());
    mln_assertion(immersed.domain() == ref_fill.domain());
    mln_assertion(immersed(point2d(0,0)) == 1);
    mln_assertion(immersed(point2d(0,2)) == 2);
    mln_assertion(immersed(point2d(2,0)) == 3);
    mln_assertion(immersed(point2d(2,2)) == 4);
  }

  {
    image2d<long> immersed = world::k1::immerse(ima, long(), 5l);
    mln_assertion(immersed == ref_fill);
  }

}

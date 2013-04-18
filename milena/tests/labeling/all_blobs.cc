// Copyright (C) 2007, 2008, 2009, 2013 EPITA Research and Development
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

#include <mln/core/image/image2d.hh>
#include <mln/io/pbm/load.hh>
#include <mln/io/pgm/load.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/labeling/all_blobs.hh>
#include <mln/value/int_u8.hh>

#include <mln/labeling/colorize.hh>
#include <mln/io/ppm/save.hh>
#include <mln/value/rgb8.hh>
#include <mln/debug/println.hh>

#include "tests/data.hh"


int main()
{
  using namespace mln;

  // Binary image
  {
    image2d<bool> pic;
    io::pbm::load(pic, MLN_IMG_DIR "/picasso.pbm");
    unsigned n;
    labeling::all_blobs(pic, c4(), n);
    mln_assertion(n == 142);
  }

  // Gray-level image
  {
    image2d<value::int_u8> pic;
    io::pgm::load(pic, MLN_IMG_DIR "/fly.pgm");
    unsigned n;
    labeling::all_blobs(pic, c4(), n);
    mln_assertion(n == 24);
  }
}
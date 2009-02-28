// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

/// \file tests/morpho/opening/algebraic.cc
///
/// Test on mln::morpho::opening::algebraic.

#include <mln/core/image/image2d.hh>
#include <mln/value/int_u8.hh>
#include <mln/core/alias/neighb2d.hh>

#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>

#include <mln/morpho/opening/algebraic.hh>
#include <mln/morpho/attribute/card.hh>
#include <mln/accu/count.hh>

#include "tests/data.hh"



int main()
{
  using namespace mln;
  using value::int_u8;

  trace::quiet = false;

  typedef image2d<int_u8> I;
  I lena;
  io::pgm::load(lena, MLN_IMG_DIR "/tiny.pgm");
  {
    accu::count<point2d> a;
    io::pgm::save(morpho::opening::algebraic(lena, c4(), a, 10),
		  "ref.pgm");
  }
  {
    morpho::attribute::card<I> a;
    io::pgm::save(morpho::opening::algebraic(lena, c4(), a, 10),
		  "out.pgm");
  }
}

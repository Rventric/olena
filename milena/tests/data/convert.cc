// Copyright (C) 2008, 2009, 2013 EPITA Research and Development
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
#include <mln/data/convert.hh>
#include <mln/data/compare.hh>

#include <mln/value/rgb8.hh>
#include <mln/literal/grays.hh>
#include <mln/opt/at.hh>


int main()
{
  using namespace mln;
  using value::rgb8;

  // bool -> rgb8
  {
    image2d<bool> ima(1, 2);
    opt::at(ima, 0, 0) = false;
    opt::at(ima, 0, 1) = true;
    image2d<rgb8> out = data::convert(ima, rgb8());
    mln_assertion(opt::at(out, 0, 0) == literal::black);
    mln_assertion(opt::at(out, 0, 1) == literal::white);
  }
  // bool -> bool
  {
    image2d<bool> ima(1, 2);
    data::fill(ima, true);
    opt::at(ima, 0, 0) = false;
    image2d<bool> out = data::convert(ima, bool());
    mln_assertion(out == ima);
    mln_assertion(out.buffer() != ima.buffer());
  }

  // bool -> rgb8
  {
    image2d<bool> ima(1, 2);
    opt::at(ima, 0, 0) = false;
    opt::at(ima, 0, 1) = true;
    fun::v2v::convert<value::rgb8> f;
    image2d<rgb8> out = data::convert(ima, rgb8(), f);
    mln_assertion(opt::at(out, 0, 0) == literal::black);
    mln_assertion(opt::at(out, 0, 1) == literal::white);
  }
  // bool -> bool
  {
    image2d<bool> ima(1, 2);
    data::fill(ima, true);
    opt::at(ima, 0, 0) = false;
    fun::v2v::convert<bool> f;
    image2d<bool> out = data::convert(ima, bool(), f);
    mln_assertion(out == ima);
    mln_assertion(out.buffer() != ima.buffer());
  }


}

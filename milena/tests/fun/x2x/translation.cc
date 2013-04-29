// Copyright (C) 2007, 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

#include <cmath>
#include <iostream>
#include <mln/fun/x2x/translation.hh>
#include <mln/fun/v2v/all_to.hh>



int main()
{
  using namespace mln;

  float
    a = 2.3,
    b = 0,
    c = 2.9;

  algebra::vec<3,float> vec1 = make::vec(a, b, c);
  fun::x2x::translation<3,float> tr1(all_to(1.6));

  std::cout << vec1 << std::endl;
  std::cout << tr1(vec1) << std::endl;
  std::cout << tr1.inv()(vec1) << std::endl;
  mln_assertion(tr1(tr1.inv()(vec1)) == vec1);
  mln_assertion(fabs(tr1(vec1)[0] - 3.9) <= 0.125 &&
		fabs(tr1(vec1)[1] - 1.6) <= 0.125 &&
		fabs(tr1(vec1)[2] - 4.5) <= 0.125);
}

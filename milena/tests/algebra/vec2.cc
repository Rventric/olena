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

#include <iostream>
#include <mln/fun/v2v/all_to.hh>
#include <mln/algebra/vec.hh>
#include <mln/algebra/h_vec.hh>



int main()
{
  using namespace mln;

  algebra::vec<1,float> v1(all_to(4.));
  algebra::vec<2,float> v2 = make::vec(6., 2.8);

  algebra::h_vec<1,float> hv1;
  algebra::h_vec<2,float> hv2 = v2.to_h_vec(); // Immersion into homogeneous.

  algebra::h_vec<3,float> hv3(all_to(1.5));

  hv3 += make::vec(0., 0., 0., 0.5);

  algebra::vec<3,float> v3 = hv3.to_vec(); // Back from homogeneous.
  algebra::vec<4,float> v4 = hv3;

  std::cout << "v1 = " << v1 << ";" << std::endl;
  std::cout << "v2 = " << v2 << ";" << std::endl;
  std::cout << "v3 = " << v3 << ";" << std::endl;
  std::cout << "v4 = " << v4 << ";" << std::endl;
  std::cout << "hv1 = " << hv1 << ";" << std::endl;
  std::cout << "hv2 = " << hv2 << ";" << std::endl;
  std::cout << "hv3 = " << hv3 << ";" << std::endl;
}

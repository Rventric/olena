// Copyright (C) 2002, 2003  EPITA Research and Development Laboratory
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

#include <ntg/int.hh>
#include <oln/basics2d.hh>
#include <oln/level/set_level.hh>
#include <oln/core/box.hh>
#include <iostream>
#include "check.hh"
#include "data.hh"

using namespace oln;
using namespace mlc;
using namespace ntg;

#define OK_OR_FAIL				\
      std::cout << "OK" << std::endl;		\
    else					\
      {						\
	std::cout << "FAIL" << std::endl;	\
	fail = true;				\
      }

#define ASSERT_CHECK(Val) { assert_check(Val, #Val, fail); }

void assert_check(bool res, const std::string& desc, bool &fail)
{
  std::cout << "---- " << desc << " ----" << std::endl;
  if (res)
    OK_OR_FAIL;
}


bool
check(void)
{
  bool fail = false;

  image2d<int_u8>	im(256, 256);
  point2d
    p1(0,0),
    p2(0, 255),
    p3(255, 255),
    p4(255, 0);

  std::cout<<"---- level::set_level (by points)  ----"<<std::endl;

  //draw lines and check some trivial points
  level::set_level(im, p1, p2, 1);
  level::set_level(im, p2, p3, 2);
  level::set_level(im, p3, p4, 3);
  level::set_level(im, p4, p1, 4);
  level::set_level(im, p1, p3, 5);
  level::set_level(im, p2, p4, 6);

  ASSERT_CHECK(im(0, 128) == 1);
  ASSERT_CHECK(im(128, 255) == 2);
  ASSERT_CHECK(im(255, 128) == 3);
  ASSERT_CHECK(im(128, 0) == 4);

  save(im, "set_level");

  std::cout<<"---- level::set_level (by boxes) ----"<<std::endl;
  box<point2d> b;
  b.add(p1);
  b.add(p2);
  b.add(p3);
  b.add(p4);
  image2d<int_u8>	im2(256, 256);
  level::set_level(im2, b, 128);
  save(im2, "set_level2");
  ASSERT_CHECK(im2(0, 128) == 128);

  return fail;
}

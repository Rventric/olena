#include <ntg/int.hh>
#include <oln/basics2d.hh>
#include <oln/utils/stat.hh>

#include <iostream>

#include "check.hh"
#include "data.hh"

using namespace oln;
using namespace ntg;
using namespace utils;

#define OK_OR_FAIL				\
      std::cout << "OK" << std::endl;		\
    else					\
      {						\
	std::cout << "FAIL" << std::endl;	\
	fail = true;				\
      }

bool
check()
{
  bool fail = false;

  image2d<int_u8> src = load(rdata("lena256.pgm"));

  f_minmax<int_u8> minmax;

  traverse(minmax, src);

  if (minmax.min() == 27 && minmax.max() == 237)
    OK_OR_FAIL;

  f_moments<int_u8> moments;

  traverse(moments, src);

  if (fabs(moments.mean() - 123.32) <= 1e-2 &&
      fabs(moments.variance() - 2241.57) <= 1e-2)
    OK_OR_FAIL;

  return fail;
}

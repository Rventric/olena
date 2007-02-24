#include <iostream>
#include "grid2.hh"

namespace glg
{
  typedef box_iterator_<point2d> box_iterator2d;
  typedef box_iterator_<point1d> box_iterator1d;
}

int main ()
{
  using namespace glg;
  point2d p1(0, 0), p2(3, 3), p3(5, 4);
  point1d pp1(2), pp2(7);
  box2d   b(p1, p2), b2(p2, p3);
  box1d   c(pp1, pp2);
  box_iterator2d bb(b);
  box_iterator1d bc(c);
  pset_std_<point2d> tst;
  image2d<int> aie(b), pwet(b2);
  image2d<int> cocopy(aie);;
  image2d<int>::iter it(aie.bbox());
  signal<int> ouille(c);

  std::cout << "[32mParcours simple image2d[m" << std::endl;

  print(aie);

  std::cout << "[35m~~~[m" << std::endl << std::endl;

  std::cout << "[32mParcours simple signal[m" << std::endl;

  print(ouille);

  std::cout << "[35m~~~[m" << std::endl << std::endl;

  int i = 1;
  for (it.start(); it.is_valid(); it.next(), i++)
    aie(it) += i;

  std::cout << "[32mIncrementation des valeurs de image2d[m" << std::endl;

  print(aie);

  std::cout << "[35m~~~[m" << std::endl << std::endl;

  // dummy instanciations

  std::cout << "[32mInstanciation de variables de divers types[m" << std::endl;

  glg::singleton<int> sing;
  glg::vec<3, int> vec; // voir avec Theo le comportement louche de g++
			// reponse : g++ c'est gland!

  std::cout << "If you can see this message, instanciations worked!" << std::endl;

  std::cout << "[35m~~~[m" << std::endl << std::endl;

  std::cout << "[32mVec fullfillment + display[m" << std::endl;
  vec[0] = 0;
  vec[1] = 5;
  vec[2] = 1;

  std::cout << vec;

  std::cout << "[35m~~~[m" << std::endl << std::endl;


  std::cout << "[32mPolite image[m" << std::endl;

  polite_image< image2d<int> > drucker(aie);
  drucker.talk();

  std::cout << std::endl << "[35m~~~[m" << std::endl << std::endl;


  return bidon (p1, p2);
}

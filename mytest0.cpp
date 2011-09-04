#include <iostream>
#include "SMatrix.h"

int main(void) {
  SMatrix m(5, 7);

  int i, j, v;

  m.setVal(2, 6, 5);
  std::cout << std::endl << m << std::endl;
  m.setVal(2, 0, 3);
  std::cout << std::endl << m << std::endl;
  m.setVal(2, 3, 4);
  std::cout << std::endl << m << std::endl;
  m.setVal(0, 3, 1);
  std::cout << std::endl << m << std::endl;
  m.setVal(4, 1, 2);
  std::cout << std::endl << m << std::endl;
  m.setVal(4, 3, 6);
  std::cout << std::endl << m << std::endl;
  //////:
  m.setVal(2, 1, 6);
  std::cout << std::endl << m << std::endl;
  //////:
  m.setVal(2, 3, 0);
  std::cout << std::endl << m << std::endl;
  m.setVal(2, 1, 0);
  std::cout << std::endl << m << std::endl;

  i = 4; j = 3; v = 0;
  m.setVal(i, j, v);
  std::cout << " i=" << i << " j=" << j << " v=" << v << std::endl << m << std::endl;

  i = 2; j = 0; v = 0;
  m.setVal(i, j, v);
  std::cout << " i=" << i << " j=" << j << " v=" << v << std::endl << m << std::endl;

  i = 0; j = 3; v = 0;
  std::cout << ">>>>>>>>>>> i=" << i << " j=" << j << " v=" << v << std::endl;
  m.setVal(i, j, v);
  std::cout << m << std::endl;
  m.checkIntegrity();


  i = 2; j = 6; v = 0;
  std::cout << ">>>>>>>>>>> i=" << i << " j=" << j << " v=" << v << std::endl;
  m.setVal(i, j, v);
  std::cout << m << std::endl;
  m.checkIntegrity();

  i = 2; j = 3; v = 0;
  std::cout << ">>>>>>>>>>> i=" << i << " j=" << j << " v=" << v << std::endl;
  m.setVal(i, j, v);
  std::cout << m << std::endl;
  std::cout << m.ridx_.size() << std::endl;
  m.checkIntegrity();

  i = 4; j = 1; v = 0;
  std::cout << ">>>>>>>>>>> i=" << i << " j=" << j << " v=" << v << std::endl;
  m.setVal(i, j, v);
  std::cout << m << std::endl;
  std::cout << m.ridx_.size() << std::endl;
  m.checkIntegrity();

  //SMatrix::size_type count = 1;
  //for (m.begin(); !m.end(); m.next()) {
    //std::cout << m.value() << " ";
    //if (count % m.cols() == 0)
      //std::cout << std::endl;
    //++count;
  //}

  //std::cout << std::endl << m << std::endl;

  return 0;
}

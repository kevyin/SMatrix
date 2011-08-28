#include <iostream>
#include "SMatrix.h"

int main(void) {
  SMatrix m(5, 7);

  m.setVal(2, 6, 5);
  m.setVal(2, 0, 3);
  m.setVal(2, 3, 4);
  m.setVal(0, 3, 1);
  m.setVal(4, 1, 2);
  m.setVal(4, 3, 6);

  SMatrix::size_type count = 1;
  for (m.begin(); !m.end(); m.next()) {
    std::cout << m.value() << " ";
    if (count % m.cols() == 0)
      std::cout << std::endl;
    ++count;
  }

  std::cout << std::endl << m << std::endl;

  return 0;
}

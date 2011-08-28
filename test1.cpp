#include <iostream>
#include <string>
#include "SMatrix.h"

int main() {
  SMatrix m(4, 3);

  try {
    m(10, 10);
  } catch (const MatrixError &e) {
    std::string err(e.what());
    if (err.substr(0, 18) == "Matrix bound error")
      std::cout << "Test passed!" << std::endl;
    else
      std::cout << "Test failed!" << std::endl;
  } catch (...) { }
  
  return 0;
}

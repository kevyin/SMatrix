#include "SMatrix.h"
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE const_string test
#include <boost/test/unit_test.hpp>

template <typename T>
void array_print(T array[], size_t array_len) {
    for (size_t k = 0; k < array_len; k++) {
        std::cout << array[k] << ' ';
    }
    std::cout << std::endl;
}

BOOST_AUTO_TEST_CASE( setVals ) {
  int i, j, v;
  SMatrix m(5, 7);
 
  m.checkIntegrity();

  BOOST_CHECK_EQUAL(m.vals_capacity_, 7);
  BOOST_CHECK_EQUAL(m.cidx_capacity_, 7);

  BOOST_CHECK_EQUAL(m.rows(), static_cast<SMatrix::size_type>(5));
  BOOST_CHECK_EQUAL(m.cols(), static_cast<SMatrix::size_type>(7));

  
  BOOST_CHECK_EQUAL(m.vals_size_, 0);
  BOOST_CHECK_EQUAL(m.cidx_size_, 0);

  m.setVal(2, 6, 5);
  m.checkIntegrity();
  BOOST_CHECK_EQUAL(m.vals_[0] , 5);
  BOOST_CHECK_EQUAL(m.cidx_[0] , 6);
  BOOST_CHECK_EQUAL(m.vals_size_, 1);
  BOOST_CHECK_EQUAL(m.cidx_size_, 1);
  

  m.setVal(2, 0, 3);
  m.checkIntegrity();
  BOOST_CHECK_EQUAL(m.vals_[0] , 3);
  BOOST_CHECK_EQUAL(m.cidx_[0] , 0);
  BOOST_CHECK_EQUAL(m.vals_[1] , 5);
  BOOST_CHECK_EQUAL(m.cidx_[1] , 6);
  BOOST_CHECK_EQUAL(m.vals_size_, 2);
  BOOST_CHECK_EQUAL(m.cidx_size_, 2);

  m.setVal(2, 3, 4);
  m.checkIntegrity();
  BOOST_CHECK_EQUAL(m.vals_[0] , 3);
  BOOST_CHECK_EQUAL(m.cidx_[0] , 0);
  BOOST_CHECK_EQUAL(m.vals_[1] , 4);
  BOOST_CHECK_EQUAL(m.cidx_[1] , 3);
  BOOST_CHECK_EQUAL(m.vals_[2] , 5);
  BOOST_CHECK_EQUAL(m.cidx_[2] , 6);
  BOOST_CHECK_EQUAL(m.vals_size_, 3);
  BOOST_CHECK_EQUAL(m.cidx_size_, 3);

  m.setVal(0, 3, 1);
  m.checkIntegrity();
  BOOST_CHECK_EQUAL(m.vals_[0] , 3);
  BOOST_CHECK_EQUAL(m.cidx_[0] , 0);
  BOOST_CHECK_EQUAL(m.vals_[1] , 4);
  BOOST_CHECK_EQUAL(m.cidx_[1] , 3);
  BOOST_CHECK_EQUAL(m.vals_[2] , 5);
  BOOST_CHECK_EQUAL(m.cidx_[2] , 6);
  BOOST_CHECK_EQUAL(m.vals_[3] , 1);
  BOOST_CHECK_EQUAL(m.cidx_[3] , 3);
  BOOST_CHECK_EQUAL(m.vals_size_, 4);
  BOOST_CHECK_EQUAL(m.cidx_size_, 4);

  m.setVal(4, 1, 2);
  m.checkIntegrity();
  BOOST_CHECK_EQUAL(m.vals_[0] , 3);
  BOOST_CHECK_EQUAL(m.cidx_[0] , 0);
  BOOST_CHECK_EQUAL(m.vals_[1] , 4);
  BOOST_CHECK_EQUAL(m.cidx_[1] , 3);
  BOOST_CHECK_EQUAL(m.vals_[2] , 5);
  BOOST_CHECK_EQUAL(m.cidx_[2] , 6);
  BOOST_CHECK_EQUAL(m.vals_[3] , 1);
  BOOST_CHECK_EQUAL(m.cidx_[3] , 3);
  BOOST_CHECK_EQUAL(m.vals_[4] , 2);
  BOOST_CHECK_EQUAL(m.cidx_[4] , 1);
  BOOST_CHECK_EQUAL(m.vals_size_, 5);
  BOOST_CHECK_EQUAL(m.cidx_size_, 5);

  m.setVal(4, 3, 6);
  m.checkIntegrity();
  BOOST_CHECK_EQUAL(m.vals_[0] , 3);
  BOOST_CHECK_EQUAL(m.cidx_[0] , 0);
  BOOST_CHECK_EQUAL(m.vals_[1] , 4);
  BOOST_CHECK_EQUAL(m.cidx_[1] , 3);
  BOOST_CHECK_EQUAL(m.vals_[2] , 5);
  BOOST_CHECK_EQUAL(m.cidx_[2] , 6);
  BOOST_CHECK_EQUAL(m.vals_[3] , 1);
  BOOST_CHECK_EQUAL(m.cidx_[3] , 3);
  BOOST_CHECK_EQUAL(m.vals_[4] , 2);
  BOOST_CHECK_EQUAL(m.cidx_[4] , 1);
  BOOST_CHECK_EQUAL(m.vals_[5] , 6);
  BOOST_CHECK_EQUAL(m.cidx_[5] , 3);
  BOOST_CHECK_EQUAL(m.vals_size_, 6);
  BOOST_CHECK_EQUAL(m.cidx_size_, 6);

  m.checkIntegrity();
  i = 4; j = 4; v = 7;
  //std::cout << ">>>>>>>>>>> i=" << i << " j=" << j << " v=" << v << std::endl;
  m.setVal(i, j, v);
  std::cout << "size " << m.vals_size_ << std::endl;
  std::cout << "capacity " << m.vals_capacity_ << std::endl;
  std::cout << m << std::endl;

  m.checkIntegrity();
  i = 1; j = 2; v = 2;
  //std::cout << ">>>>>>>>>>> i=" << i << " j=" << j << " v=" << v << std::endl;
  m.setVal(i, j, v);
  std::cout << m << std::endl;

  m.checkIntegrity();
  i = 4; j = 1; v = 9;
  //std::cout << ">>>>>>>>>>> i=" << i << " j=" << j << " v=" << v << std::endl;
  m.setVal(i, j, v);
  std::cout << m << std::endl;

  m.checkIntegrity();
  i = 4; j = 2; v = 9;
  //std::cout << ">>>>>>>>>>> i=" << i << " j=" << j << " v=" << v << std::endl;
  m.setVal(i, j, v);
  std::cout << m << std::endl;

  m.checkIntegrity();
  i = 4; j = 1; v = 0;
  //std::cout << ">>>>>>>>>>> i=" << i << " j=" << j << " v=" << v << std::endl;
  m.setVal(i, j, v);
  std::cout << m << std::endl;

  m.checkIntegrity();
  i = 0; j = 1; v = 9;
  //std::cout << ">>>>>>>>>>> i=" << i << " j=" << j << " v=" << v << std::endl;
  m.setVal(i, j, v);
  std::cout << m << std::endl;

  m.checkIntegrity();
  i = 3; j = 2; v = 9;
  //std::cout << ">>>>>>>>>>> i=" << i << " j=" << j << " v=" << v << std::endl;
  m.setVal(i, j, v);
  std::cout << m << std::endl;

  m.checkIntegrity();
  i = 4; j = 3; v = 7;
  //std::cout << ">>>>>>>>>>> i=" << i << " j=" << j << " v=" << v << std::endl;
  m.setVal(i, j, v);
  std::cout << m << std::endl;

  m.checkIntegrity();
  i = 0; j = 0; v = 1;
  //std::cout << ">>>>>>>>>>> i=" << i << " j=" << j << " v=" << v << std::endl;
  m.setVal(i, j, v);
  std::cout << m << std::endl;

  m.checkIntegrity();
  i = 0; j = 1; v = 1;
  //std::cout << ">>>>>>>>>>> i=" << i << " j=" << j << " v=" << v << std::endl;
  m.setVal(i, j, v);
  std::cout << m << std::endl;

  m.checkIntegrity();
  i = 0; j = 2; v = 1;
  //std::cout << ">>>>>>>>>>> i=" << i << " j=" << j << " v=" << v << std::endl;
  m.setVal(i, j, v);
  std::cout << m << std::endl;

  m.checkIntegrity();
  i = 0; j = 3; v = 1;
  //std::cout << ">>>>>>>>>>> i=" << i << " j=" << j << " v=" << v << std::endl;
  m.setVal(i, j, v);
  std::cout << m << std::endl;

  m.checkIntegrity();
  i = 0; j = 4; v = 1;
  //std::cout << ">>>>>>>>>>> i=" << i << " j=" << j << " v=" << v << std::endl;
  m.setVal(i, j, v);
  std::cout << m << std::endl;

  m.checkIntegrity();
  i = 0; j = 5; v = 1;
  //std::cout << ">>>>>>>>>>> i=" << i << " j=" << j << " v=" << v << std::endl;
  m.setVal(i, j, v);
  std::cout << m << std::endl;

  m.checkIntegrity();
  i = 0; j = 6; v = 1;
  //std::cout << ">>>>>>>>>>> i=" << i << " j=" << j << " v=" << v << std::endl;
  m.setVal(i, j, v);
  std::cout << m << std::endl;
}

BOOST_AUTO_TEST_CASE( delVals ) {
    std::cout << "delvals" << std::endl;

  SMatrix m(10, 7);
 
  m.checkIntegrity();

  BOOST_CHECK_EQUAL(m.vals_capacity_, 14);
  BOOST_CHECK_EQUAL(m.cidx_capacity_, 14);

  BOOST_CHECK_EQUAL(m.rows(), static_cast<SMatrix::size_type>(10));
  BOOST_CHECK_EQUAL(m.cols(), static_cast<SMatrix::size_type>(7));

  
  BOOST_CHECK_EQUAL(m.vals_size_, 0);
  BOOST_CHECK_EQUAL(m.cidx_size_, 0);

  m.setVal(2, 6, 5);
  m.checkIntegrity();
  BOOST_CHECK_EQUAL(m.vals_[0] , 5);
  BOOST_CHECK_EQUAL(m.cidx_[0] , 6);
  BOOST_CHECK_EQUAL(m.vals_size_, 1);
  BOOST_CHECK_EQUAL(m.cidx_size_, 1);
  

  m.setVal(2, 0, 3);
  m.checkIntegrity();
  BOOST_CHECK_EQUAL(m.vals_[0] , 3);
  BOOST_CHECK_EQUAL(m.cidx_[0] , 0);
  BOOST_CHECK_EQUAL(m.vals_[1] , 5);
  BOOST_CHECK_EQUAL(m.cidx_[1] , 6);
  BOOST_CHECK_EQUAL(m.vals_size_, 2);
  BOOST_CHECK_EQUAL(m.cidx_size_, 2);
  //std::cout << std::endl << m << std::endl;
  //array_print(m.vals_,4);
  //array_print(m.cidx_,4);

  m.setVal(2,0,0);
  m.checkIntegrity();
  BOOST_CHECK_EQUAL(m.vals_[0] , 5);
  BOOST_CHECK_EQUAL(m.cidx_[0] , 6);
  BOOST_CHECK_EQUAL(m.vals_size_, 1);
  BOOST_CHECK_EQUAL(m.cidx_size_, 1);
  //std::cout << std::endl << m << std::endl;
  //array_print(m.vals_,4);
  //array_print(m.cidx_,4);

  m.setVal(2, 0, 3);
  m.setVal(2, 6, 0);
  m.checkIntegrity();
  //array_print(m.vals_,4);
  //array_print(m.cidx_,4);
  BOOST_CHECK_EQUAL(m.vals_[0] , 3);
  BOOST_CHECK_EQUAL(m.cidx_[0] , 0);
  BOOST_CHECK_EQUAL(m.vals_size_, 1);
  BOOST_CHECK_EQUAL(m.cidx_size_, 1);

  m.setVal(8, 4, 4);
  m.checkIntegrity();
  m.setVal(8, 4, 2);
  m.checkIntegrity();
  m.setVal(5, 3, 8);
  m.checkIntegrity();
  m.setVal(7, 4, 2);
  m.setVal(7, 2, 2);
  m.setVal(3, 3, 8);
  m.setVal(8, 0, 2);
  m.setVal(8, 1, 2);
  m.setVal(8, 4, 4);
  m.setVal(1, 4, 0);
  m.checkIntegrity();
  std::cout << std::endl << m << std::endl;
  m.setVal(2, 4, 0);
  m.setVal(3, 3, 0);
  m.setVal(8, 0, 0);
  m.setVal(8, 1, 0);
  m.setVal(8, 4, 0);

  m.checkIntegrity();
  std::cout << std::endl << m << std::endl;
}

#include "SMatrix.h"
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE const_string test
#include <boost/test/unit_test.hpp>

#include <iostream>
#include <fstream>
using namespace std;

template <typename T>
void array_print(T array[], size_t array_len) {
    for (size_t k = 0; k < array_len; k++) {
        std::cout << array[k] << ' ';
    }
    std::cout << std::endl;
}

void prettyMatrix(SMatrix& m) {
    SMatrix::size_type count = 1;
    for (m.begin(); !m.end(); m.next()) {
        std::cout << m.value() << " ";
        if (count % m.cols() == 0)
          std::cout << std::endl;
        ++count;
    }
}

BOOST_AUTO_TEST_CASE( constructors ) {

    SMatrix m(7,14);
    std::cout << m << std::endl;

    m.checkIntegrity();

    BOOST_CHECK_EQUAL(m.vals_capacity_, 20);
    BOOST_CHECK_EQUAL(m.cidx_capacity_, 20);

    BOOST_CHECK_EQUAL(m.rows(), static_cast<SMatrix::size_type>(7));
    BOOST_CHECK_EQUAL(m.cols(), static_cast<SMatrix::size_type>(14));


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

    std::cout << m << std::endl;


    SMatrix m2(m);
    m.checkIntegrity();
    BOOST_CHECK_EQUAL(m.vals_[0] , 3);
    BOOST_CHECK_EQUAL(m.cidx_[0] , 0);
    BOOST_CHECK_EQUAL(m.vals_[1] , 5);
    BOOST_CHECK_EQUAL(m.cidx_[1] , 6);
    BOOST_CHECK_EQUAL(m.vals_size_, 2);
    BOOST_CHECK_EQUAL(m.cidx_size_, 2);
    std::cout << m2 << std::endl;

    m2.setVal(1,1,3);
    //m2.setVal(8,0,3);

    cout << "Testing 3rd constructor" << endl;
    ofstream myfile;
    myfile.open ("tmp.out", ios::out); 
    
    myfile << m2 << std::endl;
    myfile.close();

    filebuf fb;
    fb.open ("tmp.out", ios::in); 
    istream is(&fb);
    SMatrix m3(is);
    
    std::cout << m2 << std::endl;
    std::cout << m3 << std::endl;
    //myfile.close();
}

BOOST_AUTO_TEST_CASE( opEquality ) {

    cout << "operator==" << endl;

    SMatrix m(7,14);
    m.checkIntegrity();
    m.setVal(2, 6, 5);
    m.checkIntegrity();
    m.setVal(4, 0, 3);
    m.checkIntegrity();


    SMatrix m2(7,14);
    m2.checkIntegrity();
    m2.setVal(4, 0, 3);
    m2.checkIntegrity();
    m2.setVal(2, 6, 5);
    m2.checkIntegrity();

    BOOST_CHECK(m == m2);
    BOOST_CHECK(m2 == m);

    m2.setVal(2, 8, 5);
    m2.checkIntegrity();

    BOOST_CHECK(!(m == m2));


    std::cout << "error here" << std::endl;
    m.setVal(2, 8, 5);
    std::cout << "error here2" << std::endl;
    m.checkIntegrity();
    BOOST_CHECK(m == m2);
    BOOST_CHECK(m2 == m);

    std::cout << "error here" << std::endl;
    m.setVal(2, 8, 3);
    m.checkIntegrity();
    BOOST_CHECK(!(m == m2));

    m2.setVal(2, 8, 3);
    m2.checkIntegrity();
    BOOST_CHECK((m == m2));

    m.setVal(0, 0, 7);
    m.checkIntegrity();
    BOOST_CHECK(!(m == m2));

    m2.setVal(0, 0, 7);
    m2.checkIntegrity();
    BOOST_CHECK((m == m2));
}

BOOST_AUTO_TEST_CASE( opij ) {

    cout << "operator()" << endl;

    SMatrix m(7,14);
    m.checkIntegrity();
    m.setVal(2, 6, 5);
    m.checkIntegrity();
    m.setVal(4, 0, 3);
    m.checkIntegrity();

    m.setVal(0, 0, 7);
    m.checkIntegrity();
    m.setVal(6, 13, 8);
    m.checkIntegrity();

    BOOST_CHECK_EQUAL(m(0,0), 7);
    BOOST_CHECK_EQUAL(m(6,13), 8);

}
/*
BOOST_AUTO_TEST_CASE( opaddsub ) {
    //@todo
    return;
    cout << "operator+-" << endl;

    SMatrix m(2,3);
    m.checkIntegrity();
    m.setVal(0, 2, 5);
    m.checkIntegrity();
    m.setVal(1, 1, 3);
    m.checkIntegrity();

    m.setVal(0, 0, 7);
    m.checkIntegrity();
    m.setVal(1, 2, 7);
    m.checkIntegrity();
    m.setVal(0, 1, 8);
    m.checkIntegrity();

    SMatrix m1(2,3);

    std::cout << "m " << m << std::endl;
    std::cout << "m1 " << m1 << std::endl;
    m1 += m;
    BOOST_CHECK(m1 == m);

    SMatrix m0(2,3);
    SMatrix m2 = m1 + m0;
    BOOST_CHECK(m2 == m1);
    BOOST_CHECK(m2 == m);
    
    SMatrix m3 = m + m0 + m1 + m2;
    BOOST_CHECK((m3 - m - m1) == m);
    m3 -= m;
    m3 -= m;
    BOOST_CHECK((m3) == m);

  //SMatrix a(500000000, 1000000000);
  //for (SMatrix::size_type i = 0; i < 500; ++i)
    //for (SMatrix::size_type j = 0; j < 500; ++j)
      //a.setVal(i + 1000000, j + 1000000, -1);

  //SMatrix b(1000000000, 500000000);
  //for (SMatrix::size_type i = 0; i < 500; ++i)
    //for (SMatrix::size_type j = 0; j < 500; ++j)
      //b.setVal(i + 1000000, j + 1000000, -1);
    //std::cout << "ADd sub" << std::endl;

   //SMatrix abadd = a + a;
   ////SMatrix abmin = a - a;
}
*/

BOOST_AUTO_TEST_CASE( opmult ) {
    cout << "operator*" << endl;

    SMatrix m(3,3);
    m.checkIntegrity();
    m.setVal(0, 2, 5);
    m.checkIntegrity();
    m.setVal(1, 1, 3);
    m.checkIntegrity();

    m.setVal(0, 0, 7);
    m.checkIntegrity();
    m.setVal(1, 2, 7);
    m.checkIntegrity();
    m.setVal(0, 1, 8);
    m.checkIntegrity();

    prettyMatrix(m);

    SMatrix m1(8,8);
      try {
        m1 *= m;
      } catch (const MatrixError &e) {
        std::string err(e.what());
          std::cout << err << std::endl;
        if (err.substr(0, 17) == "Matrix size error") {
          std::cout << err << std::endl;
          std::cout << "Test passed!" << std::endl;
        } else {
          std::cout << "Test failed!" << std::endl;
        }
      } catch (...) { }


    SMatrix::col_map_type colMap; 
    m.buildColsMap(colMap, SMatrix::size_type(0), m.cols() - 1);

    std::cout << "finnished build" << std::endl;
    for (SMatrix::col_map_type::const_iterator it = colMap.begin();
         it != colMap.end(); ++it) {
        SMatrix::size_type col = it->first;
        std::cout << "col " << col << std::endl;
         SMatrix::col_map_type::mapped_type colvals = it->second;

        for (SMatrix::col_map_type::mapped_type::const_iterator it2 = colvals.begin();
             it2 != colvals.end(); ++it2) {
            std::cout << "row " << it2->first << " val " << it2->second << std::endl;
            
            BOOST_CHECK_EQUAL(it2->second, m(it2->first,col));
        }
    }

    //SMatrix m2 = m * m;
    //prettyMatrix(m2);
    std::cout << m * m << std::endl;

  SMatrix a(50, 10);
  for (SMatrix::size_type i = 0; i < 5; ++i)
    for (SMatrix::size_type j = 0; j < 5; ++j)
      a.setVal(i + 1, j + 1, -1);

  SMatrix b(10, 50);
  for (SMatrix::size_type i = 0; i < 5; ++i)
    for (SMatrix::size_type j = 0; j < 5; ++j)
      b.setVal(i + 1, j + 1, -1);
  SMatrix ab = a*b;

  std::cout << a * b << std::endl;
  a *= b;
  std::cout << a << std::endl;
  BOOST_CHECK(a == (ab));

    
}
/*
BOOST_AUTO_TEST_CASE( sumrow ) {
  SMatrix a(10, 10);
  for (SMatrix::size_type i = 0; i < 5; ++i)
    for (SMatrix::size_type j = 0; j < 5; ++j)
      a.setVal(i , j , -1);

  SMatrix b(10, 10);
  for (SMatrix::size_type i = 0; i < 5; ++i)
    for (SMatrix::size_type j = 0; j < 5; ++j)
      b.setVal(i + 1, j + 1, -1);

    a.setVal(1,0,8);
    b.setVal(1,0,1);
    a.setVal(1,9,7);
    b.setVal(1,9,1);
    a.setVal(0,9,7);
    b.setVal(0,9,1);
    a.setVal(9,9,7);
    b.setVal(9,9,1);

  std::cout << "a" << std::endl;
  prettyMatrix(a);
  std::cout << "b" << std::endl;
  prettyMatrix(b);
  
  SMatrix res_tmp = SMatrix::sumRow(a, a.ridx_.find(1)->second, b, b.ridx_.find(1)->second);
  std::cout << "ress" << std::endl;
  prettyMatrix(res_tmp);
  SMatrix res_tmp0 = SMatrix::sumRow(a, a.ridx_.find(0)->second, b, b.ridx_.find(0)->second);
  std::cout << "ress0" << std::endl;
  prettyMatrix(res_tmp0);
  SMatrix res_tmp9 = SMatrix::sumRow(a, a.ridx_.find(9)->second, b, b.ridx_.find(9)->second);
  std::cout << "ress9" << std::endl;
  prettyMatrix(res_tmp9);
  

}
*/

BOOST_AUTO_TEST_CASE( insertRows ) {
    std::cout << " ===================insertrows" << std::endl;
  SMatrix a(10, 10);
  for (SMatrix::size_type i = 0; i < 5; ++i)
    for (SMatrix::size_type j = 0; j < 5; ++j)
      a.setVal(i , j , -1);

  SMatrix b(10, 10);
  for (SMatrix::size_type i = 0; i < 5; ++i)
    for (SMatrix::size_type j = 0; j < 5; ++j)
      b.setVal(i + 1, j + 1, -1);

    a.setVal(1,0,8);
    b.setVal(1,0,1);
    a.setVal(1,9,7);
    b.setVal(1,9,1);
    a.setVal(0,9,7);
    b.setVal(0,9,1);
    a.setVal(9,9,7);
    b.setVal(9,9,1);

  std::cout << "a" << std::endl;
  prettyMatrix(a);
  std::cout << "b" << std::endl;
  prettyMatrix(b);

  a.insertRow(b, 9, 0);
  std::cout << "a" << std::endl;
  prettyMatrix(a);


}
//BOOST_AUTO_TEST_CASE( addrows) {
    //SMatrix m(2,3);
    //m.checkIntegrity();
    //m.setVal(0, 2, 5);
    //m.checkIntegrity();
    //m.setVal(1, 1, 3);
    //m.checkIntegrity();

    //m.setVal(0, 0, 7);
    //m.checkIntegrity();
    //m.setVal(1, 2, 7);
    //m.checkIntegrity();
    //m.setVal(0, 1, 8);
    //m.checkIntegrity();

    //m.addRows(0,1);
    ////BOOST_CHECK_EQUAL(m(0,0), 7);
    ////BOOST_CHECK_EQUAL(m(0,1), 11);
    ////BOOST_CHECK_EQUAL(m(0,2), 12);
//}

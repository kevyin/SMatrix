##
## Makefile for COMP3171/9171 Deliverable 2: Sparse Matrix Class
##

# you may need to modify this line as necessary
#CXX = g++-4.3
CXX = g++
# enable this for Mac OS X or some CSE servers
#CXX = g++-4.2

# compiler flags
CPPFLAGS = -Wall -Werror -O2
# enable this line for debugging
# CPPFLAGS = -Wall -g

default: test0 test1 test2 test3 mytest0

test0: SMatrix.h SMatrix.cpp test0.cpp
	$(CXX) $(CPPFLAGS) -o test0 SMatrix.cpp test0.cpp

test1: SMatrix.h SMatrix.cpp test1.cpp
	$(CXX) $(CPPFLAGS) -o test1 SMatrix.cpp test1.cpp

test2: SMatrix.h SMatrix.cpp test2.cpp
	$(CXX) $(CPPFLAGS) -o test2 SMatrix.cpp test2.cpp

test3: SMatrix.h SMatrix.cpp test3.cpp
	$(CXX) $(CPPFLAGS) -o test3 SMatrix.cpp test3.cpp
	
mytest0: SMatrix.h SMatrix.cpp mytest0.cpp
	$(CXX) $(CPPFLAGS) -DTEST -o mytest0 SMatrix.cpp mytest0.cpp

mytests: SMatrix.h SMatrix.cpp mytests.cpp
	$(CXX) $(CPPFLAGS) -DTEST -lboost_unit_test_framework -o mytests SMatrix.cpp mytests.cpp

mytests2: SMatrix.h SMatrix.cpp mytests2.cpp
	$(CXX) $(CPPFLAGS) -DTEST -lboost_unit_test_framework -o mytests2 SMatrix.cpp mytests2.cpp

clean: 
	rm -rf *.o a.out core *.dSYM

immaculate: clean
	rm -f test? *~

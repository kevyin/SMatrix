#ifndef SMATRIX_H
#define SMATRIX_H

#include <exception>
#include <iostream>
#include <map>
#include <string>
#include <utility>

#include <cstddef>


// matrix error class
class MatrixError : public std::exception {
 public:
  MatrixError(const std::string& what_arg) : _what(what_arg) { }
  virtual const char* what() const throw() { return _what.c_str (); }
  virtual ~MatrixError() throw() { }
 private:
  std::string _what;
};


// sparse matrix class
class SMatrix {
 public:
  // public interface
  // *** YOU SHOULD NOT MODIFY THE PUBLIC INTERFACE IN ANY WAY! ***

  // types
  typedef size_t size_type;

  // friends
  friend bool operator==(const SMatrix&, const SMatrix&);
  friend bool operator!=(const SMatrix&, const SMatrix&);
  friend SMatrix operator+(const SMatrix&, const SMatrix&) throw(MatrixError); 
  friend SMatrix operator-(const SMatrix&, const SMatrix&) throw(MatrixError); 
  friend SMatrix operator*(const SMatrix&, const SMatrix&) throw(MatrixError); 
  friend SMatrix transpose(const SMatrix&);
  friend std::ostream& operator<<(std::ostream&, const SMatrix&);
  
  // constructors
  SMatrix(size_type, size_type);
  SMatrix(std::istream&);
  SMatrix(const SMatrix&);

  // operators  
  SMatrix& operator=(const SMatrix&); 
  SMatrix& operator+=(const SMatrix&) throw(MatrixError);
  SMatrix& operator-=(const SMatrix&) throw(MatrixError);
  SMatrix& operator*=(const SMatrix&) throw(MatrixError);
  int operator()(size_type, size_type) const throw(MatrixError);
  
  // operations
  inline size_type rows() const;
  inline size_type cols() const;
  bool setVal(size_type, size_type, int) throw(MatrixError);
  void addRows(size_type, size_type) throw(MatrixError);
  void addCols(size_type, size_type) throw(MatrixError);
  void subRows(size_type, size_type) throw(MatrixError);
  void subCols(size_type, size_type) throw(MatrixError);
  void swapRows(size_type, size_type) throw(MatrixError);
  void swapCols(size_type, size_type) throw(MatrixError);

  // `iterator' operations
  void begin() const;
  bool end() const;
  void next() const;
  int value() const;

  // destructor
  ~SMatrix();
  
  // static members  
  static SMatrix identity(size_type);

 private:
  // private types
  //typedef std::pair<size_t, unsigned int> row_loc_type;
  // private data members
  int *vals_;
  size_type *cidx_;
  std::map< size_type, std::pair<size_t, unsigned int> > ridx_;

  size_type rows_;
  size_type cols_;

  // amount of elements being used by vals_ and cidx_
  size_type vals_size_; 
  size_type cidx_size_; 
  // amount of elements allocated for vals_ and cidx_
  size_type vals_capacity_; 
  size_type cidx_capacity_; 

  // private functions
  void checkIntegrity();

  bool array_expand();

  // setVal
  size_t idx_setVal(size_type, size_type); 
  void vals_setVal(size_t, int);

  size_t idx_delVal(size_type, size_type);
  void vals_delVal(size_t);
};

#endif

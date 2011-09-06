#ifdef TEST
#define private public
#endif

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
  inline size_type rows() const {
    return this->rows_;
  }
  inline size_type cols() const {
    return this->cols_;
  }
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
  ///////////////////////////////////////////////////////////////////////////////////////
  // private types
  typedef std::pair<size_t, unsigned int> row_loc_type;
  typedef std::map< size_type, row_loc_type > ridx_type;
  typedef std::pair<size_t, bool> insert_pos_type; // (pos, true=insert false=overwrite)

  // map(col_idx, map(row_idx, val))
  typedef std::map< size_type, int> row_map_type;
  typedef std::map< size_type, row_map_type> col_map_type;
  
  ///////////////////////////////////////////////////////////////////////////////////////
  // private data members
  int *vals_;
  size_type *cidx_;
  //std::map< size_type, std::pair<size_t, unsigned int> > ridx_;
  //std::map< size_type, row_loc_type > ridx_;
  ridx_type ridx_;

  size_type rows_;
  size_type cols_;

  // amount of elements being used by vals_ and cidx_
  size_t vals_size_; 
  size_t cidx_size_; 
  // amount of elements allocated for vals_ and cidx_
  size_t vals_capacity_; 
  size_t cidx_capacity_; 
  
  // iterator pos
  mutable size_t iter_pos;

  ///////////////////////////////////////////////////////////////////////////////////////
  // private functions
  void checkIntegrity();

  void init(size_type, size_type);
  void copy(const SMatrix&);
  
  /*
   * Allocate more memory to vals_ and cidx_ arrays if needed
   */
  bool array_expand();

  // vals_ and cidx_ methods
  insert_pos_type idx_setVal(const size_type&, const size_type&); 
  void vals_setVal(const size_t&, const bool& , const int&);
  void cidx_setVal(const size_t&, const bool& , const int&);

  bool delVal(const size_type&, const size_type&);
  void vals_delVal(const size_t&);
  void vals_delVals(const size_t& pos, const size_t& num); 
  void cidx_delVal(const size_t&);
  void cidx_delVals(const size_t& pos, const size_t& num); 

  // ridx_ methods
  /*
   * Apply shift to the starting position of all rows begining from >= idx
   */
  void ridx_shift(const size_t&, const long int&);
  void ridx_update(ridx_type::iterator&, size_type, row_loc_type);
  void ridx_update_numCheck(ridx_type::iterator&, size_type, size_t, unsigned int);

  std::string dimString() const;

  // rows and cols
  /*
   * Build a map of cols from first to last inclusive
   */
  void buildColsMap(col_map_type&, size_type, size_type) const;
  static SMatrix sumRow(const SMatrix&, const row_loc_type, const SMatrix&, const row_loc_type, const int& rhsMult);
  void insertRow(const SMatrix& fromM, const row_loc_type& loc, const size_type& toRow, const int& mult);
  void insertRow(const SMatrix& fromM, const size_type& fromRow, const size_type& toRow, const int& mult);
  void clearRow(const size_type& row);

  ///////////////////////////////////////////////////////////////////////////////////////
  // private static members
  static MatrixError sizeError(const std::string, const std::string, const std::string);
  static MatrixError boundError(const size_type, const size_type, const std::string); 
};

#endif

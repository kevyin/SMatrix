#include "SMatrix.h"

#include <cmath>
#include <algorithm>

// Your fantastic implementation goes here!
// You should test it extensively to ensure that it behaves exactly 
// as required by the specification, it does not leak memory and is
// optimised for performance!

// helper functions
template <typename T>
void array_insert(T array[], size_t array_len,  size_t pos, T val) {
    for (size_t i = array_len; i > pos; i--) {
        array[i] = array[i-1];
    }
    array[pos] = val;
}

template <typename T>
void array_remove(T array[], size_t array_len,  size_t pos) {
    for (size_t i = pos; i < array_len - 1; i++) {
        array[i] = array[i+1];
    }
}

template <typename T>
void array_print(T array[], size_t array_len) {
    for (size_t k = 0; k < array_len; k++) {
        std::cout << array[k] << ' ';
    }
    std::cout << std::endl;
}

// friends
std::ostream& operator<<(std::ostream &os, const SMatrix &m) {

    // print summary

    // print elements

    return os;
}

// constructors
SMatrix::SMatrix(size_type m, size_type n) {
    rows_ = m;
    cols_ = n;

    size_t capacity = std::min((int)std::ceil(m*n), 1000);
    vals_ = new int [capacity];
    vals_capacity_ = capacity;
    vals_size_ = 0;

    cidx_ = new size_type [capacity];
    cidx_capacity_ = capacity;
    cidx_size_ = 0;
}

//SMatrix(std::istream&);
//SMatrix(const SMatrix&);

// operators  

//SMatrix& SMatrix::operator=(const SMatrix& rhs) {
    //if (this == &rhs) return *this;
    
//}
//SMatrix& SMatrix::operator+=(const SMatrix&) throw(MatrixError);
//SMatrix& SMatrix::operator-=(const SMatrix&) throw(MatrixError);
//SMatrix& SMatrix::operator*=(const SMatrix&) throw(MatrixError);
//int SMatrix::operator()(size_type s1, size_type s2) const throw(MatrixError) {
//
//}

//// operations
inline SMatrix::size_type SMatrix::rows() const {
    return rows_;
}

inline SMatrix::size_type SMatrix::cols() const {
    return cols_;
}

bool SMatrix::setVal(size_type i, size_type j, int v) throw(MatrixError) {
    int arr[10] = {5,14,5413,3,123,41,234,1234,34};
    array_print(arr, 9);

    array_insert<int>(arr,9,2,999);
    array_print(arr, 10);


    bool memAlloc = false;
    // check validity

    if (v != 0) {
        // allocate more memory if necesary
        checkIntegrity();
        if (vals_size_ == vals_capacity_) {
            memAlloc = this->array_expand();
        }
        // update the internal representation
        size_t pos = this->idx_setVal(i, j);
        this->vals_setVal(pos, v);
    } else {
        // update the internal representation
        size_t pos = this->idx_delVal(i, j);
        this->vals_delVal(pos);
    }

    // check matrix integrity
    // memory vals_size_ <= capapcity
    
    return memAlloc;
}


//void addRows(size_type, size_type) throw(MatrixError);
//void addCols(size_type, size_type) throw(MatrixError);
//void subRows(size_type, size_type) throw(MatrixError);
//void subCols(size_type, size_type) throw(MatrixError);
//void swapRows(size_type, size_type) throw(MatrixError);
//void swapCols(size_type, size_type) throw(MatrixError);

//// `iterator' operations
//void begin() const;
//bool end() const;
//void next() const;
//int value() const;

// destructor
SMatrix::~SMatrix() {
    delete [] vals_;
    delete [] cidx_;
}

// private functions
void SMatrix::checkIntegrity() {
    assert(vals_size_ == cidx_size_);
    assert(vals_capacity_ == cidx_capacity_);
    assert(vals_size_ <= cidx_capacity_);
}

bool SMatrix::array_expand() {
    return true;
}

size_t SMatrix::idx_setVal(size_type i, size_type j) {
    return 1;
}

void SMatrix::vals_setVal(size_t pos, int v) {
    array_insert<int>(vals_, vals_size_, pos);
    --vals_size_;
}

size_t SMatrix::idx_delVal(size_type i, size_type j) {
    return 0;
}
void SMatrix::vals_delVal(size_t pos) {
    array_remove<int>(vals_, vals_size_, pos);
    --vals_size_;
}



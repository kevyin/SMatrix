#include "SMatrix.h"

#include <cmath>
#include <algorithm>
#include <assert.h>

#include <sstream>


// Your fantastic implementation goes here!
// You should test it extensively to ensure that it behaves exactly 
// as required by the specification, it does not leak memory and is
// optimised for performance!

// helper functions
template <typename T>
void array_insert(T*& array, const size_t& array_len,  const size_t& pos, const T& val) {
    for (size_t i = array_len; i > pos; i--) {
        array[i] = array[i-1];
    }
    array[pos] = val;
}

template <typename T>
void array_remove(T*& array, const size_t& array_len,  const size_t& pos, const size_t num) {
    for (size_t i = pos; i < array_len - 1; i++) {
        array[i] = array[i+num];
    }
}

template <typename T>
void array_copy(T* const& array1, const size_t& array1_len, T*& array2, size_t& array2_len) {
    assert(array1_len <= array2_len);
    //for (size_t i = 0; i < array1_len; i++) {
        //array2[i] = array1[i];
    //}
    std::copy(&array1[0], &array1[array1_len], array2);
}

template <typename T>
size_t array_find(const T* const& array, const size_t& first, const size_t& last, const T& val) {
    size_t i = first;
    while (i <= last && array[i] < val) ++i;
    return i;
}


template <typename T>
void array_print(const T* const& array, const size_t start, const size_t last) {
    for (size_t k = start; k <= last; k++) {
        std::cout << array[k] << ' ';
    }
    std::cout << std::endl;
}
    
struct triple {
    SMatrix::size_type first;
    SMatrix::size_type second;
    int third;
};

/*
 * parse a tuple of numbers in the format: (a,b,c)
 */
template <typename T1, typename T2, typename T3>
triple parseTriple(std::string& line) {
    assert (line[0] == '(');
    assert (line[line.size() - 1] == ')');

    // Strip brackets and convert to stream
    std::stringstream ss(line.substr(1,line.size() - 2));
    std::string val;

    triple trip;
    std::getline(ss, val, ','); 
    trip.first = static_cast<T1>(atol(val.c_str()));

    std::getline(ss, val, ','); 
    trip.second = static_cast<T2>(atol(val.c_str()));

    std::getline(ss, val, ','); 
    trip.third = static_cast<T3>(atol(val.c_str()));

    return trip;
}

template <typename I, typename J, typename K>
std::string wrap_triple(I i, J j, K k) {
    std::stringstream ss;
    ss << "(" << i << "," << j << "," << k <<")";
    return ss.str();
}

// friends
bool operator==(const SMatrix& l, const SMatrix& r) {
    if (!(l.rows_ == r.rows_ && l.cols_ == r.cols_ &&
        l.vals_size_ == r.vals_size_ && l.cidx_size_ == r.cidx_size_)) 
        return false;
    if (&l == &r) return true;

    // compare each row
    for (SMatrix::ridx_type::const_iterator lit = l.ridx_.begin(), rit = r.ridx_.begin();
         lit != l.ridx_.end() && rit != r.ridx_.end();
         ++lit, ++rit) {

        if (lit->first != rit->first) return false; // compare row number
        SMatrix::row_loc_type lloc = lit->second;
        SMatrix::row_loc_type rloc = rit->second;
        if (lloc.second != rloc.second) return false; // number of vals in each row
        size_t lstart = lloc.first;
        size_t llast = lstart + lloc.second - 1;
        size_t rstart = rloc.first;
        size_t rlast = rstart + rloc.second - 1;
        // compare vals and cidx 
        for (size_t lk = lstart, rk = rstart;
             lk <= llast && rk <= rlast;
             ++lk, ++rk) {

             if (l.vals_[lk] != r.vals_[rk]) return false;
             if (l.cidx_[lk] != r.cidx_[rk]) return false;
        }
    }

    return true;
}
bool operator!=(const SMatrix& l, const SMatrix& r) {
    return !(l == r);
}
SMatrix operator+(const SMatrix& lhs, const SMatrix& rhs) throw(MatrixError) {
    if (lhs.rows() != rhs.rows() || lhs.cols() != rhs.cols()) {
        throw SMatrix::sizeError(lhs.dimString(), rhs.dimString(), "+");
    }
    SMatrix m(lhs);
    m += rhs;
    return m;
}
SMatrix operator-(const SMatrix& lhs, const SMatrix& rhs) throw(MatrixError) {
    if (lhs.rows() != rhs.rows() || lhs.cols() != rhs.cols()) {
        throw SMatrix::sizeError(lhs.dimString(), rhs.dimString(), "-");
    }
    SMatrix m(lhs);
    m -= rhs;
    return m;

}
SMatrix operator*(const SMatrix& lhs, const SMatrix& rhs) throw(MatrixError) {
    if (lhs.cols() != rhs.rows()) {
        throw SMatrix::sizeError(lhs.dimString(), rhs.dimString(), "*");
    }
    SMatrix res(lhs.rows(), rhs.cols());

    // a memory of columns built from rhs
    SMatrix::col_map_type colMap; 
    // build all cols in rhs
    rhs.buildColsMap(colMap, SMatrix::size_type(0), rhs.cols() - 1);

    // foreach row in lhs
    for (SMatrix::ridx_type::const_iterator lit = lhs.ridx_.begin(); 
         lit != lhs.ridx_.end(); ++lit) {

        SMatrix::size_type lhs_row = lit->first;
        SMatrix::row_loc_type loc = lit->second;
        size_t lstart = loc.first;
        size_t llast = lstart + loc.second - 1;

        // foreach col in rhs
        for (SMatrix::col_map_type::const_iterator rit = colMap.begin();
             rit != colMap.end(); ++rit) {
            SMatrix::size_type rhs_col = rit->first;

            //SMatrix::col_map_type::mapped_type *colvals = &rit->second;

            // calculate value of res(lhs_row,rhs_col)
            int res_val = 0;
            for(size_t li = lstart; li <= llast; ++li) {
                SMatrix::size_type lhs_col = lhs.cidx_[li];
                SMatrix::col_map_type::mapped_type::const_iterator colit = (rit->second).find(lhs_col);
                if (colit != (rit->second).end()) {
                    res_val += lhs.vals_[li] * colit->second;
                }
            }
            if (res_val != 0) res.setVal(lhs_row, rhs_col, res_val);
        }

    }
    return res;

}

SMatrix transpose(const SMatrix& m) {
    // a memory of columns built from m
    SMatrix::col_map_type colMap; 
    // build all cols in rhs
    m.buildColsMap(colMap, SMatrix::size_type(0), m.cols() - 1);

    SMatrix res(m.cols(), m.rows());

    for (SMatrix::col_map_type::const_iterator col_it = colMap.begin(); 
         col_it != colMap.end(); ++col_it) {

        SMatrix::size_type col = col_it->first;
        SMatrix::row_map_type rowMap = col_it->second;
        
        for (SMatrix::row_map_type::const_iterator row_it = rowMap.begin();
             row_it != rowMap.end(); ++row_it) {

            SMatrix::size_type row = row_it->first;
            int val = row_it->second;
            res.setVal(col,row,val);
        }
    }
    return res;
}

std::ostream& operator<<(std::ostream &os, const SMatrix &m) {

    // print summary
    os << wrap_triple(m.rows_, m.cols_, m.vals_size_);

    // print elements
    for (SMatrix::ridx_type::const_iterator it = m.ridx_.begin(); it != m.ridx_.end(); ++it) {
        os << std::endl;
        SMatrix::row_loc_type loc = it->second;
        size_t start = loc.first;
        size_t last = start + loc.second - 1;

        for (size_t i = start; i <= last; ++i) {
            os << wrap_triple(it->first, m.cidx_[i], m.vals_[i]);
            if (i < last) os << ' ';
        }
    }
    return os;
}

// constructors

SMatrix::SMatrix(size_type m, size_type n) {
    init(m,n);
}

SMatrix::SMatrix(std::istream& is) {
    std::string line;
    triple tri;

    // initialize from first line
    is >> line;
    tri = parseTriple<SMatrix::size_type, SMatrix::size_type, int>(line);
    init(tri.first, tri.second);

    // add values
    while (is >> line) {
        tri = parseTriple<SMatrix::size_type, SMatrix::size_type, int>(line);
        setVal(tri.first, tri.second, tri.third);
        checkIntegrity();
    }

}

SMatrix::SMatrix(const SMatrix& m) {
    copy(m);
}

// operators  

SMatrix& SMatrix::operator=(const SMatrix& rhs) {
    if (this != &rhs) {
        copy(rhs);
    } 
    return *this;
}

SMatrix& SMatrix::operator+=(const SMatrix& rhs) throw(MatrixError) {
    if (this->rows() != rhs.rows() || this->cols() != rhs.cols()) {
        throw sizeError(this->dimString(), rhs.dimString(), "+=");
    }

    SMatrix res(rhs.rows(),rhs.cols());
    SMatrix& lhs= *this;
    SMatrix::size_type maxRows = res.rows();
    
    bool isEnd = false;
    SMatrix::ridx_type::const_iterator lridx_it = lhs.ridx_.begin();
    SMatrix::ridx_type::const_iterator rridx_it = rhs.ridx_.begin();
    SMatrix::size_type lrow = lridx_it->first;
    SMatrix::size_type rrow = rridx_it->first;
    while (!isEnd) {
        // update lrow and rrow
        if (lridx_it != lhs.ridx_.end()) {
            lrow = lridx_it->first;
        } else {
            lrow = maxRows; // set to past last row
        }
        if (rridx_it != rhs.ridx_.end()) {
            rrow = rridx_it->first;
        } else {
            rrow = maxRows; // set to past last row
        }

        if (lrow == rrow) {
            // compute sum of row
            SMatrix res_tmp = sumRow(lhs, lridx_it->second, rhs, rridx_it->second, 1);
            // insert new row to res
            res.insertRow(res_tmp, 0, lrow, 1); // insert from row 0 of result
            if (lridx_it != lhs.ridx_.end()) ++lridx_it;
            if (rridx_it != lhs.ridx_.end()) ++rridx_it;
        } else if (lrow < rrow) {
            // insert lrow to res
            res.insertRow(lhs, lridx_it->second, lrow, 1);
            if (lridx_it != lhs.ridx_.end()) ++lridx_it;
        } else {
            // insert rrow to res
            res.insertRow(rhs, rridx_it->second, rrow, 1);
            if (rridx_it != lhs.ridx_.end()) ++rridx_it;
        }

        if (lridx_it == lhs.ridx_.end() && rridx_it == rhs.ridx_.end()) 
            isEnd = true;
    }
    

    delete [] vals_;
    delete [] cidx_;
    copy(res);
    return *this;
}

SMatrix& SMatrix::operator-=(const SMatrix& rhs) throw(MatrixError) {
    if (this->rows() != rhs.rows() || this->cols() != rhs.cols()) {
        throw sizeError(this->dimString(), rhs.dimString(), "-=");
    }

    SMatrix res(rhs.rows(),rhs.cols());
    SMatrix& lhs= *this;
    SMatrix::size_type maxRows = res.rows();
    
    bool isEnd = false;
    SMatrix::ridx_type::const_iterator lridx_it = lhs.ridx_.begin();
    SMatrix::ridx_type::const_iterator rridx_it = rhs.ridx_.begin();
    SMatrix::size_type lrow = lridx_it->first;
    SMatrix::size_type rrow = rridx_it->first;
    while (!isEnd) {
        // update lrow and rrow
        if (lridx_it != lhs.ridx_.end()) {
            lrow = lridx_it->first;
        } else {
            lrow = maxRows; // set to past last row
        }
        if (rridx_it != rhs.ridx_.end()) {
            rrow = rridx_it->first;
        } else {
            rrow = maxRows; // set to past last row
        }

        if (lrow == rrow) {
            // compute diff of row
            SMatrix res_tmp = sumRow(lhs, lridx_it->second, rhs, rridx_it->second, -1);
            // insert new row to res
            res.insertRow(res_tmp, 0, lrow, 1); // insert from row 0 of result
            if (lridx_it != lhs.ridx_.end()) ++lridx_it;
            if (rridx_it != lhs.ridx_.end()) ++rridx_it;
        } else if (lrow < rrow) {
            // insert lrow to res
            res.insertRow(lhs, lridx_it->second, lrow, 1);
            if (lridx_it != lhs.ridx_.end()) ++lridx_it;
        } else {
            // insert rrow to res
            res.insertRow(rhs, rridx_it->second, rrow, -1);
            if (rridx_it != lhs.ridx_.end()) ++rridx_it;
        }

        if (lridx_it == lhs.ridx_.end() && rridx_it == rhs.ridx_.end()) 
            isEnd = true;
    }
    

    delete [] vals_;
    delete [] cidx_;
    copy(res);
    return *this;
}
SMatrix& SMatrix::operator*=(const SMatrix& rhs) throw(MatrixError) {
    if (this->cols() != rhs.rows()) {
        throw sizeError(this->dimString(), rhs.dimString(), "*=");
    }
    SMatrix tmp = *this * rhs;
    delete [] vals_;
    delete [] cidx_;
    copy(tmp);
    return *this;    
}

int SMatrix::operator()(size_type row, size_type col) const throw(MatrixError) {
    if (!(row < rows() && col < cols())) 
        throw boundError(row, col, this->dimString());


    ridx_type::const_iterator ridx_it = ridx_.find(row);
    if (ridx_it != ridx_.end()) {
        row_loc_type row_loc = ridx_it->second;
        size_t start = row_loc.first;
        unsigned int num_elements = row_loc.second;
        size_t last = start + num_elements - 1;

        // find the position
        //size_t i = start;
        //while (cidx_[i] < col && i <= last) ++i;
        size_t i = array_find<size_type>(cidx_, start, last, col);
        if (i <= last && cidx_[i] == col) {
            return vals_[i];
        } else {
            return 0;
        }
    } else {
        return 0;
    }
}

//// operations

bool SMatrix::setVal(size_type i, size_type j, int v) throw(MatrixError) {
    if (!(i < rows() && j < cols())) 
        throw boundError(i, j, this->dimString());

    bool memAlloc = false;
    if (v != 0) {
        // allocate more memory if necesary
        checkIntegrity();
        if (vals_size_ >= vals_capacity_) {
            memAlloc = this->array_expand();
        }
        // update the internal representation
        insert_pos_type pos = this->idx_setVal(i, j);
        this->vals_setVal(pos.first, pos.second, v);
    } else {
        // update the internal representation
        this->delVal(i,j);
    }

    checkIntegrity();
    return memAlloc;
}


void SMatrix::addRows(size_type row1, size_type row2) throw(MatrixError) {
    if (!(row1 < rows()))  
        throw boundError(row1, 0, this->dimString());
    if (!(row2 < rows()))  
        throw boundError(row2, 0, this->dimString());
    
    ridx_type::const_iterator r1_it = ridx_.find(row1);   
    ridx_type::const_iterator r2_it = ridx_.find(row2);   
    if (r1_it != ridx_.end() && r2_it != ridx_.end()) { // both rows contain values
        SMatrix tmp_res = sumRow(*this, r1_it->second, *this, r2_it->second, 1);
        insertRow(tmp_res, 0, row1, 1);
    } else if (r1_it == ridx_.end() && r2_it != ridx_.end()) { // row1 is zero row2 is not
        SMatrix tmp_res(1,cols());
        tmp_res.insertRow(*this, row2, 0, 1);
        insertRow(tmp_res, 0, row1, 1);
    } // otherwise no need to do work
        
}

void SMatrix::addCols(size_type col1, size_type col2) throw(MatrixError) {
    if (!(col1 < cols()))  
        throw boundError(col1, 0, this->dimString());
    if (!(col2 < cols()))  
        throw boundError(col2, 0, this->dimString());

    SMatrix tmp = transpose(*this);
    tmp.addRows(col1,col2);

    delete [] this->vals_;
    delete [] this->cidx_;
    this->copy(transpose(tmp));
}

void SMatrix::subRows(size_type row1, size_type row2) throw(MatrixError) {
    if (!(row1 < rows()))  
        throw boundError(row1, 0, this->dimString());
    if (!(row2 < rows()))  
        throw boundError(row2, 0, this->dimString());
    
    ridx_type::const_iterator r1_it = ridx_.find(row1);   
    ridx_type::const_iterator r2_it = ridx_.find(row2);   
    if (r1_it != ridx_.end() && r2_it != ridx_.end()) { // both rows contain values
        SMatrix tmp_res = sumRow(*this, r1_it->second, *this, r2_it->second, -1);
        insertRow(tmp_res, 0, row1, 1);
    } else if (r1_it == ridx_.end() && r2_it != ridx_.end()) { // row1 is zero row2 is not
        SMatrix tmp_res(1,cols());
        tmp_res.insertRow(*this, row2, 0, 1);
        insertRow(tmp_res, 0, row1, -1);
    } // otherwise no need to do work
        
}

void SMatrix::subCols(size_type col1, size_type col2) throw(MatrixError) {
    if (!(col1 < cols()))  
        throw boundError(col1, 0, this->dimString());
    if (!(col2 < cols()))  
        throw boundError(col2, 0, this->dimString());

    SMatrix tmp = transpose(*this);
    tmp.subRows(col1,col2);

    delete [] this->vals_;
    delete [] this->cidx_;
    this->copy(transpose(tmp));
}

void SMatrix::swapRows(size_type row1, size_type row2) throw(MatrixError) {
    if (!(row1 < rows()))  
        throw boundError(row1, 0, this->dimString());
    if (!(row2 < rows()))  
        throw boundError(row2, 0, this->dimString());
   
    SMatrix tmp_row1(1,cols());
    tmp_row1.insertRow(*this, row1, 0, 1);

    SMatrix tmp_row2(1,cols());
    tmp_row2.insertRow(*this, row2, 0, 1);

    insertRow(tmp_row2, 0, row1, 1);
    insertRow(tmp_row1, 0, row2, 1);
}

void SMatrix::swapCols(size_type col1, size_type col2) throw(MatrixError) {
    if (!(col1 < cols()))  
        throw boundError(col1, 0, this->dimString());
    if (!(col2 < cols()))  
        throw boundError(col2, 0, this->dimString());

    SMatrix tmp = transpose(*this);
    tmp.swapRows(col1,col2);

    delete [] this->vals_;
    delete [] this->cidx_;
    this->copy(transpose(tmp));
}

// `iterator' operations
void SMatrix::begin() const {
    iter_pos = 0;
}

bool SMatrix::end() const {
    return (iter_pos > (rows_ * cols_ - 1)) ? true : false;
}

void SMatrix::next() const {
    ++iter_pos;
}

int SMatrix::value() const {
    // calculate row and col, use operator() to get value
    return (*this)((iter_pos / cols_), (iter_pos % cols_));
}

// destructor
SMatrix::~SMatrix() {
    delete [] vals_;
    delete [] cidx_;
    ridx_.clear();
    ridx_.~map();
}

// static members  
SMatrix SMatrix::identity(size_type size) {
    SMatrix id(size, size);
    for (size_t i = 0; i < size; ++i)
        id.setVal(i,i,1);
    return id;
}

// private functions
void SMatrix::checkIntegrity() {
    assert(vals_size_ == cidx_size_);
    assert(vals_capacity_ == cidx_capacity_);
    assert(vals_size_ <= cidx_capacity_);
    assert(rows_*cols_ >= vals_size_);
}

void SMatrix::init(size_type m, size_type n) {
    rows_ = m;
    cols_ = n;

    size_t capacity = std::min(std::ceil((m*n)/5.0), 1000.0);
    vals_ = new int [capacity];
    vals_capacity_ = capacity;
    vals_size_ = 0;

    cidx_ = new size_type [capacity];
    cidx_capacity_ = capacity;
    cidx_size_ = 0;
}

/*
 * copy contents from another matrix
 */
void SMatrix::copy(const SMatrix& m) {
    
    rows_ = m.rows_;
    cols_ = m.cols_;

    vals_size_ = m.vals_size_;
    cidx_size_ = m.cidx_size_;
    vals_capacity_ = m.vals_capacity_;
    cidx_capacity_ = m.cidx_capacity_;

    vals_ = new int [vals_capacity_];
    array_copy<int>(m.vals_, m.vals_size_, vals_, vals_size_); 

    cidx_ = new size_type [cidx_capacity_];
    array_copy<size_type>(m.cidx_, m.cidx_size_, cidx_, cidx_size_); 

    ridx_ = ridx_type(m.ridx_);
}

bool SMatrix::array_expand() {

//#ifdef TEST
    //checkIntegrity();
    //std::cout << "array_expand " << std::endl;
    //std::cout << "before capacity " << vals_capacity_ << std::endl;
//#endif
    checkIntegrity();
    if (vals_size_ < vals_capacity_) return false;
    if (cidx_size_ < cidx_capacity_) return false;

    size_t new_cap = 2*vals_capacity_;
    
    int* new_vals_ = new int [new_cap];
    //array_print(new_vals_, new_cap);
    array_copy<int>(vals_, vals_size_, new_vals_, new_cap);
    //array_print(vals_, vals_capacity_);
    //array_print(new_vals_, new_cap);
    delete [] vals_;

    size_type* new_cidx_ = new size_type [new_cap];
    //array_print(new_cidx_, new_cap);
    array_copy<size_type>(cidx_, cidx_size_, new_cidx_, new_cap);
    //array_print(cidx_, cidx_capacity_);
    //array_print(new_cidx_, new_cap);
    delete [] cidx_;

    vals_ = new_vals_;
    vals_capacity_ = new_cap;

    cidx_ = new_cidx_;
    cidx_capacity_ = new_cap;

//#ifdef TEST
    //checkIntegrity();
    //std::cout << "after capacity " << vals_capacity_ << std::endl;
//#endif

    return true;
}

SMatrix::insert_pos_type SMatrix::idx_setVal(const size_type& row, const size_type& col) {
    // check for record of row
    ridx_type::iterator ridx_it = ridx_.find(row);
    if (ridx_it != ridx_.end()) {
        row_loc_type row_loc = ridx_it->second;
        size_t start = row_loc.first;
        unsigned int num_elements = row_loc.second;
        size_t last = start + num_elements - 1;

        // find the position to modify/insert
        size_t i = array_find<size_type>(cidx_, start, last, col);

        if (i <= last && cidx_[i] == col) { // overwrite old value
            return insert_pos_type(i, false);
        } else { // create space, insert
            
            ridx_update(ridx_it, row, row_loc_type(start, num_elements + 1));
            //ridx_.erase(ridx_it);
            //ridx_.insert(std::pair<size_type, row_loc_type>(row, row_loc_type(start, ++num_elements)));
            // shift positions of later rows
            ridx_shift(start+1, 1);

            cidx_setVal(i, true, col);
            return insert_pos_type(i, true);
        }
    } else { // create room for a new row
        ridx_.insert(std::pair<size_type, row_loc_type>(row, row_loc_type(cidx_size_, 1)));
        // insert at end of array
        size_t pos = cidx_size_;
        cidx_setVal(pos, true, col);
        return insert_pos_type(pos, true);
    }

}

void SMatrix::vals_setVal(const size_t& idx, const bool& isInsert, const int& v) {
    if (isInsert && vals_size_ >= vals_capacity_) {
        array_expand();
    }

    if (isInsert) { // insert
        array_insert<int>(vals_, vals_size_, idx, v);
        ++vals_size_;
    } else { // dont insert, modify old value
        vals_[idx] = v;
    }
}

void SMatrix::cidx_setVal(const size_t& idx, const bool& isInsert, const int& v) {
    if (isInsert && cidx_size_ >= cidx_capacity_) {
        array_expand();
    }
    // Current design means there will never be a situation where a value in
    // cidx_ is overwritten, only inserted or deleted
    assert(isInsert == true);

    array_insert<size_t>(cidx_, cidx_size_, idx, v);
    ++cidx_size_;
}

bool SMatrix::delVal(const size_type& row, const size_type& col) {

    // check for record of row
    ridx_type::iterator ridx_it = ridx_.find(row);
    if (ridx_it != ridx_.end()) {
        row_loc_type row_loc = ridx_it->second;
        size_t start = row_loc.first;
        unsigned int num_elements = row_loc.second;
        size_t last = start + num_elements - 1;

//#ifdef TEST
    //std::cout << "num_elements " << num_elements << std::endl;
    //std::cout << "start " << start << std::endl;
    //std::cout << "last " << last << std::endl;
//#endif

        // find the position to delete
        //size_t i = start;
        //while (cidx_[i] < col && i <= last) ++i;
        size_t i = array_find<size_type>(cidx_, start, last, col);

//#ifdef TEST
    //std::cout << "i " << i << std::endl;
    //std::cout << "cidx_[i] " << cidx_[i] << std::endl;
//#endif
        if (i <= last && cidx_[i] == col) { 
            // vals_
            vals_delVal(i);

            // cidx
            cidx_delVal(i);

            // ridx_
            ridx_update_numCheck(ridx_it, row, start, num_elements - 1);
            //if (num_elements > 1) {
                //ridx_update(ridx_it, row, row_loc_type(start, num_elements - 1));
                ////ridx_.erase(ridx_it);
                ////ridx_.insert(std::pair<size_type, row_loc_type>(row, row_loc_type(start, --num_elements)));
            //} else {
                //ridx_.erase(ridx_it);
            //}

            this->ridx_shift(start+1, -1);
            return true;
        }
    } 

    return false;
}

void SMatrix::vals_delVal(const size_t& pos) {
    array_remove<int>(vals_, vals_size_, pos, 1);
    --vals_size_;
}

void SMatrix::vals_delVals(const size_t& pos, const size_t& num) {
    array_remove<int>(vals_, vals_size_, pos, num);
    vals_size_ -= num;
}


void SMatrix::cidx_delVal(const size_t& pos) {
    array_remove<SMatrix::size_type>(cidx_, cidx_size_, pos, 1);
    --cidx_size_;
}

void SMatrix::cidx_delVals(const size_t& pos, const size_t& num) {
    array_remove<SMatrix::size_type>(cidx_, cidx_size_, pos, num);
    cidx_size_ -= num;
}


void SMatrix::ridx_update(ridx_type::iterator& it, size_type row, row_loc_type loc) {
    //ridx_.erase(it);
    //ridx_.insert(it, ridx_type::value_type(row, loc));
    it->second = loc;
}

void SMatrix::ridx_update_numCheck(ridx_type::iterator& it, size_type row, size_t start, unsigned int numElem) {
    if (numElem > 0) {
        ridx_update(it, row, row_loc_type(start, numElem));
    } else {
        ridx_.erase(it);
    }
}

/*
 * shift ridx entries with row location starting pos >= idx
 */
void SMatrix::ridx_shift(const size_t& idx, const long int& shift) {
    for (ridx_type::iterator ridx_it = ridx_.begin(); ridx_it != ridx_.end(); ridx_it++) {
        if ((ridx_it->second).first >= idx) {
            row_loc_type row_loc = ridx_it->second;

            //size_type row = ridx_it->first;
            size_t start = row_loc.first;
            unsigned int num_elements = row_loc.second;

            //ridx_update(ridx_it, row, row_loc_type(start + shift, num_elements));
            ridx_it->second = row_loc_type(start + shift, num_elements);
            //ridx_.erase(ridx_it);
            //ridx_.insert(std::pair<size_type, row_loc_type>(row, row_loc_type(start + shift, num_elements)));
        }
    }
}
  
std::string SMatrix::dimString() const {
    std::stringstream ss;
    ss << this->rows() << " x " << this->cols();

    return ss.str();
}

// rows and cols
void SMatrix::buildColsMap(col_map_type& map, size_type firstCol, size_type lastCol) const {
    assert(firstCol <= lastCol);

    for (SMatrix::ridx_type::const_iterator it = ridx_.begin(); it != ridx_.end(); ++it) {
        size_type row = it->first;
        SMatrix::row_loc_type loc = it->second;
        size_t start = loc.first;
        size_t last = start + loc.second - 1;

        for (size_t i = array_find<size_type>(cidx_, start, last, firstCol); i <= last; ++i) {
            size_type col = cidx_[i];
            if (firstCol <= col && col <= lastCol) {
                int val = vals_[i];

                // find col in map and update
                col_map_type::iterator itCol = map.find(col);
                if (itCol != map.end()) {
                    (itCol->second).insert(std::make_pair<size_type, int>(row, val));
                } else {
                    row_map_type tmp_map;
                    tmp_map.insert(std::make_pair<size_type, int>(row, val));
                    map.insert(col_map_type::value_type(col, tmp_map));    
                }
            }
        }

    }
}

SMatrix SMatrix::sumRow(const SMatrix& lhs, const SMatrix::row_loc_type lloc, const SMatrix& rhs, const SMatrix::row_loc_type rloc, const int& rhsMult) {
    assert(lhs.rows() == rhs.rows() && lhs.cols() == rhs.cols());

    SMatrix res(1, lhs.cols());
    unsigned int colsInserted = 0;
    SMatrix::size_type maxCols = res.cols();

    bool isEnd = false;
    int* lvals = lhs.vals_ + lloc.first;
    SMatrix::size_type* lcidx = lhs.cidx_ + lloc.first;
    int* rvals = rhs.vals_ + rloc.first;
    SMatrix::size_type* rcidx = rhs.cidx_ + rloc.first;

    size_t li = 0;
    size_t lsize = lloc.second;
    size_t ri = 0;
    size_t rsize = rloc.second;
    while (!isEnd) {
        // update new column or else set to end
        SMatrix::size_type lcol = (li < lsize) ? lcidx[li] : maxCols;
        SMatrix::size_type rcol = (ri < rsize) ? rcidx[ri] : maxCols;
        
        if (lcol == rcol) {
            int value = lvals[li] + rhsMult*rvals[ri];
            res.setVal(0, lcol, value);
            if (li < lsize) ++li;
            if (ri < rsize) ++ri;
        } else if (lcol < rcol) {
            int value = lvals[li];
            res.setVal(0, lcol, value);
            if (li < lsize) ++li;
        } else {
            int value = rhsMult*rvals[ri];
            res.setVal(0, rcol, value);
            if (ri < rsize) ++ri;
        }
        colsInserted++;

        if ((li == lsize && ri == rsize) || (lcol == maxCols && rcol == maxCols)) 
            isEnd = true;
    }
    return res;

}

/*
 * Insert a row from another matrix
 * cannot be from same matrix
 * overwrites original row
 */
void SMatrix::insertRow(const SMatrix& fromM, const row_loc_type& loc, const size_type& toRow, const int& mult) {
    assert (this != &fromM && toRow < this->rows());

    ridx_type::iterator thisRow_it = this->ridx_.find(toRow);
    if (thisRow_it != this->ridx_.end())  // row exists
        this->clearRow(toRow);
    // append new row
    int* fromM_vals = fromM.vals_ + loc.first;
    size_type* fromM_cidx = fromM.cidx_ + loc.first;
    for (size_t i = 0; i < loc.second; ++i) {
        // set val will be quick because values are appended
        this->setVal(toRow, fromM_cidx[i], mult*fromM_vals[i]);
    }
    
}

void SMatrix::insertRow(const SMatrix& fromM, const size_type& fromRow, const size_type& toRow, const int& mult) {
    ridx_type::const_iterator it = fromM.ridx_.find(fromRow);
    if (it != fromM.ridx_.end())
        insertRow(fromM, it->second, toRow, mult);
    else
        clearRow(toRow);
}

void SMatrix::clearRow(const size_type& row) {
    ridx_type::iterator it = ridx_.find(row);
    if (it != ridx_.end()) {
        row_loc_type loc = it->second;
        vals_delVals(loc.first, loc.second);
        cidx_delVals(loc.first, loc.second);
        ridx_.erase(it);
        long int shift = loc.second;
        ridx_shift(loc.first + 1, -1*shift);
    }
}

MatrixError SMatrix::sizeError(const std::string l, const std::string r, const std::string op) {
    std::stringstream ss;
    ss << "(" << l << ")" << " " << op << " " << "(" << r << ")";
    MatrixError err("Matrix size error: " + ss.str());
    return err;
}

MatrixError SMatrix::boundError(const size_type row, const size_type col, const std::string dim) {
    std::stringstream ss;
    ss << "(" << row << ", " << col << ")" << " entry of " << dim << " matrix";
    MatrixError err("Matrix bound error: " + ss.str());
    return err;
}

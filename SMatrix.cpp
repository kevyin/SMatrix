#include "SMatrix.h"

#include <cmath>
#include <algorithm>
#include <assert.h>

//#include <string>
// @TODO delete after
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
void array_remove(T*& array, const size_t& array_len,  const size_t& pos) {
    for (size_t i = pos; i < array_len - 1; i++) {
        array[i] = array[i+1];
    }
}

template <typename T>
void array_copy(T* const& array1, const size_t& array1_len, T*& array2, size_t& array2_len) {
    assert(array1_len <= array2_len);
    for (size_t i = 0; i < array1_len; i++) {
        array2[i] = array1[i];
    }
}

template <typename T>
void array_print(T*& array, const size_t array_len) {
    for (size_t k = 0; k < array_len; k++) {
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
    SMatrix m(lhs);
    m *= rhs;
    return m;
}

SMatrix transpose(const SMatrix& m) {
    SMatrix tm(m.cols(), m.rows());

    // iterate through each row of m
    for (SMatrix::ridx_type::const_iterator it = m.ridx_.begin();
         it != m.ridx_.end(); ++it) {

        size_t row = it->first;

        SMatrix::row_loc_type loc = it->second;
        size_t start = loc.first;
        size_t last = start + loc.second - 1;

        for (size_t k = start; k <= last; ++k) {
            size_t col = m.cidx_[k];
            tm.setVal(col,row,m.vals_[k]);
        }
    }
    return tm;
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

    //std::cout << "plus" << std::endl;
    for (size_type i = 0; i < rows_; ++i) {
        for (size_type j = 0; j < cols_; ++j) {
            //std::cout << "i " << i << "j " << j << std::endl;
            
            int thisVal = (*this)(i,j);
            int res = thisVal + rhs(i,j);
            if (thisVal != res) (*this).setVal(i,j,res);
        }
    }
    return *this;
}
SMatrix& SMatrix::operator-=(const SMatrix& rhs) throw(MatrixError) {
    if (this->rows() != rhs.rows() || this->cols() != rhs.cols()) {
        throw sizeError(this->dimString(), rhs.dimString(), "-=");
    }

    for (size_type i = 0; i < rows_; ++i) {
        for (size_type j = 0; j < cols_; ++j) {
            int thisVal = (*this)(i,j);
            int res = thisVal - rhs(i,j);
            if (thisVal != res) (*this).setVal(i,j,res);
        }
    }
    return *this;
}
SMatrix& SMatrix::operator*=(const SMatrix& rhs) throw(MatrixError) {
    if (this->cols() != rhs.rows()) {
        throw sizeError(this->dimString(), rhs.dimString(), "*=");
    }

    for (size_type i = 0; i < rows_; ++i) {
        for (size_type j = 0; j < cols_; ++j) {
            int thisVal = (*this)(i,j);
            int res = 0;
            for (size_type k = 0; k < cols_; ++k) {
                res += (*this)(i,k) * rhs(k,j);
            }
            if (thisVal != res) (*this).setVal(i,j,res);
        }
    }
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
        size_t i = start;
        // @todo binary search
        while (cidx_[i] < col && i <= last) ++i;
        if (cidx_[i] == col) {
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
        if (vals_size_ == vals_capacity_) {
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


//void SMatrix::addRows(size_type row1, size_type row2) throw(MatrixError) {
    //if (!(row1 < rows_ && row2 < cols_)) 
        //throw boundError(row1, 0, this->dimString());
    
    //ridx_type::iterator it1 = ridx_.find(row1);
    //ridx_type::iterator it2 = ridx_.find(row2);

    //row_loc_type loc1 = it1->second;
    //row_loc_type loc2 = it2->second;

    //size_t start1 = loc1.first;
    //size_t start2 = loc2.first;
    //size_t last1 = start1 + loc1.second - 1;
    //size_t last2 = start2 + loc2.second - 1;
    //assert(start1 <= last1 && start2 <= last2);

    //long int insertCount = 0; // used to update ridx_ later
    //size_t pos1 = last1;
    //size_t pos2 = last2;
    //bool done = false;
    //while (!done) {
        //std::cout << "start1 " << start1 << " start2 " << start2 << std::endl;
        //std::cout << "pos1 " << pos1 << " pos2 " << pos2 << std::endl;
        //const size_t col1 = cidx_[pos1];
        //const size_t col2 = cidx_[pos2];
        //if (col1 == col2) {
            //int res = vals_[pos1] + vals_[pos2];
            //if (res != 0) {
                //vals_[pos1] = res;
            //} else {
                //vals_delVal(pos1);
                //cidx_delVal(pos1);
                //--insertCount;
            //}
            //if (start1 < pos1) --pos1;
            //if (start2 < pos2) --pos2;
        //} else if (col1 < col2) { // zero value in row1

            //vals_setVal(pos1 + 1, true, vals_[pos2]);
            //cidx_setVal(pos1 + 1, true, col2);
            //// since value inserted, row2 may be shifted right
            //if (start1 < start2) { 
                //++start2; ++last2; ++pos2;
            //}
            //++insertCount;
            //if (start2 < pos2) --pos2;
        //} else { //(col2 < col1) zero value in row2
            //assert(start1 <= pos1);
            //if (start1 < pos1) { //not the end yet, no need to add since a + 0 = a
                //--pos1;
            //} else if (start1 == pos1) { // reached the end
                //vals_setVal(pos1, true, vals_[pos2]);
                //cidx_setVal(pos1, true, col2);
                //// since value inserted, row2 may be shifted right
                //if (start1 < start2) { 
                    //++start2; ++last2; ++pos2;
                //}
                //++insertCount;
                //if (start2 < pos2) --pos2;
            //}
        //}
    //}
    //// update ridx_
    //unsigned int numElem = loc1.second + insertCount;
    //ridx_update_numCheck(it1, row1, start1, numElem);
    //ridx_shift(start1 + 1, insertCount);
//}

//void addCols(size_type, size_type) throw(MatrixError);

//void SMatrix::subRows(size_type row1, size_type row2) throw(MatrixError) {

//}
//void subCols(size_type, size_type) throw(MatrixError);
//void swapRows(size_type, size_type) throw(MatrixError);
//void swapCols(size_type, size_type) throw(MatrixError);

//// `iterator' operations
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
}

// static members  
//SMatrix SMatrix::identity(size_type);

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

    size_t capacity = std::min(static_cast<int>(std::ceil((m*n)/5.0)), 1000);
    vals_ = new int [capacity];
    vals_capacity_ = capacity;
    vals_size_ = 0;

    cidx_ = new size_type [capacity];
    cidx_capacity_ = capacity;
    cidx_size_ = 0;
}

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
        size_t i = start;
        //@todo binary search
        while (cidx_[i] < col && i <= last) ++i;

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
        size_t i = start;
        // todo Binary search
        while (cidx_[i] < col && i <= last) ++i;

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
    array_remove<int>(vals_, vals_size_, pos);
    --vals_size_;
}


void SMatrix::cidx_delVal(const size_t& pos) {
    array_remove<size_t>(cidx_, cidx_size_, pos);
    --cidx_size_;
}


void SMatrix::ridx_update(ridx_type::iterator& it, size_type row, row_loc_type loc) {
    ridx_.erase(it);
    ridx_.insert(std::pair<size_type, row_loc_type>(row, loc));
}

void SMatrix::ridx_update_numCheck(ridx_type::iterator& it, size_type row, size_t start, unsigned int numElem) {
    if (numElem > 0) {
        ridx_update(it, row, row_loc_type(start, numElem));
    } else {
        ridx_.erase(it);
    }
}

void SMatrix::ridx_shift(const size_t& idx, const long int& shift) {
    for (ridx_type::iterator ridx_it = ridx_.begin(); ridx_it != ridx_.end(); ++ridx_it) {
        if ((ridx_it->second).first >= idx) {
            row_loc_type row_loc = ridx_it->second;

            size_type row = ridx_it->first;
            size_t start = row_loc.first;
            unsigned int num_elements = row_loc.second;

            ridx_update(ridx_it, row, row_loc_type(start + shift, num_elements));
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

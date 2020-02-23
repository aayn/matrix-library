//! Matrix class.
/* !
   Class used to handle matrix operations.
*/
#ifndef GUARD_MATRIX_W_VECTOR_HPP
#define GUARD_MATRIX_W_VECTOR_HPP

#include <algorithm>
#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

template <class T>
class matrix {
public:

    typedef typename std::vector<T>::iterator iterator;  /*!< Define iterator for matrix. */
    typedef typename std::vector<T>::const_iterator const_iterator;
    typedef typename std::vector<T>::size_type size_type;
    typedef T value_type;

    //! Default constructor.
    matrix(): num_rows(0), num_cols(0), transpose_toggle(true) {}
    //! Constructor for an m x n matrix.
    matrix(size_type m, size_type n, T init_val=0): transpose_toggle(true)  { initialize(m, n, init_val); }
    matrix(size_type m, size_type n, std::vector<T>& v): transpose_toggle(true) { initialize(m, n, v); }
    // If a 2D vector is passed that is not rectangular, then the missing values are
    // filled with `fill_value`.
    matrix(const std::vector<std::vector<T>>& v, T fill_value=0): transpose_toggle(true) 
        { initialize(v, fill_value); }

    matrix<T>& operator=(const matrix<T>& rhs) {
        if (&rhs != this) {
            data = rhs.data;
            num_rows = rhs.num_rows;
            num_cols = rhs.num_cols;
        }
        return *this;
    }
    
    T& operator()(size_type i, size_type j) {
        return transpose_toggle ? data[i * num_cols + j] : data[j * num_rows + i]; 
    }
    T operator()(size_type i, size_type j) const {
        return transpose_toggle ? data[i * num_cols + j] : data[j * num_rows + i]; 
    }


    iterator begin() { return data.begin(); }
    const iterator begin() const { return data.cbegin(); }

    iterator end() { return data.end(); } // TODO: might want to compute this only once
    const_iterator end() const { return data.cend(); }

    matrix<T> operator*(const matrix<T>& mat) {
        if (!verify_size(mat)) {
            std::string err = "Matrices with sizes (" + std::to_string(num_rows) + ", " +
            std::to_string(num_cols) + ") and (" + std::to_string(mat.num_rows) + ", " + 
            std::to_string(mat.num_cols) + ") cannot be multiplied.";
            throw std::domain_error(err);
        }
        matrix<T> out(num_rows, mat.num_cols);

        for (size_type i = 0; i != num_rows; ++i)
            for (size_type j = 0; j != mat.num_cols; ++j)
                for (size_type k = 0; k != num_cols; ++k)
                    out(i, j) += (*this)(i, k) * mat(k, j);
        
        return out;        
    }

    std::pair<size_type, size_type> size() const {
        return std::make_pair(num_rows, num_cols);
    }

    void display() const {
        std::string out;
        for (size_type i = 0; i < num_rows; ++i) {
            for (size_type j = 0; j < num_cols; ++j) {
                if (j == 0 && num_cols == 1)
                    out += "[" + std::to_string((*this)(i, j)) + "]\n";
                else if (j == 0)
                    out += "[" + std::to_string((*this)(i, j)) + ", ";
                else if (j == (num_cols - 1))
                    out += std::to_string((*this)(i, j)) + "]\n";
                else
                    out += std::to_string((*this)(i, j)) + ", ";
            }
        }
        std::cout << out;
    }

    // Converts self into transpose and returns self; in place conversion 
    // with reference returned; constant-time transpose

    matrix<T>& transpose() {
        std::swap(num_cols, num_rows);
        transpose_toggle ^= 1;
        return *this;
    }

    // Returns a transposed copy
    matrix<T> transpose_copy() {
        
    }

    void gettt() {
        std::cout << transpose_toggle << std::endl;
    }

    std::vector<std::vector<T>> to_2d_vec() const {
        std::vector<std::vector<T>> res;
        for (size_type i = 0; i != num_rows; i++) {
            std::vector<T> row;
            for (size_type j = 0; j != num_cols; j++)
                row.push_back((*this)(i, j));
            res.push_back(row);
        }
        return res;
    }


private:
    std::vector<T> data;
    size_type num_rows, num_cols;
    bool transpose_toggle;

    void initialize(size_type m, size_type n, T init_val=0) {
        num_rows = m;
        num_cols = n;
        data = std::vector<T>(m * n, init_val);
    }

    void initialize(const std::vector<std::vector<T>>& v, size_type fill_value=0) {
        size_type row_size = 0;
        for (auto row: v)
            if (row.size() > row_size)
                row_size = row.size();
        std::cout << row_size << std::endl;
        initialize(v.size(), row_size);
         
        for (size_type i = 0; i < v.size(); i++) {
            for (size_type j = 0; j < row_size; j++)
                (*this)(i, j) = v[i][j];
        }
    }

    void initialize(size_type m, size_type n, std::vector<T>& v) {
        if (m * n != v.size()) {
            std::string err = "Cannot convert given vector of size " + std::to_string(v.size()) +
                " to matrix of size (" + std::to_string(m) + ", " + std::to_string(n) + ").";
            throw std::domain_error(err);
        }
        data = v;
        num_rows = m;
        num_cols = n;
    }

    bool verify_size(const matrix<T>& mat) {
        return (num_cols == mat.num_rows);
    }

    
};


#endif
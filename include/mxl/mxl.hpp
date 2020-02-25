//! Matrix class of the MXL library.
/*!
   Class used to handle matrix operations. It can currently handle matrix
   multiplication and matrix transposing. Salient features include convenient
   constructors, constant-time transposing, concurrent matrix multiplication
   (TODO), and intuitive operator-overloading.

   This is meant to be a single-header library that you can just drop into your
   project.
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
    //! Define iterator for matrix.
    /*! Iterates element-by-element from the top-left element to the 
    bottom-right element. */
    typedef typename std::vector<T>::iterator iterator;  
    //! Same as the matrix iterator but is const.
    typedef typename std::vector<T>::const_iterator const_iterator;
    //! Defines a size type for the given data type T.
    typedef typename std::vector<T>::size_type size_type;
    //! Defines the value_type as T.
    typedef T value_type;

    //! Default constructor.
    matrix(): num_rows(0), num_cols(0), transpose_toggle(true) {}
    
    //! Constructor for an m x n matrix with an initial value.
    /*! 
        \param m the number of rows.
        \param n the number of columns.
        \param init_val the value to fill the matrix with.
        \sa initialize(size_type, size_type, T)
    */
    matrix(size_type m, size_type n, T init_val=0): transpose_toggle(true)  { initialize(m, n, init_val); }
    
    //! Constructor that reshapes an std::vector to create a matrix.
    /*!
        Note that the number of elements in the vector must equal m x n. If 
        not, it will throw a std::domain_error.
        \param m the number of rows.
        \param n the number of columns.
        \param v std::vector that is reshaped to fill the matrix.
        \sa initialize(const std::vector<std::vector<T>>&, size_type)
    */
    matrix(size_type m, size_type n, std::vector<T>& v): transpose_toggle(true) { initialize(m, n, v); }

    //! Constructor that takes in a 2-D std::vector (a vector of vectors) to
    //! create a matrix.
    /*!
        \param v the 2-D std::vector (vector of vectors).
        \param fill_value the value with which undefined indexes are filled.
        \sa initialize(size_type, size_type, std::vector<T>&)
    */
    matrix(const std::vector<std::vector<T>>& v, T fill_value=0): transpose_toggle(true) 
        { initialize(v, fill_value); }

    //! Overloaded = operator.
    /*!
        NOTE: may not need this.
    */
    matrix<T>& operator=(const matrix<T>& rhs) {
        if (&rhs != this) {
            data = rhs.data;
            num_rows = rhs.num_rows;
            num_cols = rhs.num_cols;
        }
        return *this;
    }
    
    //! Overloaded () operator for easy element indexing.
    /*!
        If mat is a matrix then mat(i, j) will access, by reference, the j-th 
        element in the i-th row. By having a toggle flag which controls how an
        element is accessed, we can implement constant-time in-place 
        transposing.
        \param i the row index.
        \param j the column index.
    */    
    T& operator()(size_type i, size_type j) {
        return transpose_toggle ? data[i * num_cols + j] : data[j * num_rows + i]; 
    }

    //! Overloaded () operator, same as above, but returns a copy of the
    //! element instead of a reference.
    /*!
        \param i the row index.
        \param j the column index.
    */
    T operator()(size_type i, size_type j) const {
        return transpose_toggle ? data[i * num_cols + j] : data[j * num_rows + i]; 
    }

    //! Returns an iterator to the beginning (top-left) of the matrix.
    iterator begin() { return data.begin(); }
    
    //! Returns a const iterator to the beginning (top-left) of the matrix.
    const iterator begin() const { return data.cbegin(); }

    //! Returns an iterator refering to one-past the end of the underlying
    //! matrix container.
    iterator end() { return data.end(); }

    //! Returns a const iterator refering to one-past the end of the underlying
    //! matrix container.
    const_iterator end() const { return data.cend(); }

    //! Overloaded * operator for matrix multiplication.
    /*!
        Throws a std::domain_error if the matrices don't have appropriate sizes.
        \param mat the matrix with the multiplication is done.
    */
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
    
    //! Returns the dimensions of the matrix as a std::pair.
    std::pair<size_type, size_type> size() const {
        return std::make_pair(num_rows, num_cols);
    }

    //! Returns a (const) std::string representation of the matrix.
    std::string represent() const {
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
        return out;
    }

    //! Prints the std::string matrix representation to stdout.
    void display() const {
        std::cout << represent();
    }

    //! Returns a 2-D std::vector (vector of vectors) representation of the
    //! matrix.
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

    //! Performs an constant-time, in-place transpose of the matrix.
    /*!
        This is done by simply changing the way in which the underlying
        container is accessed. Also returns a reference to the matrix.
    */
    matrix<T>& transpose() {
        std::swap(num_cols, num_rows);
        transpose_toggle ^= 1;
        return *this;
    }

    //! Returns a copy of the transposed matrix.
    matrix<T> transpose_copy() {
        
    }

private:
    //! The underlying container
    std::vector<T> data;
    //! The number of rows in the matrix
    size_type num_rows;
    //! The number of columns in the matrix
    size_type num_cols;
    //! The toggle which enables constant-time transpose
    bool transpose_toggle;

    //! Initializes the underlying container for the matrix constructor
    /*!
        \param m the number of rows.
        \param n the number of columns.
        \param init_val the value to fill the container with.
        \sa matrix(size_type, size_type, T)
    */
    void initialize(size_type m, size_type n, T init_val=0) {
        num_rows = m;
        num_cols = n;
        data = std::vector<T>(m * n, init_val);
    }

    //! Initializes the underlying container for the matrix reshaped from an 
    //! std::vector.
    /*!
        Note that the number of elements in the vector must equal m x n. If 
        not, it will throw a std::domain_error.
        \param m the number of rows.
        \param n the number of columns.
        \param v std::vector that is reshaped to fill the matrix.
        \sa matrix(size_type, size_type, std::vector<T>&)
    */
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

    //! Intializes the underlying container for the matrix constructed from a
    //! 2-D std::vector (a vector of vectors).
    /*!
        \param v the 2-D std::vector (vector of vectors).
        \param fill_value the value with which undefined indexes are filled.
        \sa matrix(const std::vector<std::vector<T>>&, T)
    */
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

    //! TODO: What is this?
    bool verify_size(const matrix<T>& mat) {
        return (num_cols == mat.num_rows);
    }

    
};


#endif
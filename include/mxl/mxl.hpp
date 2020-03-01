/*! \mainpage The MLX Matrix library.

   This library is used to handle matrix operations. It can currently handle
   matrix multiplication and matrix transposing. Salient features include
   convenient constructors, constant-time transposing, matrix multiplication,
   and intuitive operator-overloading.

   This is meant to be a single-header library that you can just drop in to your
   project.

   \see
     \ref mxl    
     \ref mxl::matrix
*/
#ifndef GUARD_MATRIX_W_VECTOR_HPP
#define GUARD_MATRIX_W_VECTOR_HPP

#include <algorithm>
#include <cstddef>
#include <initializer_list>
#include <iostream>
#include <random>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

//! MXL namespace  
/*!
    The MXL matrix class and related functions are under the mxl namespace.
*/
namespace mxl {

    template <typename T>
    class matrix {
    public:
        //! Define iterator for matrix.
        /*! Iterates element-by-element from the top-left element to the 
        bottom-right element. */
        using iterator = typename std::vector<T>::iterator;  
        //! Same as the matrix iterator but is const.
        using const_iterator = typename std::vector<T>::const_iterator;
        //! Defines a size type for the given data type T.
        using size_type = typename std::vector<T>::size_type;
        //! Defines a dimensions type as std::pair of size_types.
        using dimensions = typename std::pair<size_type, size_type>;
        //! Defines the value_type as T.
        using value_type = T;

        //! Default constructor.
        matrix(): num_rows(0), num_cols(0), transpose_toggle(true) {}

        //! Constructor for an m x n matrix with an initial value.
        /*! 
            \param m the number of rows.
            \param n the number of columns.
            \param init_val the value to fill the container with.
            \sa initialize(size_type, size_type, T)
        */
        matrix(size_type m, size_type n, T init_val=0): 
            num_rows(m), num_cols(n), transpose_toggle(true)  { initialize(init_val); }

        //! Constructor that uses a 2-D std::initializer_list to create a
        //! matrix.
        /*! 
            \param il the std::initializer_list from which the matrix is
            constructed.
        */
        matrix(const std::initializer_list<std::initializer_list<T>>& il): transpose_toggle(true) 
         { initialize(il); }
        
        //! Constructor for an m x n matrix with an "initialzer" (see params).
        /*! 
            \param m the number of rows.
            \param n the number of columns.
            \param initializer a string that can take one of four values: 
                "zeros" (m x n matrix of 0s), "ones" (m x n matrix of 1s), 
            "random" (m x n matrix of random numbers), "identity" (m x n
            identity-like matrix). The random numbers are uniformly continuous
            distributed for "real" types (like doubles or floats), and uniformly 
            discretely distributed between (1 and 1000000) for integral types
            (like int or long).
            \sa initialize(size_type, size_type, std::string)
        */
        matrix(size_type m, size_type n, const std::string& initializer): 
            num_rows(m), num_cols(n), transpose_toggle(true)  { initialize(initializer); }

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
            Returns a dereferenced this.
        */
        matrix<T>& operator=(const matrix<T>& rhs) {
            if (&rhs != this) {
                data = rhs.data;
                num_rows = rhs.num_rows;
                num_cols = rhs.num_cols;
                transpose_toggle = rhs.transpose_toggle;
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

        //! Overloaded *= operator for matrix multiplication.
        /*!
            Throws a std::domain_error if the matrices don't have appropriate sizes.
            \param rhs the matrix with the multiplication is done.
        */
        matrix<T>& operator*=(const matrix<T>& rhs) {
            if (!check_shape_mult(rhs)) {
                std::string err = generate_error_message("multiplied", rhs);
                throw std::domain_error(err);
            }
            // Number of columns in rhs
            size_type ncols = rhs.shape().second;
            matrix<T> out(num_rows, ncols);

            for (size_type i = 0; i != num_rows; ++i)
                for (size_type j = 0; j != ncols; ++j)
                    for (size_type k = 0; k != num_cols; ++k)
                        out(i, j) += (*this)(i, k) * rhs(k, j);
            *this = out;

            return *this;        
        }

        //! Overloaded *= operator for scalar-matrix multiplication.
        /*!
            Throws a std::domain_error if the matrices don't have appropriate sizes.
            \param scalar the scalar with the multiplication is done.
        */
        matrix<T>& operator*=(const T& scalar) {

            for (size_type i = 0; i != num_rows; ++i)
                for (size_type j = 0; j != num_cols; ++j)
                    (*this)(i, j) *= scalar;

            return *this;        
        }

        matrix<T>& operator+=(const matrix<T>& rhs) {
            if (!check_shape_add(rhs)) {
                std::string err = generate_error_message("added", rhs);
                throw std::domain_error(err);
            }

            matrix<T> out(num_rows, num_cols);

            for (size_type i = 0; i != num_rows; ++i)
                for (size_type j = 0; j != num_cols; ++j)
                    out(i, j) = (*this)(i, j) + rhs(i, j);
            *this = out;

            return *this;
        }

        //! Returns true if and only if every single element is the same in 
        //! both the matrices.
        /*!
            \param rhs the matrix against which to compare.
        */
        bool operator==(const matrix<T>& rhs) const {
            if (shape() != rhs.shape())
                return false;
            
            for (size_type i = 0; i != num_rows; i++)
                for (size_type j = 0; j != num_cols; j++)
                    if ((*this)(i, j) != rhs(i, j))
                        return false;
            return true;
        }
        
        //! Returns the dimensions of the matrix as a std::pair.
        dimensions shape() const {
            return std::make_pair(num_rows, num_cols);
        }

        //! Returns a (const) std::string representation of the matrix.
        std::string to_string() const {
            std::string out;
            for (size_type i = 0; i < num_rows; ++i) {
                for (size_type j = 0; j < num_cols; ++j) {
                    if (j == 0 && num_cols == 1)
                        out += "[[" + std::to_string((*this)(i, j)) + "]]\n";
                    else if (i == 0 && j == 0)
                        out += "[[" + std::to_string((*this)(i, j)) + ", ";
                    else if (i == (num_rows - 1) && j == (num_cols - 1))
                        out += std::to_string((*this)(i, j)) + "]]\n";
                    else if (j == 0)
                        out += " [" + std::to_string((*this)(i, j)) + ", ";
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
            std::cout << to_string();
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
            transpose();
            matrix<T> out(*this);
            transpose();
            return out;            
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
            \param init_val the value to fill the container with.
            \sa matrix(size_type, size_type, T)
        */
        void initialize(T init_val=0) {
            data = std::vector<T>(num_rows * num_cols, init_val);
        }

        //! Intializes the underlying container for the matrix constructed from
        //! the 2-D std::initializer_list.
        /*! 
            \param il the std::initializer_list from which the matrix is
            constructed.
        */
        void initialize(const std::initializer_list<std::initializer_list<T>>& il) { 
            num_rows = il.size();
            num_cols = il.begin()->size();
            data = std::vector<T>(num_rows * num_cols);

            using row_il_iter = typename std::initializer_list<std::initializer_list<T>>::iterator;
            using col_il_iter = typename std::initializer_list<T>::iterator;

            size_type i = 0, j = 0;
            row_il_iter rb = il.begin();

            while (rb != il.end()) {
                col_il_iter cb = rb->begin();
                j = 0;
                while (cb != rb->end()) {
                    (*this)(i, j) = *cb;
                    j++; cb++;
                }
                i++; rb++;
            }
        }
        
        //! Initializes the underlying container for the matrix constructor
        /*!
            \param m the number of rows.
            \param n the number of columns.
            \param initializer a string that can take one of four values: 
                "zeros" (m x n matrix of 0s), "ones" (m x n matrix of 1s), 
                "random" (m x n matrix of random numbers), "identity" (m x n
                identity-like matrix).
            \sa matrix(size_type, size_type, T)
        */
        void initialize(const std::string& initializer) {
            if (initializer == "zeros")
                data = std::vector<T>(num_rows * num_cols, 0);
            else if (initializer == "ones")
                data = std::vector<T>(num_rows * num_cols, 1);
            else if (initializer == "random" && std::is_floating_point<T>::value) {
                data = std::vector<T>(num_rows * num_cols);
                const int nrolls = num_rows * num_cols;
                std::default_random_engine generator;
                std::uniform_real_distribution<double> distribution(0.0, 1.0);

                for (iterator b = data.begin(); b != data.end(); b++) {
                    T number = distribution(generator);
                    *b = number;
                }

            } else if (initializer == "random") {
                data = std::vector<T>(num_rows * num_cols);
                const int nrolls = num_rows * num_cols;
                std::default_random_engine generator;
                std::uniform_int_distribution<int> distribution(0, 1000000);
                
                for (iterator b = data.begin(); b != data.end(); b++) {
                    T number = distribution(generator);
                    *b = number;
                }

            } else if (initializer == "identity") {
                data = std::vector<T>(num_rows * num_cols, 0);
                size_type k = std::min(num_rows, num_cols);
                for (size_type i = 0; i != k; i++)
                    (*this)(i, i) = 1;
            }
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

        //! Intializes the underlying container for the matrix constructed from a
        //! 2-D std::vector (a vector of vectors).
        /*!
            \param v the 2-D std::vector (vector of vectors).
            \param fill_value the value with which undefined indexes are filled.
            \sa matrix(const std::vector<std::vector<T>>&, T)
        */
        void initialize(const std::vector<std::vector<T>>& v, size_type fill_value=0) {
            size_type row_size = 0;
            for (auto row: v)
                if (row.size() > row_size)
                    row_size = row.size();
            num_rows = v.size();
            num_cols = row_size;
            data = std::vector<T>(num_rows * num_cols, fill_value);
            
            for (size_type i = 0; i < v.size(); i++) {
                for (size_type j = 0; j < row_size; j++)
                    (*this)(i, j) = v[i][j];
            }
        }

        //! Checks if the matrix dimensions are appropriate for matrix
        //! multiplication.
        /*!
            \param mat the matrix with the multiplication is done.
        */
        bool check_shape_mult(const matrix<T>& rhs) {
            return (num_cols == rhs.shape().first);
        }

        //! Checks if the matrix dimensions are appropriate for matrix
        //! multiplication.
        /*!
            \param mat the matrix with the addition is done.
        */
        bool check_shape_add(const matrix<T>& mat) {
            dimensions d = mat.shape();
            return (d.first == mat.num_rows && d.second == mat.num_cols);
        }

        //! A convenience function for generating error messages.
        /*!
            \param operation the operation for which there was an error.
        */
        std::string generate_error_message(std::string operation, const matrix<T>& mat) {
            
            std::string err = "Matrices with sizes (" + std::to_string(num_rows) + ", " +
                std::to_string(num_cols) + ") and (" + std::to_string(mat.shape().first) + ", " + 
                std::to_string(mat.shape().second) + ") cannot be " + operation + ".";
            return err;
        }

    };

    //! Operator overloading for matrix multiplication.
    /*!
        \param lhs the left matrix.
        \param rhs the right matrix.
    */
    template<typename T>
    matrix<T> operator*(matrix<T> lhs, const matrix<T>& rhs) {
        return lhs *= rhs;
    }

    //! Operator overloading for matrix addition.
    /*!
        \param lhs the left matrix.
        \param rhs the right matrix.
    */
    template<typename T>
    matrix<T> operator+(matrix<T> lhs, const matrix<T>& rhs) {
        return lhs += rhs;
    }

    //! Operator overloading for matrix-scalar multiplication.
    /*!
        \param lhs the left matrix.
        \param scalar the scalar multiplied to the right.
    */
    template<typename T>
    matrix<T> operator*(matrix<T> lhs, const T& scalar) {
        return lhs *= scalar;
    }

    //! Operator overloading for scalar-matrix multiplication.
    /*!
        \param scalar the scalar multiplied to the left.
        \param lhs the right matrix.
    */
    template<typename T>
    matrix<T> operator*(const T& scalar, matrix<T> rhs) {
        return rhs *= scalar;
    }

}
#endif
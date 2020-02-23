//! Matrix class.
/* !
   Class used to handle matrix operations.
*/
#ifndef GUARD_MATRIX_HPP
#define GUARD_MATRIX_HPP

#include <cstddef>
#include <memory>

template <class T>
class matrix {

public:

    typedef T* iterator;  /*!< Define iterator for matrix. */
    typedef const T* const_iterator;
    typedef size_t size_type;
    typedef T value_type;

    //! Default constructor.
    matrix() { initialize(); }
    //! Constructor for an m x n matrix.
    matrix(size_type m, size_type n) { initialize(m, n); }

    matrix(const matrix& m) { initialize(m.begin(), m.end()); }
    matrix& operator=(const matrix& m);
    ~matrix() { destroy(); }

    T& operator()(size_type i, size_type j) {return this[i*num_rows + j]; }

    iterator begin() { return data; }
    const iterator begin() const { return data; }

    iterator end() { return (data + num_rows * num_cols); } // TODO: might want to compute this only once
    const_iterator end() const { return (data + num_rows * num_cols); }



private:
    iterator data; //! Index zero pointer
    size_type num_rows, num_cols;

    T& operator[](size_type i) {return *(data + i); }

    
};


#endif
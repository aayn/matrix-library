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


private:
    
};


#endif
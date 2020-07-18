#include <iostream>
#include <mxl/mxl.hpp>
#include <random>

using namespace std;
using mxl::matrix;

int main() {
    // Initialize
    matrix<int> mat1 = {{1, 2, 3},
                        {4, 5, 6},
                        {7, 8, 9}};
    // Transpose copy
    matrix<int> mat2 = mat1.transpose_copy();
    
    // Convenient display and to_string methods
    cout << "Matrix 1:\n";
    cout << mat1.to_string() << endl;
    cout << "Matrix 2:\n";
    mat2.display(); cout << endl;

    // Matrix multiplication and addition
    matrix<int> mat3 = (mat1 * mat2) + mat1;
    cout << "Matrix 3:\n";
    mat3.display(); cout << endl;

    // Scalar multiplication
    mat3 *= 2;
    cout << "Matrix 3 again:\n";
    mat3.display(); cout << endl;

    // In-place, constant-time transpose
    mat3.transpose();
    cout << "Matrix 3 again, transposed:\n";
    mat3.display();

    // And, much more...
    // See the documentation for all the features.
  
    return 0;
}
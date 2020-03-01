#include <iostream>
#include <mxl/mxl.hpp>

using namespace std;
using mxl::matrix;

int main() {
    // // vector<int> v = {1, 2, 3, 4, 5, 6};
    // vector<vector<int>> v = {{1, 2, 3}, {4, 5, 6}};
    // matrix<int> mat(v);
    // mat.display();
    // mat.transpose();
    // mat.display();
    // mat.transpose();
    // mat.display();

    // matrix<int> mat2;
    // cout << mat2.shape().first;

    matrix<int> mat1(3, 4, 7);
    vector<vector<int>> v1 = {{1, 2, 3},
                              {4, 5, 6},
                              {7, 8, 9},
                              {10, 11, 12}};
    matrix<int> mat2(v1);

    (mat1 * mat2).display();
    (mat2 * mat1).display();

    mat1 = mat1 * mat2;
    // mat1 = mat1 + mat1; 
    mat1.display();
    cout << mat1.shape().second << endl;
    
    // mat1.transpose();
    // mat2.transpose();
    // (mat1 * mat2).display();
    // (mat2 * mat1).display();

    // mat1 *= mat2;
    // // mat1 = mat1 + mat1; 
    // mat1.display();


    // matrix<int> m = {1, 2, 3};
    // m.display();
    // matrix<int> mat1(10, 3, 1), mat2(3, 10, 1);
    // matrix<int> m;
    // m = mat2;
    // mat2 = mat1;

    // // cout << mat1(0, 1);
    // mat1.display();
    // mat2.display();
    // m.display();

    // (mat2 * m).display();

    // int x = 5, y = 10;

    // {{1, 2, 3}, {2,3, 4}};

    return 0;
}
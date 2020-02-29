#include <iostream>
#include <mxl/mxl.hpp>

using namespace std;


int main() {
    mxl::matrix<int, 3, 4> mat1(7);
    mat1.display();
    vector<vector<int>> v1 = {{1, 2, 3, 4},
                              {5, 6, 7, 8},
                              {9, 10, 11, 12}};

    mxl::matrix<int, 4, 3> mat2(v1);

    (mat1 * mat2).display();

    // mxl::square_matrix<double, 5> mat2(6.2832);
    // mat2.display();

    // mxl::matrix<int, 5, 5> mat;
    // // mat = mxl::matrix<int, 5, 5>::I();
    // mat.display();
    // matrix<int> mat(v);
    // mat.display();
    // mat.transpose();
    // mat.display();
    // mat.transpose();
    // mat.display();

    // matrix<int> mat2;
    // cout << mat2.shape().first;

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
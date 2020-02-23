#include <iostream>
#include <mxl/mxl.hpp>

using namespace std;

int main() {
    // vector<int> v = {1, 2, 3, 4, 5, 6};
    vector<vector<int>> v = {{1, 2, 3}, {4, 5, 6}};
    matrix<int> mat(v);
    mat.display();
    mat.gettt();
    mat.transpose();
    mat.display();
    mat.transpose();
    mat.display();

    matrix<int> mat2;
    cout << mat2.size().first;

    matrix<int> m = {1, 2, 3};
    m.display();
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
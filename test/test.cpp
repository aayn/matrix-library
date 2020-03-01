#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "mxl.hpp"

using namespace std;
using mxl::matrix;

TEST_CASE("Verifying the constructors", "[matrix]") {
        
    SECTION("default constructor") {
        matrix<int> mat1;
        matrix<int>::size_type s1 = 0, s2 = 0;
        REQUIRE(mat1.shape() == make_pair(s1, s2));
    }
    SECTION("row, column and intitial value constructor") {
        matrix<double> mat2(7, 12, 3.5);
        vector<vector<double>> v1(7, vector<double>(12, 3.5));
        vector<vector<double>> v2 = mat2.to_2d_vec();

        for (size_t i = 0; i != v1.size(); i++)
            for (size_t j = 0; j != v1[0].size(); j++)
                 REQUIRE(v1[i][j] == v2[i][j]);        
    }
    SECTION("1d vector constructor") {
        vector<long> v3 = {1, 2, 3, 4, 5, 6};
        vector<long> v3_1 = {1, 2, 3, 4, 5, 6};
        vector<long long> v4 = {1, 2, 3, 4, 5, 6, 7, 8};
        matrix<long> mat3(3, 2, v3);
        
        for (size_t i = 0; i != mat3.shape().first; ++i)
            for (size_t j = 0; j != mat3.shape().second; ++j)
                REQUIRE(mat3(i, j) == v3_1[i * mat3.shape().second + j]);
        
        try {
            matrix<long long> mat4(3, 3, v4);
        } catch (domain_error e) {
            REQUIRE(true);
        }
    }
    SECTION("2d vector constructor") {
        vector<vector<int>> v5 = {{1, 2, 3, 4},
                                  {5, 6, 7, 8}};

        matrix<int> m5(v5);
        for (size_t i = 0; i != m5.shape().first; i++)
            for (size_t j = 0; j != m5.shape().second; j++)
                REQUIRE(m5(i, j) == v5[i][j]);                            
    }
}

TEST_CASE("Testing matrix-matrix and matrix-scalar multiplication", "[matrix]") {
    matrix<double> mat1(3, 4, 7);
    vector<vector<double>> v1 = {{1, 2, 3},
                              {4, 5, 6},
                              {7, 8, 9},
                              {10, 11, 12}};
    matrix<double> mat2(v1);
    vector<vector<double>> result = {{154000, 182000, 210000},
                                  {154000, 182000, 210000},
                                  {154000, 182000, 210000}};
    mat1 = 2.0 * mat1 * mat2 * 5.0;
    mat1 += 99.0 * mat1;
    SECTION("separate assignment and multiplication") {

        for (size_t i = 0; i != mat1.shape().first; i++)
            for (size_t j = 0; j != mat1.shape().second; j++)
                REQUIRE(mat1(i, j) == result[i][j]);
    }

    matrix<double> mat3(3, 4, 7);

    SECTION("combined assignment and multiplication") {
        mat3 *= mat2 * 10.0;
        mat3 *= 100.0;

        cout << mat1.shape().first << mat1.shape().second << endl;
        cout << mat3.shape().first << mat3.shape().second << endl;

        REQUIRE((mat3 == mat1) == true);
    }


}
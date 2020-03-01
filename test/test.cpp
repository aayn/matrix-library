#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include <mxl/naik_aayush.hpp>

using namespace std;
using mxl::matrix;

TEST_CASE("Verifying the simple constructors", "[matrix]") {
        
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

    SECTION("2d initializer list") {
        matrix<long> mat1 = {{1, 2, 3, 4, 5, 6}};
        matrix<long> mat2({{1}, {2}, {3}, {4}, {5}, {6}});

        matrix<long>::size_type s1 = 0, s2 = 0;
        s1 = 1; s2 = 6;
        REQUIRE(mat1.shape() == make_pair(s1, s2));
        REQUIRE(mat2.shape() == make_pair(s2, s1));
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

TEST_CASE("Verifying the special constructors", "[matrix]") {
        
    SECTION("zeros") {
        matrix<int> mat1(5, 7, "zeros");
        matrix<int> mat2(5, 7, 0);

        REQUIRE((mat1 == mat2) == true);
    }

    SECTION("ones") {
        matrix<int> mat1(5, 7, "ones");
        matrix<int> mat2(5, 7, 1);

        REQUIRE((mat1 == mat2) == true);
    }

    SECTION("random") {
        matrix<double> mat1(7, 8, "random");
        double prev = 0;
        for (auto x: mat1) {
            REQUIRE(x != prev);
            prev = x;
        }

        matrix<long> mat2(7, 8, "random");
        for (auto x: mat2) {
            REQUIRE((x >= 0 && x <= 1000000));
        }
    }

    SECTION("identity") {
        matrix<long> mat1(7, 7, "identity");
        using size_type = matrix<long>::size_type;
        
        for (size_type i = 0; i != 7; i++) 
            for (size_type j = 0; j != 7; j++) {
                if (i == j)
                    REQUIRE(mat1(i, j) == 1);
                else
                    REQUIRE(mat1(i, j) == 0);
            }
    }
}


TEST_CASE("Testing matrix-matrix and matrix-scalar multiplication", "[matrix]") {
    matrix<double> mat1(3, 4, 7);
    matrix<double> mat2 = {{1, 2, 3},
                           {4, 5, 6},
                           {7, 8, 9},
                           {10, 11, 12}};
    matrix<double> result = {{154000, 182000, 210000},
                             {154000, 182000, 210000},
                             {154000, 182000, 210000}};
    mat1 = 2.0 * mat1 * mat2 * 5.0;
    mat1 += 99.0 * mat1;
    SECTION("separate assignment and multiplication") {

        for (size_t i = 0; i != mat1.shape().first; i++)
            for (size_t j = 0; j != mat1.shape().second; j++)
                REQUIRE(mat1(i, j) == result(i, j));
    }

    matrix<double> mat3(3, 4, 7);

    SECTION("combined assignment and multiplication") {
        mat3 *= mat2 * 10.0;
        mat3 *= 100.0;

        REQUIRE((mat3 == mat1) == true);
    }

    SECTION("error catching") {
        matrix<int> mat4 = {{1, 2, 3, 4}};
        matrix<int> mat5 = {{1, 2, 3}};

        try {
            mat4 *= mat5;
        } catch (std::domain_error e) {
            REQUIRE(true);
        }

        try {
            mat4 += mat5;
        } catch (std::domain_error e) {
            REQUIRE(true);
        }
    }
}

TEST_CASE("Testing transpose methods", "[matrix]") {
    matrix<int> mat1 = {{1, 2, 3},
                        {4, 5, 6},
                        {7, 8, 9}};
    matrix<int> mat2 = {{1, 4, 7},
                        {2, 5, 8},
                        {3, 6, 9}};
    
    SECTION("in-place transpose") {
        mat1.transpose();
        REQUIRE((mat1 == mat2) == true);
        mat1.transpose();
        mat2.transpose();
        REQUIRE((mat1 == mat2) == true);
    }

    SECTION("transpose copy") {
        REQUIRE((mat1.transpose_copy() == mat2) == true);
        REQUIRE((mat1 == mat2.transpose_copy()) == true);
    }
}
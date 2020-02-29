#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "mxl.hpp"

using namespace std;
using mxl::matrix;

TEST_CASE("Verifying the constructors", "[matrix]") {
        
    SECTION("default constructor") {
        matrix<int, 0, 0> mat1;
        size_t s1 = 0, s2 = 0;
        REQUIRE(mat1.shape() == make_pair(s1, s2));
    }

    SECTION("row, column and intitial value constructor") {
        matrix<double, 7, 12> mat2(3.5);
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
        matrix<long, 3, 2> mat3(v3);
        
        for (size_t i = 0; i != mat3.shape().first; ++i)
            for (size_t j = 0; j != mat3.shape().second; ++j)
                REQUIRE(mat3(i, j) == v3_1[i * mat3.shape().second + j]);
        
        try {
            matrix<long long, 3, 3> mat4(v4);
        } catch (domain_error e) {
            REQUIRE(true);
        }
    }
    // SECTION("2d vector constructor") {
    //     vector<vector<int>> v5 = {{1, 2, 3, 4},
    //                               {5, 6, 7, 8}};

    //     matrix<int> m5(v5);
    //     for (size_t i = 0; i != m5.shape().first; i++)
    //         for (size_t j = 0; j != m5.shape().second; j++)
    //             REQUIRE(m5(i, j) == v5[i][j]);                            
    // }
}

TEST_CASE("Testing matrix multiplication", "[matrix]") {
    
}
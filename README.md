# MXL: A Matrix Library
Aayush Naik (anaik@eng.ucsd.edu)

## Summary

A simple header-only matrix library.

## Build and Running Instructions

The main header file, where the library is written, is
[`include/mxl/mxl.hpp`](include/mxl/mxl.hpp). It is a
single-header library, that you can just drop in to your project and use
seamlessly. To see it in action (for building and running) it, do the following.

1. Enter the build directory: `cd build`
2. Set up the makefiles: `cmake ..`
3. Build the project: `make`
4. Run the demo: `./Demo`. The source code for the demo file can be found at
   [`src/demo.cpp`](src/demo.cpp). The demo only gives a "feel for" what the
   library can do. See the documentation for more features.
5. Run the tests: `./Test`

## Documentation

The library documentation can be found at
[`docs/html/index.html`](docs/html/index.html). The main chunk of the
documentation is under `mxl::matrix` (which is a link on the above page). This
should be more than sufficient for the client's engineering staff's
maintainability needs.

## Salient Features

Some salient features of the MXL library are:

1. Convenient initialization. With initialization capabilities that use
   `std::initializer_list`, 1-D and 2-D `std::vector`, and more, getting
   started right away is very easy with the MXL library.
2. Extensively tested. With four large, nested test-cases and a total of 280
   assertions, the functionality of the MXL library has been well-tested and
   the tests demonstrate correctness. The tests can be found in
   [`test/test.cpp`](test/test.cpp).
3. Intuitive operator-overloading. The `*`, `+`, `==` and other operators have
   been overloaded to provide and intuitive interface to operate with the
   matrices.
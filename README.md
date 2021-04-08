# Linear Equation System Solver

This is a solver for linear equation systems in matrix form implemented in C++.
It is based on the gauss algorithm:

https://en.wikipedia.org/wiki/Gaussian_elimination

Typical use can be found in all domains of mathematics, like linear algebra or statistics.

The Matrix is implemented as a class in matrix.h and provides matrix manipulation operations.
Data is hold in an single onedimensional array.

Solving logic is contained in gauss.cpp and handles all possible cases:
Unique solution, no solution, infinite solutions with subsequent choice of free variables.

File input.h provides matrix input logic.

### TODOs
Currently input is given manually or via hard coded array. Input via csv should be implemented.

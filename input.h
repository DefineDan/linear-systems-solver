#include <iostream>
#include "matrix.h"

// returns matrix build from manual input of all elements, does not check input validity
Matrix2D* manual_matrix(){
    int rows = 0;
    int cols = 0;

    // get size of matrix
    std::cout << "Number of Equations? ";
    std::cin >> rows;
    std::cout << "Number of Variables? ";
    std::cin >> cols;
    if(rows < 0 || cols < 0){
        std::cout <<"Invalid inputs\n";
        return nullptr;
    }
    // allocate matrix object
    Matrix2D *M = new Matrix2D(rows, cols + 1);
    
    // get matrix elements
    for (size_t i = 0; i < M->numRows(); i++)
    {
        for (size_t j = 0; j < M->numCols(); j++)
        {
            double val;
            std::cout << "Element (" << i << "," << j << "): \n";
            std::cin >> val;
            M->set(i, j, val);
            M->display();
        }
    }
    return M;
}

// returns matrix build from array, param test_matrix determines which is used
Matrix2D* array_matrix(int test_matrix)
{
    int rows = 0;
    int cols = 0;
    int a = 0;
    
    //test data
    double a1[] = {1,2,3,4,5,6,7,8,9,10,11,12};
    double a2[] = {2,2,4,4,4,8,8,8,15};
    double a3[] = {1,1,1,1,1,2,2,2,2,2,4,4,4,4,4,8,8,8,8,8};
    double* arrays[5];
    arrays[0] = a1;
    arrays[1] = a2;
    arrays[2] = a3;

    // test selection
    switch (test_matrix)
    {
    case 1:
        rows = 2;
        cols = 3;
        a = 0;
        break;
    case 2:
        rows = 3;
        cols = 2;
        a = 0;
        break;
    case 3:
        rows = 4;
        cols = 5;
        a = 2;
        break;
    default:
        rows = 3;
        cols = 4;
        a = 0;
    }
    Matrix2D* M = new Matrix2D(rows,cols,arrays[a]);
    return M;
}
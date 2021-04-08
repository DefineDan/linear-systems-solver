#include <iostream>
#include <map>
#include <vector>
#include "matrix.h"
#include "input.h"

using namespace std;

// declarations

void solve(Matrix2D* M, map<int, double>* choices);

// definitions

// transform matrix to upper triangualar form
void tri_matrix(Matrix2D* M){
    for (size_t i = 0; i < M->numCols() - 1; i++)
    {
        double step = M->get(i,i);
        // switch to find eqation that does not have 0 for variable
        if (step == 0)
        {
            for (size_t j = i + 1; j < M->numRows(); j++)
            {
                if (M->get(i,j) != 0)
                {
                    M->switch_rows(i,j);
                }
            }
        }
        // normalize row for variable
        M->norm_row(i,i);

        // subtract normalized row from other rows
        for (size_t j = i + 1; j < M->numRows(); j++)
        {
            double factor = M->get(j,i);
            if (factor == 0 || M->get(i,i) == 0) 
                continue;
            M->sub_rows(j, i, factor);
        }
    }
}

// transform upper triangualar matrix in normed upper triangualar form
void norm_tri_matrix(Matrix2D* M){
    for (size_t i = 0; i < M->numCols() - 1; i++)
    {
        for (size_t j = i + 1; j < M->numRows(); j++)
        {
            double factor = M->get(i,j);
            if (factor == 0 || M->get(j,j) == 0)
                continue;
            M->sub_rows(i, j, factor);
        }
    }
}  

// find rows in Matrix that are all 0, return as vector
vector<int>* zero_rows(Matrix2D* M)
{
    vector<int>* z_rows = new std::vector<int>();
    for (size_t i = 0; i < M->numRows(); i++)
    {
        bool z_row = true;
        for (size_t j = 0; j < M->numCols() - 1; j++)
        {
            if(round(M->get(i,j) != 0))
            {
                z_row = false;
                break;
            }
        }
        if (z_row)
        {
            z_rows->push_back(i);
        }
    }
    return z_rows;
}

// displays solution for linear equations system dependent on own choices if possible
void display_solution(Matrix2D* M, map<int,double>* choices)
{
    std::cout <<"=== Unique solution ===\n\n";
    int row = 0;
    for (size_t i = 0; i < choices->size() + M->numCols() - 1; i++)
    {
        bool choice = false;    // flag
        // check if variable was chosen
        for ( auto it = choices->begin(); it != choices->end(); ++it  )
        {   
            if (it->first == i)
            {
                std::cout.precision(4);
                std::cout <<"var_" << it->first << " = " << it->second << " (your choice)\n\n";
                choice = true;  // flag
                break;
            }
        }
        // if not chosen get value for variable from solved matrix
        if(!choice)
        {
            std::cout.precision(4);
            std::cout <<"Var_" << i <<" = " << M->get(row++, M->numCols() - 1) <<"\n\n";
        }
    }
}

// linear equation system has infinite solutions, variables can be chosen freely
void choose_variable(Matrix2D* M, vector<int>* z_rows, map<int,double>* choices)
{
    std::cout <<"=== Infinite solutions ===\n\n";
    std::cout <<"You can choose " <<z_rows->size() <<" Variable(s)\n";
    int var = 0;
    double val = 0;

    
    for (size_t i = 0; i < z_rows->size(); i++)
    {
        // get choices dependent on number of zero rows of matrix
        do
        {
            std::cout << "Choose from Var from 0 to " << M->numCols() - 2 <<": \n";
            std::cin >> var;
        } while (var > M->numCols() - 2 || var < 0);
        std::cout << "Choose Value for Var_" << var << ": \n";
        std::cin >> val;

        // eliminate chosen variable
        M->eliminate_var(var, val);
        M->delete_row((*z_rows)[i]);
        M->display();

        // save choice to map in order to display later
        for ( auto it = choices->begin(); it != choices->end(); ++it  )
        { 
            if (it->first <= var)
                var++;
        }
        choices->insert( pair<int,double>(var, val) );
    }
    // solve new resulting matrix
    solve(M, choices);
}

// solves linear equation system in matrix from, allows choices if possible
void solve(Matrix2D* M, map<int, double>* choices)
{
    // get matrix in normalized upper triangular form
    tri_matrix(M);
    norm_tri_matrix(M);
    // check zero rows of matrix
    vector<int>* z_rows = zero_rows(M);
    
    // determine solution
    if (z_rows->size() != 0)
    {
        for (auto i = z_rows->begin(); i != z_rows->end(); i++ ) 
        {
            // no solution
            if (round(M->get(*i, M->numCols() - 1)) != 0)
            {
                std::cout <<"=== No solution ===\n";
                delete z_rows;
                return;
            }
        }
        if (M->numCols() - 1 + z_rows->size() == M->numRows())
        {
            // unique solution
            display_solution(M, choices);
        }
        else
        {
            // infinite solutions
            choose_variable(M, z_rows, choices);    // recursive solve()
        }
    }
    else
    {
        // unique solution
        display_solution(M, choices);
    }
    delete z_rows;
}

int main(){
    char input;
    Matrix2D* M;
    std::cout <<"Linear Equations Solver\n\n";

    // choose input variant
    std::cout <<"Choose input variant (m)anual matrix input or (t)est matrix:\n";
    std::cin >> input;
    int test = 0;
    switch (input)
    {
    case 't':
        std::cout <<"Choose test number:\n";
        std::cin >> input;
        M = array_matrix(test);
        break;
    
    default:
        M = manual_matrix();
        break;
    }
    M->display();
    static std::map<int, double> choices{};
    M->display_operations = true;

    // solve linear equation system
    solve(M, &choices);
    
    delete M;
}
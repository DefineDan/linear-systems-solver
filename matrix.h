#include <iostream>
#include <iomanip> 
#include <ios> 
#include <cmath>

#ifndef MATRIX_H
#define MATRIX_H

class Matrix2D
{
private:
    double *m_array;   // array of values
    int m_rows, m_cols;
    int get_index(int row, int col);
public:
    // constructors
    Matrix2D();
    Matrix2D(int rows, int cols);
    Matrix2D(int rows, int cols, double* array);
    Matrix2D(const Matrix2D& copyMatrix);

    // destructor
    ~Matrix2D();

    // methods
    int size();
    double get(int row, int col);
    bool set(int row, int col, double val);
    int numRows();
    int numCols();
    bool delete_row(int row);
    bool delete_col(int col);
    bool switch_rows(int row1, int row2);
    bool sub_rows(int row1, int row2, double factor);
    bool norm_row(int row, int col);
    bool eliminate_var(int var, double val);
    void display();

    // public vars
    // shows operations of solving procedure
    bool display_operations = false;
};

// constructors
Matrix2D::Matrix2D()
{
    m_rows = 1;
    m_cols = 1;
    m_array = new double[size()]();
    display_operations = false;
}

Matrix2D::Matrix2D(int rows, int cols)
{
    m_rows = rows;
    m_cols = cols;
    m_array = new double[size()]();
}

Matrix2D::Matrix2D(int rows, int cols, double* array)
{
    m_rows = rows;
    m_cols = cols;
    m_array = new double[size()]();
    for (size_t i = 0; i < size(); i++)
        m_array[i] = array[i];
}

Matrix2D::Matrix2D(const Matrix2D& copyMatrix)
{
    m_rows = copyMatrix.m_rows;
    m_cols = copyMatrix.m_cols;
    m_array = new double[size()];
    for (int i = 0; i < size(); i++)
        m_array[i] = copyMatrix.m_array[i];
}

// destructor
Matrix2D::~Matrix2D()
{
    if(m_array) delete[] m_array;
}

// public methods

// matrix size
int Matrix2D::size()
{
    return m_rows*m_cols;
}

// gets value at given row and column
double Matrix2D::get(int row, int col)
{
    int index = get_index(row, col);
    if (index >= 0)
        return m_array[index];
    else
        return 0;
}

// sets element at given row and column to val
bool Matrix2D::set(int row, int col, double val)
{
    int index = get_index(row, col);
    if (index >= 0)
    {
        m_array[index] = val;
        return true;
    }
    else
        return false;
}

int Matrix2D::numRows()
{
    return m_rows;
}

int Matrix2D::numCols()
{
    return m_cols;
}

// switches given rows
bool Matrix2D::switch_rows(int row1, int row2)
{
    int i_row1 = get_index(row1, 0);
    int i_row2 = get_index(row2, 0);
    if (i_row1 < 0 || i_row2  < 0) return false;

    for (int i = 0; i < m_cols; i++)
    {
        double buffer = m_array[i_row1 + i];
        m_array[i_row1 + i] = m_array[i_row2 + i];
        m_array[i_row2 + i] = buffer;
    }
    if (display_operations)
    {
        std::cout << "=== Switching row_" << row1+1 <<  " with row_" << row2 + 1 << " ===\n";
        display();
    }
    return true;
}

// subtracts factor * row2 from row1
bool Matrix2D::sub_rows(int row1, int row2, double factor=1)
{
    int i_row1 = get_index(row1, 0);
    int i_row2 = get_index(row2, 0);
    if (i_row1 < 0 || i_row2  < 0) return false;

    
    for (size_t j = 0; j < m_cols; j++)
    {
        set(row1, j, (get(row1, j) - (get(row2, j) * factor)));
    }
    if (display_operations)
    {
        std::cout << "=== Subtracting " << factor << " * " << "row_" <<  row2 + 1 << " from row_" << row1 + 1 << " ===\n";
        display();
    }
    return true;
}

// norms given row by value in given column
bool Matrix2D::norm_row(int row, int col)
{
    int index = get_index(row, col);
    if (index < 0) return false;

    double factor = m_array[index];
    if (factor == 0 || factor == 1) return true;

    for (size_t j = 0; j < numCols(); j++)
    {
        double element = get(row, j);
        if (round(element*1000) == 0)   // prevent floating point errors
            set(row, j, 0.0);
        set(row, j, (element/factor));
    }
    if (display_operations)
    {
        std::cout << "=== Dividing row_" << row + 1 << " by " << factor << " ===\n";
        display();
    }
    return true;
}

// substitutes variable with value, thereby eliminating its column
bool Matrix2D::eliminate_var(int var, double val)
{
    // calculate new values for last (no variable) column
    double factor = 0;
    for (size_t i = 0; i < m_rows; i++)
    {
        factor = get(i, var);
        set(i, m_cols - 1, get(i, m_cols - 1) - factor*val);
    }
    // delete column of var
    delete_col(var);
    if (display_operations)
    {
        std::cout << "=== Substituting Var_" << var << " with " << val << " ===\n";
        display();
    }
    return true;
}

/* deletes given column, updates number of columns, does not resize array for better performance */
bool Matrix2D::delete_col(int col)
{
    for (size_t i = 0; i < m_rows; i++)
    {
        for (size_t j = m_cols*i + col - i; j < size() - 1; j++)
        {
            m_array[j] = m_array[j+1];
        }
    }
    m_cols--;
    return true;
}

/* deletes given row, updates number of rows, does not resize array for better performance */
bool Matrix2D::delete_row(int row)
{
    for (size_t i = row; i < m_rows - 1; i++)
    {
        for (size_t j = 0; j < m_cols; j++)
        {
            set(i, j, get(i + 1, j));
        }
    }
    m_rows--;
    return true;
}

// prints matrix to std::out
void Matrix2D::display()
{
    std::cout << "\n";
    for (int i = 0; i < m_rows; i++)
    {
        for (int j = 0; j < m_cols; j++)
        {   
            std::cout.precision(2);
            std::cout << std::setw(10) << std::right << std::fixed << m_array[get_index(i,j)] << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

// private methods

// returns array index for given row and column of matrix
int Matrix2D::get_index(int row, int col)
{
    if ((row >= 0) && (row < m_rows) && (col >= 0) && (col < m_cols))
        return (m_cols * row) + col;
    else
        return -1;
}

#endif /* MATRIX_H */
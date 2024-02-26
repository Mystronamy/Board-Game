/*-------------------------------------------
Program 7: Outlast the Baddies & Avoid the Abyss
Course: CS 211, Fall 2023, UIC
System: Advanced zyLab
Author: William Nguyen
------------------------------------------- */

#pragma once

#include <algorithm>
#include <exception>
#include <iostream>
#include <stdexcept>

using namespace std;

template <typename T>
class Grid {
private:
 struct ROW {
  T* Cols; // array of column elements
  size_t NumCols; // total # of columns (0..NumCols-1)
 };

 ROW* Rows; // array of ROWs
 size_t NumRows; // total # of rows (0..NumRows-1)

public:
 //
 // Default Constructor:
 //
 // Purpose: Constructs a default 4x4 Grid.
 // Parameters: None.
 // Returns: A new Grid instance with all elements initialized to default value
 // of T.
 //
 Grid() {
  // Allocating memory for 4 rows (complies with code layout standards)
  Rows = new ROW[4];  // 4 rows for the grid
  NumRows = 4;

  // Loop through each row to initialize columns
  for (size_t r = 0; r < NumRows; ++r) {
   Rows[r].Cols = new T[4];  // Allocating 4 columns per row
   Rows[r].NumCols = 4;

   // Nested loop to initialize each element in the grid to the default value
   for (size_t c = 0; c < Rows[r].NumCols; ++c) {
    Rows[r].Cols[c] = T();  // Setting default value for type T
   }
  }
 }

 //
 // Parameterized Constructor:
 //
 // Purpose: Constructs a Grid with specified number of rows (R) and columns (C).
 //          Each element in the grid is initialized to the default value of
 //          type T.
 // Parameters: size_t R - Number of rows, size_t C - Number of columns.
 // Returns: A new Grid instance with specified dimensions.
 //
 Grid(size_t R, size_t C) {
  // Allocating memory for R rows
  Rows = new ROW[R];  // Dynamically setting the number of rows based on R
  NumRows = R;

  // Loop through each row to initialize columns
  for (size_t r = 0; r < NumRows; r++) {
   Rows[r].Cols = new T[C];  // Allocating C columns for each row
   Rows[r].NumCols = C;

   // Nested loop to initialize each element in the grid to the default value
   for (size_t c = 0; c < Rows[r].NumCols; c++) {
    Rows[r].Cols[c] = T();  // Setting each element to default value of type T
   }
  }
 }

 //
 // Destructor:
 //
 // Purpose: Automatically called to free allocated memory of this Grid instance.
 //          It deallocates memory for each column in every row, followed by
 //          deallocating the rows themselves.
 // Parameters: None.
 // Returns: None.
 //
 virtual ~Grid() {
  // Loop to deallocate memory for each column in every row
  for (size_t r = 0; r < NumRows; r++) {
   delete[] Rows[r].Cols;  // Freeing memory allocated for columns
  }
  delete[] Rows;  // Freeing memory allocated for rows
 }

 //
 // Copy Constructor:
 //
 // Purpose: Constructs a Grid instance that is a copy of an existing Grid.
 //          It ensures a deep copy of the Grid, meaning each element is
 //          individually copied. This constructor is invoked, for example, when
 //          a Grid is passed by value to a function.
 // Parameters: const Grid<T>& other - A reference to the Grid instance to copy
 // from. Returns: A new Grid instance that is a copy of 'other'.
 //
 Grid(const Grid<T>& other) {
  // Copying the number of rows from 'other'
  NumRows = other.NumRows;
  Rows = new ROW[NumRows];  // Allocating memory for rows

  // Loop through each row
  for (size_t r = 0; r < NumRows; r++) {
   Rows[r].NumCols = other.Rows[r].NumCols;  // Copying the number of columns
   Rows[r].Cols = new T[Rows[r].NumCols];    // Allocating memory for columns

   // Nested loop to copy each element
   for (size_t c = 0; c < Rows[r].NumCols; c++) {
    Rows[r].Cols[c] =
        other.Rows[r].Cols[c];  // Copying each element individually
   }
  }
 }

 //
 // Copy Assignment Operator:
 //
 // Purpose: Overloads the assignment operator to allow one Grid to be assigned to another.
 //          It ensures a deep copy of the Grid, avoiding shallow copy issues.
 //          This operator checks for self-assignment and only performs the copy
 //          if 'other' is a different object.
 // Parameters: const Grid& other - A reference to the Grid instance being
 // assigned from. Returns: A reference to the updated Grid instance (*this).
 //
 Grid& operator=(const Grid& other) {
  // Checking for self-assignment
  if (this != &other) {
   // Deallocating existing memory
   for (size_t r = 0; r < NumRows; r++) {
    delete[] Rows[r].Cols;
   }
   delete[] Rows;

   // Copying the number of rows and allocating new memory
   NumRows = other.NumRows;
   Rows = new ROW[NumRows];

   // Loop through each row
   for (size_t r = 0; r < NumRows; r++) {
    Rows[r].NumCols = other.Rows[r].NumCols;  // Copying the number of columns
    Rows[r].Cols = new T[Rows[r].NumCols];    // Allocating memory for columns

    // Nested loop to copy each element
    for (size_t c = 0; c < Rows[r].NumCols; c++) {
     Rows[r].Cols[c] =
         other.Rows[r].Cols[c];  // Copying each element individually
    }
   }
  }

  return *this;  // Returning the current object
 }

 //
 // numrows:
 //
 // Purpose: Retrieves the number of rows in the Grid.
 //          The indices for these rows range from 0 to numrows - 1.
 // Parameters: None.
 // Returns: size_t - The number of rows in the Grid.
 //
 size_t numrows() const {
  // Returning the number of rows in the grid
  return NumRows;  // Currently returns the value of NumRows member variable
 }

 //
 // numcols:
 //
 // Purpose: Returns the number of columns in a specified row of the Grid.
 //          The indices for these columns range from 0 to numcols - 1.
 //          As of now, it is assumed each row has the same number of columns.
 // Parameters: size_t r - The index of the row for which the number of columns
 // is requested. Returns: size_t - The number of columns in row r. If r is out
 // of bounds, returns 0.
 //
 size_t numcols(size_t r) const {
  // Returning the number of columns in row r
  // The ternary operator checks if r is within the bounds of NumRows
  return (r < NumRows) ? Rows[r].NumCols : 0;  // Safe check for row index
 }

 //
 // size:
 //
 // Purpose: Calculates and returns the total number of elements in the Grid.
 //          This is computed by summing up the number of columns in each row.
 // Parameters: None.
 // Returns: size_t - The total number of elements in the Grid.
 //
 size_t size() const {
  // Initializing totalSize to accumulate the total number of elements
  size_t totalSize = 0;

  // Loop through each row to sum up the number of columns
  for (size_t r = 0; r < NumRows; r++) {
   totalSize += Rows[r].NumCols;  // Adding the number of columns in each row
  }

  return totalSize;  // Returning the total size of the grid
 }

 //
 // operator():
 //
 // Purpose: Overloads the parentheses operator to provide direct access to the element
 //          at a specific location (row r, column c) in the Grid. This allows
 //          for both accessing and assigning values to the grid elements.
 //          Throws an exception if the specified indices are out of the grid's
 //          range.
 // Usage:   grid(r, c) = value;  // Assignment
 //          cout << grid(r, c);  // Access
 // Parameters: size_t r - The row index, size_t c - The column index.
 // Returns: T& - A reference to the element at the specified location.
 //
 T& operator()(size_t r, size_t c) {
  // Checking if the provided indices are within the bounds of the grid
  if (r >= NumRows || c >= Rows[r].NumCols) {
   throw out_of_range(
       "Grid indices out of range");  // Throwing exception if out of range
  }

  // Returning a reference to the element at (r, c)
  return Rows[r].Cols[c];
 }

 //
 // _output:
 //
 // Purpose: Provides a debugging tool by outputting the contents of the Grid.
 //          This function iterates through each row and column, printing the
 //          values stored in the Grid. Each row is printed on a new line. Note:
 //          This function is for debugging purposes and is not part of the main
 //          functionality.
 // Parameters: None.
 // Returns: None. Outputs the Grid contents to standard output (e.g., console).
 //
 void _output() {
  // Looping through each row
  for (size_t r = 0; r < NumRows; r++) {
   // Looping through each column in the current row
   for (size_t c = 0; c < Rows[r].NumCols; c++) {
    cout << Rows[r].Cols[c] << " ";  // Printing the value in each column
   }
   cout << endl;  // New line after each row for clarity
  }
 }
};

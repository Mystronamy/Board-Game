/*-------------------------------------------
Program 7: Outlast the Baddies & Avoid the Abyss
Course: CS 211, Fall 2023, UIC
System: Advanced zyLab
Author: William Nguyen
------------------------------------------- */

#ifndef _GAMEBOARD_H
#define _GAMEBOARD_H

#include <cstdlib>

#include <iostream>

#include <string>

#include <ctime>

#include <stdexcept>

#include "boardcell.h"

#include "grid.h"

using namespace std;

class GameBoard {
  private: Grid < BoardCell * > board;
  size_t numRows;
  size_t numCols;
  size_t HeroRow; // Hero's position row
  size_t HeroCol; // Hero's position column
  int numMonsters;
  int numSuperMonsters;
  int numAbysses;
  int numBats;
  bool wonGame; // false, unless the Hero reached the exit successfully

  public:
    /* default constructor */
    GameBoard() {
      numMonsters = 4;
      numSuperMonsters = 2;
      numAbysses = 50;
      numBats = 2;
      wonGame = false;

      this -> numRows = 15;
      this -> numCols = 40;

      Grid < BoardCell * > boardnew(numRows, numCols);
      board = boardnew;

      blankBoard();
    }

  /* param constructor */
  GameBoard(size_t numRows, size_t numCols) {
    numMonsters = 4;
    numSuperMonsters = 2;
    numAbysses = 20;
    numBats = 3;
    wonGame = false;

    this -> numRows = numRows;
    this -> numCols = numCols;

    Grid < BoardCell * > boardnew(numRows, numCols);
    board = boardnew;

    blankBoard();
  }

  /* destructor */
  virtual~GameBoard() {
    for (size_t row = 0; row < board.numrows(); row++) {
      for (size_t col = 0; col < board.numcols(row); col++) {
        delete board(row, col);
      }
    }
  }

  void blankBoard() {
    for (size_t row = 0; row < board.numrows(); row++) {
      for (size_t col = 0; col < board.numcols(row); col++) {
        board(row, col) = new Nothing(row, col);
      }
    }
  }

  char getCellDisplay(size_t r, size_t c) {
    return board(r, c) -> display();
  }

  void setCell(BoardCell * myCell, size_t r, size_t c) {
    board(r, c) = myCell;
  }

  void freeCell(size_t r, size_t c) {
    delete board(r, c);
  }

  // fills board with by randomly placing...
  //  - Hero (H) in the first three columns
  //  - EscapeLadder (*) in last three columns
  //  - 3 vertical Walls (+), each 1/2 of board height, in middle segment
  //  - Abyss cells (#), quantity set by numAbysses, in middle segment
  //  - Baddies [Monsters (m), Super Monsters (M), & Bats (~)] in middle segment;
  //    number of Baddies set by numMonsters, numSuperMonsters, & numBats
  void setupBoard(int seed) {
    srand(seed);
    size_t r, c;

    r = rand() % numRows;
    c = rand() % 3;
    delete board(r, c);
    board(r, c) = new Hero(r, c);
    HeroRow = r;
    HeroCol = c;

    r = rand() % numRows;
    c = numCols - 1 - (rand() % 3);
    delete board(r, c);
    board(r, c) = new EscapeLadder(r, c);

    int sizeMid = numCols - 6;

    c = 3 + (rand() % sizeMid);
    for (r = 0; r < numRows / 2; ++r) {
      delete board(r, c);
      board(r, c) = new Wall(r, c);
    }
    size_t topc = c;

    while (c == topc || c == topc - 1 || c == topc + 1) {
      c = 3 + (rand() % sizeMid);
    }
    for (r = numRows - 1; r > numRows / 2; --r) {
      delete board(r, c);
      board(r, c) = new Wall(r, c);
    }
    size_t botc = c;

    while (c == topc || c == topc - 1 || c == topc + 1 || c == botc || c == botc - 1 || c == botc + 1) {
      c = 3 + (rand() % sizeMid);
    }
    for (r = numRows / 4; r < 3 * numRows / 4; ++r) {
      delete board(r, c);
      board(r, c) = new Wall(r, c);
    }

    for (int i = 0; i < numMonsters; ++i) {
      r = rand() % numRows;
      c = 3 + (rand() % sizeMid);
      while (board(r, c) -> display() != ' ') {
        r = rand() % numRows;
        c = 3 + (rand() % sizeMid);
      }
      delete board(r, c);
      board(r, c) = new Monster(r, c);
      board(r, c) -> setPower(1);
    }

    for (int i = 0; i < numSuperMonsters; ++i) {
      r = rand() % numRows;
      c = 3 + (rand() % sizeMid);
      while (board(r, c) -> display() != ' ') {
        r = rand() % numRows;
        c = 3 + (rand() % sizeMid);
      }
      delete board(r, c);
      board(r, c) = new Monster(r, c);
      board(r, c) -> setPower(2);
    }

    for (int i = 0; i < numBats; ++i) {
      r = rand() % numRows;
      c = 3 + (rand() % sizeMid);
      while (board(r, c) -> display() != ' ') {
        r = rand() % numRows;
        c = 3 + (rand() % sizeMid);
      }
      delete board(r, c);
      board(r, c) = new Bat(r, c);
    }

    for (int i = 0; i < numAbysses; ++i) {
      r = rand() % numRows;
      c = 3 + (rand() % sizeMid);
      while (board(r, c) -> display() != ' ') {
        r = rand() % numRows;
        c = 3 + (rand() % sizeMid);
      }
      delete board(r, c);
      board(r, c) = new Abyss(r, c);
    }
  }

  // neatly displaying the game board 
  void display() {
    cout << '-';
    for (size_t col = 0; col < board.numcols(0); col++) {
      cout << '-';
    }
    cout << '-';
    cout << endl;
    for (size_t row = 0; row < board.numrows(); row++) {
      cout << '|';
      for (size_t col = 0; col < board.numcols(row); col++) {
        cout << board(row, col) -> display();
      }
      cout << '|';
      cout << endl;
    }
    cout << '-';
    for (size_t col = 0; col < board.numcols(0); col++) {
      cout << '-';
    }
    cout << '-';
    cout << endl;

  }

  bool getWonGame() {
    return wonGame;
  }

  // distributing total number of monsters so that 
  //  ~1/3 of num are Super Monsters (M), and
  //  ~2/3 of num are Regular Monsters (m)
  void setNumMonsters(int num) {
    numSuperMonsters = num / 3;
    numMonsters = num - numSuperMonsters;
  }

  void setNumAbysses(int num) {
    numAbysses = num;
  }

  void setNumBats(int num) {
    numBats = num;
  }

  size_t getNumRows() {
    return numRows;
  }

  size_t getNumCols() {
    return numCols;
  }

  // getHeroPosition: Retrieves the current position of the hero
  // Parameters: 
  //   row - reference to size_t for storing the hero's row position
  //   col - reference to size_t for storing the hero's column position
  void getHeroPosition(size_t & row, size_t & col) {
    // TODO: Implement the logic to retrieve the hero's position
    // Currently, it assigns the global variables HeroRow and HeroCol to the parameters
    row = HeroRow; // modify/remove this line
    col = HeroCol; // modify/remove this line
  }

  // setHeroPosition: Sets the position of the hero
  // Parameters: 
  //   row - size_t representing the new row position for the hero
  //   col - size_t representing the new column position for the hero
  void setHeroPosition(size_t row, size_t col) {
    // Sets the global variables HeroRow and HeroCol to the new position values
    HeroRow = row;
    HeroCol = col;
  }

  // findHero: Searches the game board for the hero's position
  // Updates the global variables HeroRow and HeroCol with the found position,
  // or sets them to -1 if the hero is not found
  void findHero() {
    // Iterate over each cell of the board to find the hero
    for (size_t row = 0; row < numRows; row++) {
      for (size_t col = 0; col < numCols; col++) {
        // Check if the current cell has the hero
        if (board(row, col) -> display() == 'H') {
          HeroRow = row; // Update hero's row
          HeroCol = col; // Update hero's column
          return; // Exit the function once the hero is found
        }
      }
    }

    // Set HeroRow and HeroCol to -1 if the hero is not found
    HeroRow = -1;
    HeroCol = -1;
  }

  // isDiagonalMove: Checks if the given move is a diagonal move
  // Parameters:
  //   move - char representing the move command
  // Returns: 
  //   bool - true if the move is diagonal ('q', 'e', 'z', 'c'), false otherwise
  bool isDiagonalMove(char move) {
    return move == 'q' || move == 'e' || move == 'z' || move == 'c';
  }

  // removeHeroFromBoard: Removes the hero from the current position on the board
  // It frees the current cell and sets it to a 'Nothing' object.
  // Also resets HeroRow and HeroCol to -1
  void removeHeroFromBoard() {
    // Check if the hero's position is within the board bounds
    if (HeroRow >= 0 && HeroRow < numRows && HeroCol >= 0 && HeroCol < numCols) {
      freeCell(HeroRow, HeroCol); // Free the cell where the hero is currently located
      board(HeroRow, HeroCol) = new Nothing(HeroRow, HeroCol); // Replace the hero's cell with a 'Nothing' object
    }
    // Reset the hero's position
    HeroRow = -1;
    HeroCol = -1;
  }

  // makeMoves: Executes the hero's move and updates the board state
  // Parameters:
  //   HeroNextMove - char representing the next move of the hero
  // Returns:
  //   bool - true if the move is successfully executed, false otherwise
  bool makeMoves(char HeroNextMove) {
    // Reset the 'moved' status of all cells on the board
    for (size_t r = 0; r < numRows; r++) {
      for (size_t c = 0; c < numCols; c++) {
        board(r, c) -> setMoved(false);
      }
    }

    // Prepare for the hero's next move
    size_t newR, newC;
    board(HeroRow, HeroCol) -> setNextMove(HeroNextMove);
    board(HeroRow, HeroCol) -> attemptMoveTo(newR, newC, HeroRow, HeroCol);

    // Check and handle if the hero's new row position is out-of-bounds
    try {
      if (newR < 0 || newR >= numRows) {
        throw runtime_error("Hero trying to move out-of-bounds");
      }
    } catch (const runtime_error & e) {
      cout << e.what() << endl; // Output the exception message
      newR = HeroRow; // Reset new row position to current row position
    }

    // Check and handle if the hero's new column position is out-of-bounds
    try {
      if (newC < 0 || newC >= numCols) {
        throw runtime_error("Hero trying to move out-of-bounds");
      }
    } catch (const runtime_error & e) {
      cout << e.what() << endl; // Output the exception message
      newC = HeroCol; // Reset new column position to current column position
    }

    // Determine the type of cell at the new hero position
    char cellType = board(newR, newC) -> display();

    // Check for wall collision and handle diagonal moves
    try {
      if (cellType == '+') {
        // If the move is diagonal and leads to a wall, adjust the position
        if (isDiagonalMove(HeroNextMove)) {
          newC = HeroCol;
          if (board(newR, newC) -> display() == '+') {
            newR = HeroRow;
          }
        } else {
          // If it's not a diagonal move, it's an invalid move into a wall
          throw runtime_error("Hero trying to move into a wall");
        }
      }
    } catch (const runtime_error & e) {
      cout << e.what() << endl; // Output the exception message
      // Reset hero's position if a wall collision occurs
      newR = HeroRow;
      newC = HeroCol;
    }

    // Check for hazards or escape conditions
    try {
      // Check if the hero encounters a hazard
      if (cellType == '#' || cellType == 'm' || cellType == 'M' || cellType == '~') {
        throw runtime_error("Hero encounters a hazard");
      }

      // Check if the hero has reached an escape point
      if (cellType == '*') {
        throw runtime_error("Hero escaped");
      }
    } catch (const runtime_error & e) {
      cout << e.what() << endl; // Output the exception message
      // Handle different scenarios based on the encountered cell type
      if (cellType == '#' || cellType == 'm' || cellType == 'M' || cellType == '~') {
        removeHeroFromBoard();
        return false; // End the game due to hazard encounter
      }

      if (cellType == '*') {
        removeHeroFromBoard();
        wonGame = true; // Set game win status and end the game
        return false; 
      }

      return false; // End the game for other scenarios
    }

    // Update the board if the hero moves to a new position
    if (newR != HeroRow || newC != HeroCol) {
      freeCell(HeroRow, HeroCol); // Free the cell where the hero was previously located
      board(HeroRow, HeroCol) = new Nothing(HeroRow, HeroCol); // Replace the old hero cell with a 'Nothing' object
      freeCell(newR, newC); // Free the new cell for the hero
      board(newR, newC) = new Hero(newR, newC); // Place the hero in the new cell
      HeroRow = newR; // Update the hero's row position
      HeroCol = newC; // Update the hero's column position
    }

    // Store the last known position of the hero
    size_t lastHeroRow = HeroRow;
    size_t lastHeroCol = HeroCol;

    // Iterate over each cell of the board to update baddie positions
    for (size_t r = 0; r < numRows; r++) {
      for (size_t c = 0; c < numCols; c++) {
        BoardCell * currentCell = board(r, c);
        // Check if the cell contains a baddie and if it has not moved yet
        if (currentCell -> isBaddie() && !currentCell -> getMoved()) {
          size_t baddieNewR, baddieNewC;
          currentCell -> attemptMoveTo(baddieNewR, baddieNewC, lastHeroRow, lastHeroCol);
          // Check and handle baddie movement out-of-bounds
          try {
            if (baddieNewR >= numRows || baddieNewC >= numCols) {
              throw out_of_range("Baddie trying to move out-of-bounds");
            }

            BoardCell * targetCell = board(baddieNewR, baddieNewC);

            // Check if the baddie is trying to move into a barrier or exit
            if (targetCell -> isBarrier() || targetCell -> isExit()) {
              if (newC != c && board(baddieNewR, c) -> isSpace()) {
                newC = c; // Adjust the baddie's new column position if possible
              } else {
                throw runtime_error("Baddie trying to move into a barrier");
              }
            }

            // Check if the target cell is a hole
            if (targetCell -> isHole()) {
              throw runtime_error("Baddie falls into Abyss");
            }

            // Check if the target cell is the hero
            if (targetCell -> isHero()) {
              throw runtime_error("Baddie captures Hero");
            }

            // Check if the target cell already has another baddie
            if (targetCell -> isBaddie()) {
              throw runtime_error("Baddie collides with another Baddie");
            }

            // Move the baddie to the new cell if it's different from the current cell
            if (targetCell != currentCell) {
              freeCell(baddieNewR, baddieNewC); // Free the cell to which the baddie is moving
              board(r, c) = new Nothing(r, c); // Replace the baddie's current cell with a 'Nothing' object
              board(baddieNewR, baddieNewC) = currentCell; // Move the baddie to the new cell
              currentCell -> setPos(baddieNewR, baddieNewC); // Update the baddie's position to the new coordinates
            }
          } catch (const out_of_range & e) {
            // Handle baddie movement out-of-bounds
            baddieNewR = r;
            baddieNewC = c;
          } catch (const runtime_error & e) {
            // Check the specific exception message
            if (e.what() == string("Baddie falls into Abyss")) {
              freeCell(r, c); // Free the current cell occupied by the baddie
              board(r, c) = new Nothing(r, c); // Replace the baddie cell with a 'Nothing' object
            } else if (e.what() == string("Baddie captures Hero")) {
              removeHeroFromBoard(); // Remove the hero from the board due to capture
              board(r, c) = new Nothing(r, c); // Replace the hero's cell with a 'Nothing' object
              board(baddieNewR, baddieNewC) = currentCell; // Place the baddie in the hero's former position
              currentCell -> setPos(baddieNewR, baddieNewC); // Update the baddie's position
              currentCell -> setMoved(true); // Set the baddie's moved status to true
            } else {
              baddieNewR = r; // Reset the baddie's new row position
              baddieNewC = c; // Reset the baddie's new column position
            }
          }

          // Mark the current cell as moved
          currentCell -> setMoved(true);
        }
      }
    }

    // Re-find the hero's position at the end of all movements
    findHero();

    // Return false if the hero is not on the board anymore, true otherwise
    if (HeroRow == -1 && HeroCol == -1) {
      return false;
    }
    return true;
  }

};

#endif //_GAMEBOARD_H
/*-------------------------------------------
Program 7: Outlast the Baddies & Avoid the Abyss
Course: CS 211, Fall 2023, UIC
System: Advanced zyLab
Author: William Nguyen
------------------------------------------- */

#ifndef _BOARDCELL_H
#define _BOARDCELL_H

using namespace std;

// First, the BoardCell abstract base class 

class BoardCell {

  public: BoardCell() {} // default contstructor (do nothing)
  virtual~BoardCell() {} // destructor (do nothing)

  virtual char display() = 0; // pure virtual function; this is an abstract base class

  // attemptMoveTo: Base class function to determine the new position of a board cell
  // Parameters:
  //   newR - reference to size_t for the new row position
  //   newC - reference to size_t for the new column position
  //   hRow - size_t for the hero's current row position (not used in base class)
  //   hCol - size_t for the hero's current column position (not used in base class)
  virtual void attemptMoveTo(size_t & newR, size_t & newC, size_t hRow, size_t hCol) {
    // The default action for a board cell is to stay put
    // Set the new position (newR, newC) to be the same as the current position (myRow, myCol)
    newR = myRow; // Current row position of the cell
    newC = myCol; // Current column position of the cell
  }

  virtual bool isHero() {
    return false;
  }
  virtual bool isBaddie() {
    return false;
  }
  virtual bool isSpace() {
    return false;
  }
  virtual bool isBarrier() {
    return false;
  }
  virtual bool isHole() {
    return false;
  }
  virtual bool isExit() {
    return false;
  }
  virtual bool isStatic() {
    return true;
  }

  virtual void setPower(int num) {}
  virtual int getPower() {
    return 1;
  }
  virtual void setNextMove(char inChar) {}

  void setMoved(bool m) {
    moved = m;
  }
  bool getMoved() {
    return moved;
  }
  void setRow(size_t r) {
    myRow = r;
  }
  size_t getRow() {
    return myRow;
  }
  void setCol(size_t c) {
    myCol = c;
  }
  size_t getCol() {
    return myCol;
  }
  void setPos(size_t r, size_t c) {
    setRow(r);
    setCol(c);
  }

  private: size_t myRow; // current row for this board cell in a 2D grid
  size_t myCol; // current column for this board cell in a 2D grid
  bool moved; // true = this board cell already moved in the current round

}; // BoardCell (abstract base class)

// Then, all the derived classes

class Hero: public BoardCell {

  public: Hero(size_t r, size_t c) {
    setRow(r);
    setCol(c);
  }

  virtual bool isHero() {
    return true;
  }
  virtual bool isStatic() {
    return false;
  }
  virtual char display() {
    return 'H';
  }

  virtual void setNextMove(char inChar) {
    nextMove = inChar;
  }

  // attemptMoveTo: Determines the Hero's new position based on the nextMove
  // Parameters:
  //   newR - reference to size_t for the new row position
  //   newC - reference to size_t for the new column position
  //   hRow - size_t for the hero's current row position (unused in this context)
  //   hCol - size_t for the hero's current column position (unused in this context)
  virtual void attemptMoveTo(size_t & newR, size_t & newC, size_t hRow, size_t hCol) {
    // Initialize newR and newC with the current position of the hero
    newR = getRow(); // Current row position
    newC = getCol(); // Current column position

    // Determine the hero's next position based on the nextMove character
    switch (nextMove) {
    case 'q':
      if (newR > 0 && newC > 0) {
        newR--;
        newC--;
      }
      break; // Move up and left
    case 'w':
      if (newR > 0) {
        newR--;
      }
      break; // Move up
    case 'e':
      if (newR > 0) {
        newR--;
        newC++;
      }
      break; // Move up and right
    case 'a':
      if (newC > 0) {
        newC--;
      }
      break; // Move left
    case 'd':
      newC++;
      break; // Move right
    case 'z':
      newR++;
      newC--;
      break; // Move down and left
    case 'x':
      newR++;
      break; // Move down
    case 'c':
      newR++;
      newC++;
      break; // Move down and right
    default:
      break; // Stay in the same position
    }
  }

  private: char nextMove;

}; // Hero

class Monster: public BoardCell {

  public: Monster(size_t r, size_t c) {
    setRow(r);
    setCol(c);
    power = 1;
  }

  virtual bool isBaddie() {
    return true;
  }
  virtual bool isStatic() {
    return false;
  }

  virtual char display() {
    if (power == 2) {
      return 'M'; // Super Monster
    } else {
      return 'm'; // Monster
    }
  }

  virtual void setPower(int num) {
    power = num;
  }

  virtual int getPower() {
    return power;
  }

  // attemptMoveTo: Calculates the new position for a move towards the hero
  // Parameters:
  //   newR - reference to size_t representing the new row position
  //   newC - reference to size_t representing the new column position
  //   hRow - size_t representing the hero's current row position
  //   hCol - size_t representing the hero's current column position
  virtual void attemptMoveTo(size_t & newR, size_t & newC, size_t hRow, size_t hCol) {
    // Initialize newR and newC with the current position
    newR = getRow(); // Current row position of the object
    newC = getCol(); // Current column position of the object

    // Calculate the vertical and horizontal moves based on the power and hero's position
    int verticalMove = power * ((hRow > newR) - (hRow < newR));
    int horizontalMove = power * ((hCol > newC) - (hCol < newC));

    // Update newR and newC with the calculated move towards the hero
    newR += verticalMove;
    newC += horizontalMove;
  }
  private: int power; // power = 1 for Monster, power = 2 for SuperMonster

}; // Monster

class Bat: public BoardCell {
  public: Bat(size_t r, size_t c) {
    setRow(r);
    setCol(c);
  }
  virtual bool isBaddie() {
    return true;
  }
  virtual bool isStatic() {
    return false;
  }
  virtual char display() {
    return '~';
  }

  // attemptMoveTo: Determines the Bat's new position
  // Parameters:
  //   newR - reference to size_t for the new row position
  //   newC - reference to size_t for the new column position
  //   hRow - size_t for the hero's current row position (not used in this context)
  //   hCol - size_t for the hero's current column position
  virtual void attemptMoveTo(size_t & newR, size_t & newC, size_t hRow, size_t hCol) {
    // The Bat's movement logic:
    // It always navigates to the hero's column but stays in its current row

    newR = getRow(); // Retain the Bat's current row position
    newC = hCol; // Move to the column where the hero is located
  }

}; // Bat

class Abyss: public BoardCell {
  public: Abyss(size_t r, size_t c) {
    setRow(r);
    setCol(c);
  }
  virtual char display() {
    return '#';
  }
  virtual bool isHole() {
    return true;
  }
}; // Abyss

class Wall: public BoardCell {
  public: Wall(size_t r, size_t c) {
    setRow(r);
    setCol(c);
  }
  virtual char display() {
    return '+';
  }
  virtual bool isBarrier() {
    return true;
  }
}; // Wall

class Nothing: public BoardCell {
  public: Nothing(size_t r, size_t c) {
    setRow(r);
    setCol(c);
  }
  virtual char display() {
    return ' ';
  }
  virtual bool isSpace() {
    return true;
  }
}; // Nothing

class EscapeLadder: public BoardCell {
  public: EscapeLadder(size_t r, size_t c) {
    setRow(r);
    setCol(c);
  }
  virtual char display() {
    return '*';
  }
  virtual bool isExit() {
    return true;
  }
}; // EscapeLadder

#endif //_BOARDCELL_H
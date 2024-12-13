#ifndef _4X4_TICTACTOE_H
#define _4X4_TICTACTOE_H

#include "BoardGame_Classes.h"
#include <iostream>
#include <iomanip>
#include <cctype>
using namespace std;
int startX, startY;
template <typename T>
class X_O_Board : public Board<T> {
public:
    X_O_Board();
    bool update_board(int x, int y, T symbol);
    void display_board();
    bool is_win();
    bool is_draw();
    bool game_is_over();
    T get_cell(int x, int y) const {
        return this->board[x][y];
    }

private:
    bool is_adjacent(int x1, int y1, int x2, int y2);
    template <typename U>
    friend class X_O_Game_Random_Player;
};

template <typename T>
class X_O_Human_Player : public Player<T> {
public:
    X_O_Human_Player(string name, T symbol,X_O_Board<T>* boardPtr);
    void getmove(int& x, int& y);

private:
    X_O_Board<T>* boardPtr;
};

template <typename T>
class X_O_Game_Random_Player : public RandomPlayer<T> {
public:
    X_O_Game_Random_Player(T symbol, X_O_Board<T>* boardPtr); // Constructor takes the board
    void getmove(int& x, int& y);

private:
    X_O_Board<T>* boardPtr;  // Pointer to the shared board
};


// Board constructor
template <typename T>
X_O_Board<T>::X_O_Board() {
    this->rows = this->columns = 4; // Set grid to 4x4
    this->board = new T*[this->rows];
    for (int i = 0; i < this->rows; i++) {
        this->board[i] = new T[this->columns];
        for (int j = 0; j < this->columns; j++) {
            this->board[i][j] = 0; // Initialize all cells to empty
        }
    }
    // Initializing specific positions for tokens
    this->board[0][0] = 'O';
    this->board[0][1] = 'X';
    this->board[0][2] = 'O';
    this->board[0][3] = 'X';
    this->board[3][0] = 'X';
    this->board[3][1] = 'O';
    this->board[3][2] = 'X';
    this->board[3][3] = 'O';
    this->n_moves = 0;
}

// Update the board with a player's move
template <typename T>
bool X_O_Board<T>::update_board(int x, int y, T symbol) {
    if (x >= 0 && x < this->rows && y >= 0 && y < this->columns && this->board[x][y] == 0) {
                if (this->board[startX][startY] == symbol && is_adjacent(startX, startY, x, y)) {
                    this->board[startX][startY] = 0;  // Clear the old position
                    this->board[x][y] = symbol; // Place the token at the new position
                    this->n_moves++;
                    return true;
                }
    }
    cout << "Invalid move! Ensure the move is adjacent and the target cell is empty.\n";
    return false;
}

// Display the board
template <typename T>
void X_O_Board<T>::display_board() {
    cout << "\nCurrent Board State:\n";
    for (int i = 0; i < this->rows; i++) {
        for (int j = 0; j < this->columns; j++) {
            cout << "| " << (this->board[i][j] ? this->board[i][j] : ' ') << " ";
        }
        cout << "|\n";
        cout << string(this->columns * 4, '-') << "\n";
    }
}

// Check if a move is to an adjacent position
template <typename T>
bool X_O_Board<T>::is_adjacent(int x1, int y1, int x2, int y2) {
    return (abs(x1 - x2) + abs(y1 - y2)) == 1; // Manhattan distance check
}

// Check win condition
template <typename T>
bool X_O_Board<T>::is_win() {
    for (int i = 0; i < this->rows; i++) {
        for (int j = 0; j < this->columns; j++) {
            T symbol = this->board[i][j];
            if (symbol != 0) {
                // Horizontal, Vertical, Diagonal checks
                if ((j <= this->columns - 3 && this->board[i][j + 1] == symbol && this->board[i][j + 2] == symbol) ||
                    (i <= this->rows - 3 && this->board[i + 1][j] == symbol && this->board[i + 2][j] == symbol) ||
                    (i <= this->rows - 3 && j <= this->columns - 3 && this->board[i + 1][j + 1] == symbol && this->board[i + 2][j + 2] == symbol) ||
                    (i >= 2 && j <= this->columns - 3 && this->board[i - 1][j + 1] == symbol && this->board[i - 2][j + 2] == symbol)) {
                    cout << "Player " << symbol << " wins!\n";
                    return true;
                }
            }
        }
    }
    return false;
}

// Check draw condition
template <typename T>
bool X_O_Board<T>::is_draw() {
    return this->n_moves >= 16; // All cells are filled
}

// Check if the game is over
template <typename T>
bool X_O_Board<T>::game_is_over() {
    return is_win() || is_draw();
}

// Human player constructor
template <typename T>
X_O_Human_Player<T>::X_O_Human_Player(string name, T symbol, X_O_Board<T>* boardPtr) : Player<T>(name, symbol),boardPtr(boardPtr) {}

// Get move from human player
template <typename T>
void X_O_Human_Player<T>::getmove(int& x, int& y) {

     // Variables to store the starting position

    // Step 1: Select the token to move
    while (true) {
        cout << "Enter the position of the token you want to move, " << this->name
             << " (" << this->symbol << ") [0-3 for both]: ";
        cin >> startX >> startY;

        // Validate input
        if (cin.fail() || startX < 0 || startX >= 4 || startY < 0 || startY >= 4 ||
            this->boardPtr->get_cell(startX, startY) != this->symbol) {
            cin.clear(); // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
            cout << "Invalid position! You must select your own token.\n";
            } else {
                break; // Valid input
            }
    }

    // Step 2: Select the destination for the move
    while (true) {
        cout << "Enter the row and column for your move, " << this->name
             << " (" << this->symbol << ") [0-3 for both]: ";
        cin >> x >> y;

        // Validate input
        if (cin.fail() || x < 0 || x >= 4 || y < 0 || y >= 4 ||
            this->boardPtr->get_cell(x, y) != 0) {
            cin.clear(); // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
            cout << "Invalid move! Please select an empty position within bounds.\n";
            } else if (abs(x - startX) > 1 || abs(y - startY) > 1) {
                // Additional movement rule: restrict to adjacent cells
                cout << "Invalid move! You can only move to an adjacent cell.\n";
            } else {
                break; // Valid destination
            }
    }
}

// Random computer player constructor
template <typename T>
X_O_Game_Random_Player<T>::X_O_Game_Random_Player(T symbol, X_O_Board<T>* boardPtr)
    : RandomPlayer<T>(symbol), boardPtr(boardPtr) {}

// Get move from random player
template <typename T>
void X_O_Game_Random_Player<T>::getmove(int& x, int& y) {
    while (true) {
        x = rand() % 4;
        y = rand() % 4;
        startX = rand() % 4;
        startY = rand() % 4;

        // Check if the selected cell is empty
        if (this->boardPtr->get_cell(x,y) == 0 &&
            this->boardPtr->is_adjacent(startX, startY, x, y)) {
            break; // Valid move
        }
    }
}

#endif //_4X4_TICTACTOE_H

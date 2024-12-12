#ifndef _4X4_TICTACTOE_H
#define _4X4_TICTACTOE_H

#include "BoardGame_Classes.h"
#include <iostream>
#include <iomanip>
#include <cctype>
using namespace std;

template <typename T>
class X_O_Board : public Board<T> {
public:
    X_O_Board();
    bool update_board(int x, int y, T symbol);
    void display_board();
    bool is_win();
    bool is_draw();
    bool game_is_over();

private:
    bool is_adjacent(int x1, int y1, int x2, int y2);
};

template <typename T>
class X_O_Human_Player : public Player<T> {
public:
    X_O_Human_Player(string name, T symbol);
    void getmove(int& x, int& y);
};

template <typename T>
class X_O_Game_Random_Player : public RandomPlayer<T> {
public:
    X_O_Game_Random_Player(T symbol);
    void getmove(int& x, int& y);
};

// Board constructor
template <typename T>
X_O_Board<T>::X_O_Board() {
    this->rows = this->columns = 4;  // Set grid to 4x4
    this->board = new T*[this->rows];
    for (int i = 0; i < this->rows; i++) {
        this->board[i] = new T[this->columns];
        for (int j = 0; j < this->columns; j++) {
            this->board[i][j] = 0;
        }
    }
    this->board[0][0] = 'O';
    this->board[0][1] = 'X';
    this->board[3][2] = 'O';
    this->board[3][3] = 'X';
    this->n_moves = 0;
}

// Update board with move
template <typename T>
bool X_O_Board<T>::update_board(int x, int y, T mark) {
    if (x >= 0 && x < this->rows && y >= 0 && y < this->columns && this->board[x][y] == 0) {
        for (int i = 0; i < this->rows; i++) {
            for (int j = 0; j < this->columns; j++) {
                if (this->board[i][j] == mark && is_adjacent(i, j, x, y)) {
                    this->board[i][j] = 0;  // Remove the token from the old position
                    this->board[x][y] = mark;  // Place the token in the new position
                    this->n_moves++;
                    return true;
                }
            }
        }
    }
    return false;
}

// Display board
template <typename T>
void X_O_Board<T>::display_board() {
    for (int i = 0; i < this->rows; i++) {
        cout << "\n| ";
        for (int j = 0; j < this->columns; j++) {
            cout << "(" << i << "," << j << ")";
            cout << setw(2) << this->board[i][j] << " |";
        }
        cout << "\n---------------------------------";
    }
    cout << endl;
}

// Check if the move is to an adjacent position
template <typename T>
bool X_O_Board<T>::is_adjacent(int x1, int y1, int x2, int y2) {
    return (abs(x1 - x2) + abs(y1 - y2)) == 1;
}

// Check win condition
template <typename T>
bool X_O_Board<T>::is_win() {
    for (int i = 0; i < this->rows; i++) {
        for (int j = 0; j < this->columns; j++) {
            T symbol = this->board[i][j];
            if (symbol != 0) {
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
    return (this->n_moves >= 16);
}

// Check if the game is over
template <typename T>
bool X_O_Board<T>::game_is_over() {
    return is_win() || is_draw();
}

// Human player constructor
template <typename T>
X_O_Human_Player<T>::X_O_Human_Player(string name, T symbol) : Player<T>(name, symbol) {}

// Get move from human player
template <typename T>
void X_O_Human_Player<T>::getmove(int& x, int& y) {
    cout << "Enter row and column for your move, " << this->name << " (" << this->symbol << "): ";
    cin >> x >> y;
}

// Random computer player constructor
template <typename T>
X_O_Game_Random_Player<T>::X_O_Game_Random_Player(T symbol) : RandomPlayer<T>(symbol) {}

// Get move from random player
template <typename T>
void X_O_Game_Random_Player<T>::getmove(int& x, int& y) {
    x = rand() % this->dimension;
    y = rand() % this->dimension;
}

#endif //_4X4_TICTACTOE_H

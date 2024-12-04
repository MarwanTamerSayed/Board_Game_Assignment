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
    int count_threes(T symbol);
};

template <typename T>
class X_O_Human_Player : public Player<T> {
public:
    X_O_Human_Player(string name, T symbol);
    void getmove(int& x, int& y);
};

template <typename T>
class X_O_Random_Player : public RandomPlayer<T> {
public:
    X_O_Random_Player(T symbol);
    void getmove(int& x, int& y) override;
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
    this->board[0][2] = 'O';
    this->board[0][3] = 'X';
    this->board[3][0] = 'X';
    this->board[3][1] = 'O';
    this->board[3][2] = 'X';
    this->board[3][3] = 'O';
    this->n_moves = 0;
}

// Update board with move
template <typename T>
bool X_O_Board<T>::update_board(int x, int y, T mark) {
    if (x >= 0 && x < this->rows && y >= 0 && y < this->columns && this->board[x][y] == 0) {
        this->board[x][y] = mark;
        this->n_moves++;
        return true;
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

// Count aligned tokens
template <typename T>
int X_O_Board<T>::count_threes(T symbol) {
    int count = 0;

    // Check rows
    for (int i = 0; i < this->rows; i++) {
        for (int j = 0; j <= this->columns - 3; j++) {
            if (this->board[i][j] == symbol && this->board[i][j + 1] == symbol && this->board[i][j + 2] == symbol) {
                count++;
            }
        }
    }

    // Check columns
    for (int j = 0; j < this->columns; j++) {
        for (int i = 0; i <= this->rows - 3; i++) {
            if (this->board[i][j] == symbol && this->board[i + 1][j] == symbol && this->board[i + 2][j] == symbol) {
                count++;
            }
        }
    }

    // Check diagonals (top-left to bottom-right)
    for (int i = 0; i <= this->rows - 3; i++) {
        for (int j = 0; j <= this->columns - 3; j++) {
            if (this->board[i][j] == symbol && this->board[i + 1][j + 1] == symbol && this->board[i + 2][j + 2] == symbol) {
                count++;
            }
        }
    }

    // Check diagonals (top-right to bottom-left)
    for (int i = 0; i <= this->rows - 3; i++) {
        for (int j = 2; j < this->columns; j++) {
            if (this->board[i][j] == symbol && this->board[i + 1][j - 1] == symbol && this->board[i + 2][j - 2] == symbol) {
                count++;
            }
        }
    }

    return count;
}

// Check win condition
template <typename T>
bool X_O_Board<T>::is_win() {
    int x_score = count_threes('X');
    int o_score = count_threes('O');
    if (x_score > 0) {
        cout << "Player X wins!\n";
        return true;
    } else if (o_score > 0) {
        cout << "Player O wins!\n";
        return true;
    }
    return false;
}
template <typename T>
bool X_O_Board<T>::is_draw() {
    int xscore = count_threes('X');
    int oscore = count_threes('O');
    return (oscore == xscore && this->n_moves == 16);
}
// Check if the game is over
template <typename T>
bool X_O_Board<T>::game_is_over() {
    if (is_win() || this->n_moves >= 16) {
        cout << "Game over!\n";
        return true;
    }
    return false;
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
X_O_Random_Player<T>::X_O_Random_Player(T symbol) : RandomPlayer<T>(symbol) {
    this->dimension = 4;
    this->name = "Random Computer Player";
    srand(static_cast<unsigned int>(time(0)));  // Seed the random number generator
}

// Get move from random player
template <typename T>
void X_O_Random_Player<T>::getmove(int& x, int& y) {
    x = rand() % this->dimension;
    y = rand() % this->dimension;
}

#endif //_4X4_TICTACTOE_H
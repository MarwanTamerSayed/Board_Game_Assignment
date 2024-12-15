#ifndef _FOUR_IN_A_ROW_H
#define _FOUR_IN_A_ROW_H

#include "BoardGame_Classes.h"
#include <iostream>
#include <vector>
#include <limits>
using namespace std;

template <typename T>
class FourInARowMinimaxPlayer; // Forward declaration
template <typename T>
class FourInARowBoard : public Board<T> {
public:
    // Constructor to initialize the board
    FourInARowBoard() {
        this->rows = 6;
        this->columns = 7;
        this->n_moves = 0;
        this->board = new T*[this->rows];
        for (int i = 0; i < this->rows; i++) {
            this->board[i] = new T[this->columns];
            std::fill(this->board[i], this->board[i] + this->columns, '.'); // Empty cell represented by '.'
        }
    }

    ~FourInARowBoard() {
        for (int i = 0; i < this->rows; i++) {
            delete[] this->board[i];
        }
        delete[] this->board;
    }

    int get_first_empty_row(int col);
    int get_columns() {
        return this->columns;
    }

    int get_nmoves() {
        return this->n_moves;
    }
    T& at(int row, int col) {
        if (row < 0 || row >= this->rows || col < 0 || col >= this->columns) {
            throw std::out_of_range("Index out of bounds");
        }
        return this->board[row][col];
    }

    int getRows() const {
        return this->rows;  // Accessing the protected member of the base class
    }

    // Override the getColumns function
    int getColumns() const {
        return this->cols;  // Accessing the protected member of the base class
    }
    void dec_n() {
        this->n_moves--;
    }
    void inc_n() {
        this->n_moves++;
    }
    int get_rows() {
        return this->rows;
    }
    int getWidth() {
        return get_columns();
    }
    int getHeight() {
        return get_rows();
    }
    T getCell(int row, int col) {
        return at(row, col);
    }

    bool update_board(int x, int y, T symbol) override {
        if (y < 0 || y >= this->columns) return false;
        for (int i = this->rows - 1; i >= 0; i--) {
            if (this->board[i][y] == '.') {
                this->board[i][y] = symbol;
                this->n_moves++;
                return true;
            }
        }
        return false; // Column is full
    }

    void display_board() override {
        for (int i = 0; i < this->rows; i++) {
            for (int j = 0; j < this->columns; j++) {
                std::cout << this->board[i][j] << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

    bool is_win() override {
        // Check horizontal
        for (int i = 0; i < this->rows; i++) {
            for (int j = 0; j <= this->columns - 4; j++) {
                if (this->board[i][j] != '.' &&
                    this->board[i][j] == this->board[i][j + 1] &&
                    this->board[i][j] == this->board[i][j + 2] &&
                    this->board[i][j] == this->board[i][j + 3]) {
                    return true;
                }
            }
        }

        // Check vertical
        for (int i = 0; i <= this->rows - 4; i++) {
            for (int j = 0; j < this->columns; j++) {
                if (this->board[i][j] != '.' &&
                    this->board[i][j] == this->board[i + 1][j] &&
                    this->board[i][j] == this->board[i + 2][j] &&
                    this->board[i][j] == this->board[i + 3][j]) {
                    return true;
                }
            }
        }

        // Check diagonals (\)
        for (int i = 0; i <= this->rows - 4; i++) {
            for (int j = 0; j <= this->columns - 4; j++) {
                if (this->board[i][j] != '.' &&
                    this->board[i][j] == this->board[i + 1][j + 1] &&
                    this->board[i][j] == this->board[i + 2][j + 2] &&
                    this->board[i][j] == this->board[i + 3][j + 3]) {
                    return true;
                }
            }
        }

        // Check diagonals (/)
        for (int i = 3; i < this->rows; i++) {
            for (int j = 0; j <= this->columns - 4; j++) {
                if (this->board[i][j] != '.' &&
                    this->board[i][j] == this->board[i - 1][j + 1] &&
                    this->board[i][j] == this->board[i - 2][j + 2] &&
                    this->board[i][j] == this->board[i - 3][j + 3]) {
                    return true;
                }
            }
        }

        return false;
    }

    bool is_draw() override {
        return this->n_moves == this->rows * this->columns && !is_win();
    }

    bool game_is_over() override {
        return is_win() || is_draw();
    }
    friend class FourInARowMinimaxPlayer<T>;
};

template <typename T>
int FourInARowBoard<T>::get_first_empty_row(int col) {
    for (int i = this->rows - 1; i >= 0; i--) {
        if (this->board[i][col] == '.') {
            return i;
        }
    }
    return -1; // No empty row in the column
}


template <typename T>
class FourInRowPlayer : public Player<T> {
public:
    // Constructor to initialize the player with a name and symbol
    FourInRowPlayer(std::string name, T symbol) : Player<T>(name, symbol) {}

    // Get move from the player (human input)
    void getmove(int& x, int& y) override {
        std::cout << this->getname() << " (" << this->getsymbol() << "), enter your move (column 0-6): ";
        std::cin >> y; // Human player inputs the column number
        x = 0;         // Row is determined by the board logic
    }
};




// Specialized RandomPlayer for Four-in-a-Row
template <typename T>
class FourInARowRandomPlayer : public RandomPlayer<T> {
public:
    FourInARowRandomPlayer(T symbol) : RandomPlayer<T>(symbol) {}

    void getmove(int& x, int& y) override {
        x = 0; // Not used in this game
        y = rand() % 7; // Generate a random column
    }
};










#endif //_FOUR_IN_A_ROW_H

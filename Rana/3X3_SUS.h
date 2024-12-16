#ifndef _SUS_GAME_H
#define _SUS_GAME_H

#include "BoardGame_Classes.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <cctype>
#include <limits>

using namespace std;

template <typename T>
class SUS_Board : public Board<T> {
    static int countP1;
    static int countP2;
    T currentplayersymbol;
    vector<pair<int, int>> visited_cells; // Stores visited positions to avoid re-counting

public:
    SUS_Board();
    ~SUS_Board();
    void switchPlayer();
    bool update_board(int x, int y, T symbol) override;
    void display_board() override;
    bool is_win() override;
    bool is_draw() override;
    bool game_is_over() override;
    T getCell(int x, int y) const {
        return this->board[x][y];
    }

private:
    void count_SUS_S();
    bool already_visited(vector<pair<int, int>> positions);
    void add_visited(vector<pair<int, int>> positions);
    template<typename U>
    friend class SUS_Player;
};

// Static variable definitions
template<typename T>
int SUS_Board<T>::countP1 = 0;

template<typename T>
int SUS_Board<T>::countP2 = 0;

template <typename T>
SUS_Board<T>::SUS_Board() {
    this->rows = this->columns = 3;
    this->board = new T*[this->rows];
    for (int i = 0; i < this->rows; i++) {
        this->board[i] = new T[this->columns]{};
    }
    this->currentplayersymbol = 'S';
    this->n_moves = 0;
}

template <typename T>
SUS_Board<T>::~SUS_Board() {
    for (int i = 0; i < this->rows; i++) {
        delete[] this->board[i];
    }
    delete[] this->board;
}

template <typename T>
void SUS_Board<T>::switchPlayer() {
    currentplayersymbol = (currentplayersymbol == 'S') ? 'U' : 'S';
}

template <typename T>
bool SUS_Board<T>::update_board(int x, int y, T mark) {
    if (x >= 0 && x < this->rows && y >= 0 && y < this->columns && this->board[x][y] == 0) {
        this->board[x][y] = toupper(mark);
        this->n_moves++;
        return true;
    }
    return false;
}

template <typename T>
void SUS_Board<T>::display_board() {
    for (int i = 0; i < this->rows; i++) {
        cout << "\n| ";
        for (int j = 0; j < this->columns; j++) {
            cout << (this->board[i][j] ? this->board[i][j] : '.') << " |";
        }
        cout << "\n" << string(this->columns * 4, '-');
    }
    cout << endl;
}

// Helper function to check if positions have already been visited
template <typename T>
bool SUS_Board<T>::already_visited(vector<pair<int, int>> positions) {
    return all_of(positions.begin(), positions.end(), [&](pair<int, int> pos) {
        return find(visited_cells.begin(), visited_cells.end(), pos) != visited_cells.end();
    });
}

// Add positions to visited list
template <typename T>
void SUS_Board<T>::add_visited(vector<pair<int, int>> positions) {
    for (auto& pos : positions) {
        visited_cells.push_back(pos);
    }
}

// Count all unique "S-U-S" sequences
template <typename T>
void SUS_Board<T>::count_SUS_S() {
    // Check rows and columns
    for (int i = 0; i < 3; i++) {
        // Rows
        if (this->board[i][0] == 'S' && this->board[i][1] == 'U' && this->board[i][2] == 'S') {
            vector<pair<int, int>> positions = {{i, 0}, {i, 1}, {i, 2}};
            if (!already_visited(positions)) {
                add_visited(positions);
                (currentplayersymbol == 'S') ? countP1++ : countP2++;
            }
        }
        // Columns
        if (this->board[0][i] == 'S' && this->board[1][i] == 'U' && this->board[2][i] == 'S') {
            vector<pair<int, int>> positions = {{0, i}, {1, i}, {2, i}};
            if (!already_visited(positions)) {
                add_visited(positions);
                (currentplayersymbol == 'S') ? countP1++ : countP2++;
            }
        }
    }
    // Check diagonals
    vector<pair<int, int>> diag1 = {{0, 0}, {1, 1}, {2, 2}};
    if (this->board[0][0] == 'S' && this->board[1][1] == 'U' && this->board[2][2] == 'S' && !already_visited(diag1)) {
        add_visited(diag1);
        (currentplayersymbol == 'S') ? countP1++ : countP2++;
    }

    vector<pair<int, int>> diag2 = {{0, 2}, {1, 1}, {2, 0}};
    if (this->board[0][2] == 'S' && this->board[1][1] == 'U' && this->board[2][0] == 'S' && !already_visited(diag2)) {
        add_visited(diag2);
        (currentplayersymbol == 'S') ? countP1++ : countP2++;
    }

    switchPlayer();
}

template <typename T>
bool SUS_Board<T>::is_win() {
    count_SUS_S();
    if (this->n_moves == 9 || visited_cells.size() == 18) { // All positions filled or no more SUS
        cout << "Score:\nPlayer 1: " << countP1 << "\nPlayer 2: " << countP2 << endl;
        return true;
    }
    return false;
}

template <typename T>
bool SUS_Board<T>::is_draw() {
    return (this->n_moves == 9 && !is_win());
}

template <typename T>
bool SUS_Board<T>::game_is_over() {
    return is_win();
}

template <typename T>
class SUS_Player : public Player<T> {
public:
    SUS_Player(string name, T symbol,SUS_Board<T>* boardPtr);
    void getmove(int& x, int& y) override;
private:
    SUS_Board<T>* boardPtr;
};

template <typename T>
SUS_Player<T>::SUS_Player(string name, T symbol,SUS_Board<T>* boardPtr) : Player<T>(name, symbol),boardPtr(boardPtr) {}

template <typename T>
void SUS_Player<T>::getmove(int& x, int& y) {
    while (true) {
        cout << "\nIt's your turn, " << this->name << " (" << this->symbol << ")!" << endl;
        cout << "Please enter your move (row and column) in the format 'x y' (0-2): ";

        // Check if the user input is valid
        if (cin >> x >> y) {
            // Check if the move is within bounds
            if (x < 0 || x > 2 || y < 0 || y > 2) {
                cout << "Invalid input. Please enter coordinates within the range (0-2) for both row and column." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear input buffer
            }
            // Check if the chosen cell is already occupied
            else if (this->boardPtr->getCell(x, y) != 0) {
                cout << "This cell is already occupied. Please choose an empty cell." << endl;
            } else {
                break; // Valid move
            }
        } else {
            cout << "Invalid input. Please enter numeric values for both row and column." << endl;
            cin.clear(); // Clear error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore invalid input
        }
    }
}

template <typename T>
class SUS_Random_Player : public RandomPlayer<T> {
public:
    SUS_Random_Player(T symbol);
    void getmove(int& x, int& y) override;
};

template <typename T>
SUS_Random_Player<T>::SUS_Random_Player(T symbol) : RandomPlayer<T>(symbol) {
    this->dimension = 3;
    this->name = "Random Computer Player";
    srand(static_cast<unsigned int>(time(0)));
}

template <typename T>
void SUS_Random_Player<T>::getmove(int& x, int& y) {
    x = rand() % this->dimension;
    y = rand() % this->dimension;

}

#endif









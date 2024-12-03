#include "BoardGame_Classes.h"
#include <cctype>
#include <iostream>  // For input/output stream
#include <cstdlib>   // For rand(), srand()
#include <ctime>     // For time()

// Pyramid Board Class Definition
template <typename T>
class PyramidBoard : public Board<T> {
public:
    PyramidBoard();
    bool update_board(int x, int y, T symbol);
    void display_board();
    bool is_win();
    bool is_draw();
    bool game_is_over();
};

// Player Classes
template <typename T>
class X_O_Player : public Player<T> {
public:
    X_O_Player(string name, T symbol);
    void getmove(int& x, int& y);
};

template <typename T>
class X_O_Random_Player : public RandomPlayer<T> {
public:
    X_O_Random_Player(T symbol);
    void getmove(int& x, int& y);
};

//=============== Implementation ===========================//

// Constructor for PyramidBoard
template <typename T>
PyramidBoard<T>::PyramidBoard() {
    this->rows = 3;
    this->columns = 5;
    this->board = new char*[this->rows];
    for (int i = 0; i < this->rows; i++) {
        this->board[i] = new char[this->columns];
        for (int j = 0; j < this->columns; j++) {
            this->board[i][j] = 0;  // Initialize all cells to 0
        }
    }
    this->n_moves = 0;
}

// Update Board Function
template <typename T>
bool PyramidBoard<T>::update_board(int x, int y, T mark) {
    // Check if the move is valid for pyramid structure
    if (((x == 0 && y == 0) ||
         (x == 1 && y >= 0 && y < 3) ||
         (x == 2 && y >= 0 && y < 5)) &&
        (this->board[x][y] == 0 || mark == 0)) {

        if (mark == 0) {  // Undo the move
            this->n_moves--;
            this->board[x][y] = 0;
        } else {  // Place the mark
            this->n_moves++;
            this->board[x][y] = toupper(mark);  // Convert mark to uppercase
        }
        return true;
    }
    return false;
}

// Display Board Function
template <typename T>
void PyramidBoard<T>::display_board() {
    // Array representing the number of columns for each row
    int row_sizes[] = {1, 3, 5};  // Pyramid shape
    int max_columns = row_sizes[2];  // Maximum cells in the last row for alignment

    // Loop through each row to display the board
    for (int i = 0; i < 3; i++) {
        // Calculate the spaces to center each row
        int padding = (max_columns - row_sizes[i]) * 3;  // 3 spaces per missing cell
        cout << string(padding, ' ');  // Add padding before the row

        // Loop through each column in the row
        for (int j = 0; j < row_sizes[i]; j++) {
            cout << "(" << i << "," << j << ")";
            cout << " " << this->board[i][j] << " ";
        }
        cout << endl;  // Move to the next row
    }
}

// Check for a winner
template <typename T>
bool PyramidBoard<T>::is_win() {
    // Check horizontal wins (rows)
    // and vertical win 
    if (
        (this->board[2][0] != 0 && this->board[2][0] == this->board[2][1] && this->board[2][1] == this->board[2][2]) || // Bottom row: cells 0, 1, 2
        (this->board[2][1] != 0 && this->board[2][1] == this->board[2][2] && this->board[2][2] == this->board[2][3]) || // Bottom row: cells 1, 2, 3
        (this->board[2][2] != 0 && this->board[2][2] == this->board[2][3] && this->board[2][3] == this->board[2][4]) || // Bottom row: cells 2, 3, 4
        (this->board[1][0] != 0 && this->board[1][0] == this->board[1][1] && this->board[1][1] == this->board[1][2]) ||   // Middle row: cells 0, 1, 2
        (this->board[0][0] != 0 && this->board[0][0] == this->board[1][1] && this->board[1][1] == this->board[2][2])) {
        return true;
    }

    // Check diagonal wins
    if ((this->board[0][0] != 0 && this->board[0][0] == this->board[1][0] && this->board[1][0] == this->board[2][0])||
        (this->board[0][0] != 0 && this->board[0][0] == this->board[1][2] && this->board[1][2] == this->board[2][4])) 
        {
        return true;
    }



    return false;
}

// Check for a draw
template <typename T>
bool PyramidBoard<T>::is_draw() {
    return (this->n_moves == 9 && !is_win());
}

// Check if the game is over (win or draw)
template <typename T>
bool PyramidBoard<T>::game_is_over() {
    return is_win() || is_draw();
}

//====================== Player Class =========================

// Constructor for X_O_Player
template <typename T>
X_O_Player<T>::X_O_Player(string name, T symbol) : Player<T>(name, symbol) {}

// Get the player's move
template <typename T>
void X_O_Player<T>::getmove(int& x, int& y) {
    cout << "\nPlease enter your move x and y separated by spaces: ";
    cout << "\nEnter the available indices on screen  ";
    cin >> x >> y;
}

// Constructor for X_O_Random_Player
template <typename T>
X_O_Random_Player<T>::X_O_Random_Player(T symbol) : RandomPlayer<T>(symbol) {
    this->dimension = 3;
    this->name = "Random Computer Player";
    srand(static_cast<unsigned int>(time(0)));  // Seed the random number generator
}

// Get the random player's move
template <typename T>
void X_O_Random_Player<T>::getmove(int& x, int& y) {
    int row_sizes[] = {1, 3, 5};  // Array representing row sizes

    // Randomly select a row
    x = rand() % this->dimension;  // Random row from 0 to 2

    // Select a column within the selected row
    y = rand() % row_sizes[x];  // Random column from 0 to row_sizes[x]
}

// ======================================================

// Revise the code again

// some notes 
// 1 ) if and endif 
// 2 ) ai player doesn't work well
// 3 ) make the UI good 
// 4 ) the validation but not now

#ifndef WORD_TIC_H
#define WORD_TIC_H



#include "BoardGame_Classes.h"
#include <iostream>
#include <fstream>
#include <cstdlib>   
#include <ctime>
#include <iomanip>
#include <cctype>  

using namespace std ;

template <typename T>
class WordBoard : public Board<T> {
public:
    WordBoard();
    bool update_board(int x, int y, T symbol);
    void display_board();
    bool is_win();
    bool is_draw();
    bool game_is_over();
    bool isWordInFile(const string& word);
};


// Player Classes
template <typename T>
class WordPlayer : public Player<T> {
public:
    WordPlayer(string name, T symbol);
    void getmove(int& x, int& y);
};


template <typename T>
class Word_Random_Player : public RandomPlayer<T> {
public:
    Word_Random_Player(T symbol);
    void getmove(int& x, int& y);
};


//=============== Implementation ===========================//


// Constructor for WordBoard
template <typename T>
WordBoard<T>::WordBoard() {
    this->rows = this->columns = 3;
    this->board = new char*[this->rows];
    for (int i = 0; i < this->rows; i++) {
        this->board[i] = new char[this->columns];
        for (int j = 0; j < this->columns; j++) {
            this->board[i][j] = 0;
        }
    }
    this->n_moves = 0;
}


template <typename T>
bool WordBoard<T>::update_board(int x, int y, T mark) {
    // Only update if move is valid
    if (!(x < 0 || x >= this->rows || y < 0 || y >= this->columns) && (this->board[x][y] == 0|| mark == 0)) {
        if (mark == 0){
            this->n_moves--;
            this->board[x][y] = 0;
        }
        else {
            this->n_moves++;
            this->board[x][y] = toupper(mark);
        }

        return true;
    }
    return false;
}



// Display the board and the pieces on it
template <typename T>
void WordBoard<T>::display_board() {
    for (int i = 0; i < this->rows; i++) {
        cout << "\n| ";
        for (int j = 0; j < this->columns; j++) {
            cout << "(" << i << "," << j << ")";
            cout << setw(2) << this->board[i][j] << " |";
        }
        cout << "\n-----------------------------";
    }
    cout << endl;
}

template <typename T>
bool WordBoard <T> :: isWordInFile(const string& word) {
    ifstream file("dic.txt");
    string line;
    if (file.is_open()) {
        while (getline(file, line)) {
            if (line == word) { // Case-sensitive match
                file.close();
                return true;
            }
        }
        file.close();
    }
    return false;
}

template<typename T> 
bool WordBoard<T>::is_win() {
    for (int i = 0; i < this->rows; i++) {
        string word;

        // Check rows
        word = string(1, this->board[i][0]) + this->board[i][1] + this->board[i][2];
        if (this->board[i][0] != 0 && isWordInFile(word)) {
            return true;
        }
        word = string(1, this->board[i][2]) + this->board[i][1] + this->board[i][0];
        if (this->board[i][2] != 0 && isWordInFile(word)) {
            return true;
        }
    }

    for (int i = 0; i < this->columns; i++) {
        string word;
        string word2;

        // Check columns
        word = string(1, this->board[0][i]) + this->board[1][i] + this->board[2][i];
        if (this->board[0][i] != 0 && isWordInFile(word)) {
            return true;
        }
        // Check columns
        word2 = string(1, this->board[2][i]) + this->board[1][i] + this->board[0][i];
        if (this->board[2][i] != 0 && isWordInFile(word2)) {
            return true;
        }
    }

    // Check diagonals
    string diag1 = string(1, this->board[0][0]) + this->board[1][1] + this->board[2][2];
    if (this->board[0][0] != 0 && isWordInFile(diag1)) {
        return true;
    }
    string diag2 = string(1, this->board[2][2]) + this->board[1][1] + this->board[0][0];
    if (this->board[2][2] != 0 && isWordInFile(diag2)) {
        return true;
    }



    string diag3 = string(1, this->board[0][2]) + this->board[1][1] + this->board[2][0];
    if (this->board[0][2] != 0 && isWordInFile(diag3)) {
        return true;
    }
    string diag4 = string(1, this->board[2][0]) + this->board[1][1] + this->board[0][2];
    if (this->board[0][2] != 0 && isWordInFile(diag4)) {
        return true;
    }

    return false;
}


// Return true if 9 moves are done and no winner
template <typename T>
bool WordBoard<T>::is_draw() {
    return (this->n_moves == 9 && !is_win());
}


template <typename T>
bool WordBoard<T>::game_is_over() {
    return is_win() || is_draw();
}

// ====================Players================================


// Constructor for WordPlayer
template <typename T>
WordPlayer<T>::WordPlayer(string name, T symbol) : Player<T>(name, symbol) {}

template <typename T>
void WordPlayer<T>::getmove(int& x, int& y) {
    cout << "\nPlease enter your move x and y (0 to 2) separated by spaces: ";
    cin >> x >> y;
}

// Constructor for X_O_Random_Player
template <typename T>
Word_Random_Player<T>::Word_Random_Player(T symbol) : RandomPlayer<T>(symbol) {
    this->dimension = 3;
    this->name = "Random Computer Player";
    srand(static_cast<unsigned int>(time(0)));  // Seed the random number generator
}

template <typename T>
void Word_Random_Player<T>::getmove(int& x, int& y) {
    x = rand() % this->dimension;  // Random number between 0 and 2
    y = rand() % this->dimension;
}


#endif // WORD_TIC_H

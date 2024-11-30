#ifndef NUMERICAL_TIC_TAC_TOE_H
#define NUMERICAL_TIC_TAC_TOE_H

#include "BoardGame_Classes.h"
#include <iostream>
#include <iomanip>
#include <unordered_set>

using namespace std;

template<typename T>
class NumericalBoard : public Board<int> {
public:
    NumericalBoard(int size = 3) {
        rows = columns = size;
        board = new int*[rows];
        for (int i = 0; i < rows; ++i) {
            board[i] = new int[columns]();
        }
    }

    ~NumericalBoard() {
        for (int i = 0; i < rows; ++i) {
            delete[] board[i];
        }
        delete[] board;
    }

    bool update_board(int x, int y, int symbol) override {
        if (x < 0 || x >= rows || y < 0 || y >= columns || board[x][y] != 0) {
            return false;
        }
        board[x][y] = symbol;
        n_moves++;
        return true;
    }

    void display_board() override {
        cout << "Board State:\n";
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < columns; ++j) {
                cout << setw(4) << (board[i][j] == 0 ? "-" : to_string(board[i][j]));
            }
            cout << "\n";
        }
    }

    bool is_win() override {
        for (int i = 0; i < rows; ++i) {
            if (check_sum(board[i][0], board[i][1], board[i][2])) return true;
        }

        for (int j = 0; j < columns; ++j) {
            if (check_sum(board[0][j], board[1][j], board[2][j])) return true;
        }

        if (check_sum(board[0][0], board[1][1], board[2][2]) ||
            check_sum(board[0][2], board[1][1], board[2][0])) {
            return true;
        }
        return false;
    }

    bool is_draw() override {
        return n_moves == rows * columns && !is_win();
    }

    bool game_is_over() override {
        return is_win() || is_draw();
    }

private:
    bool check_sum(int a, int b, int c) {
        return (a != 0 && b != 0 && c != 0 && a + b + c == 15);
    }
};

template<typename T>
class HumanPlayer : public Player<int> {
private:
    unordered_set<int> availableNumbers;

public:
    HumanPlayer(string name, unordered_set<int> numbers) : Player<int>(name, 0), availableNumbers(numbers) {}

    void getmove(int& x, int& y) override {
        displayAvailableNumbers();
        int num;
        cout << this->name << ", choose a number from the available set: ";
        cin >> num;

        while (availableNumbers.find(num) == availableNumbers.end()) {
            cout << "Invalid number. Choose a valid number: ";
            cin >> num;
        }

        cout << "Enter your move (row and column): ";
        cin >> x >> y;

        this->symbol = num;  // Set the chosen number as the player's symbol
        availableNumbers.erase(num); // Remove the number from the available set
    }

    void displayAvailableNumbers() {
        cout << this->name << "'s available numbers: { ";
        for (int num : availableNumbers) {
            cout << num << " ";
        }
        cout << "}\n";
    }
};


template<typename T>
class NumericalRandomPlayer : public RandomPlayer<int> {
private:
    unordered_set<int> availableNumbers;

public:
    NumericalRandomPlayer(unordered_set<int> numbers) : RandomPlayer<int>(0), availableNumbers(numbers) {}

    void getmove(int& x, int& y) override {
        x = rand() % 3;
        y = rand() % 3;

        int num = *availableNumbers.begin();  // Take the first available number
        this->symbol = num;
        availableNumbers.erase(num);
        cout << "Computer chooses: " << num << "\n";
    }
};

#endif // NUMERICAL_TIC_TAC_TOE_H

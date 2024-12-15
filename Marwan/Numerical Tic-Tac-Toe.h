#ifndef NUMERICAL_TIC_TAC_TOE_H
#define NUMERICAL_TIC_TAC_TOE_H


#include "BoardGame_Classes.h"
#include <iostream>
#include <iomanip>
#include <unordered_set>
#include <utility> // For std::pair
#include <cstdlib>
#include <set>
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
        // Check if the move is within bounds
        if (x < 0 || x >= rows || y < 0 || y >= columns) {
            cout << "Invalid move: (" << x << ", " << y << ") is out of bounds." << endl;
            return false;
        }

        // Check if the spot is already occupied
        if (board[x][y] != 0) {
            cout << "Invalid move: (" << x << ", " << y << ") is already occupied." << endl;
            return false;
        }

        // Update the board with the player's move
        board[x][y] = symbol;
        n_moves++; // Increment the move counter

        // Print the updated board state for debugging purposes
        cout << "Selected move: (" << x << ", " << y << ")" << endl;
        display_board();  // Assuming you have a method to print the board for debugging

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

    // Grant friendship to the player classes
    template<typename U>
    friend class HumanPlayer;

    template<typename U>
    friend class NumericalRandomPlayer;

    template<typename U>
    friend class NumericalSmartPlayer;
    template<typename U>
    friend class NumericalMinMaxPlayer;

private:
    bool check_sum(int a, int b, int c) {
        return (a != 0 && b != 0 && c != 0 && a + b + c == 15);
    }

    int** board;
    int rows;
    int columns;
    int n_moves = 0;
};





// Custom hash function for pair<int, int>
struct hash_pair {
    size_t operator()(const pair<int, int>& p) const {
        return hash<int>()(p.first) ^ (hash<int>()(p.second) << 1);
    }
};


// Global shared sets to track used positions and numbers
unordered_set<pair<int, int>, hash_pair> globalUsedPositions;
unordered_set<int> globalUsedNumbers;

// Function to reset the global sets
void resetGlobalState() {
    globalUsedPositions.clear();
    globalUsedNumbers.clear();
}


// HumanPlayer class
template<typename T>
class HumanPlayer : public Player<int> {
private:
    unordered_set<int> availableNumbers;

public:
    HumanPlayer(string name, unordered_set<int> numbers) : Player<int>(name, 0), availableNumbers(numbers) {}

    void getmove(int& x, int& y) override {
        NumericalBoard<int> boardInstance;
        getmove(x, y, boardInstance);
    }

    void getmove(int& x, int& y, NumericalBoard<int>& boardInstance) {
        displayAvailableNumbers();

        int num;
        cout << this->name << ", choose a number from the available set: ";
        cin >> num;

        // Ensure the chosen number is in the available set and not globally used
        while (availableNumbers.find(num) == availableNumbers.end() || globalUsedNumbers.find(num) != globalUsedNumbers.end()) {
            cout << "Invalid number. Choose a valid number: ";
            cin >> num;
        }

        cout << "Enter your move (row and column): ";
        cin >> x >> y;

        // Validate the chosen cell
        while (x < 0 || x >= boardInstance.rows || y < 0 || y >= boardInstance.columns ||
               boardInstance.board[x][y] != 0 || globalUsedPositions.find({x, y}) != globalUsedPositions.end()) {
            cout << "Invalid move. Choose an empty cell within the board: ";
            cin >> x >> y;
        }

        // Assign the symbol
        this->symbol = num;
        boardInstance.board[x][y] = num;
        availableNumbers.erase(num);
        globalUsedNumbers.insert(num); // Update globally used numbers
        globalUsedPositions.insert({x, y}); // Update globally used positions
    }

    void displayAvailableNumbers() {
        cout << this->name << "'s available numbers: { ";
        for (int num : availableNumbers) {
            if (globalUsedNumbers.find(num) == globalUsedNumbers.end()) {
                cout << num << " ";
            }
        }
        cout << "}\n";
    }
};

// NumericalRandomPlayer class
template<typename T>
class NumericalRandomPlayer : public RandomPlayer<int> {
private:
    unordered_set<int> availableNumbers;

public:
    NumericalRandomPlayer(unordered_set<int> numbers) : RandomPlayer<int>(0), availableNumbers(numbers) {}

    void getmove(int& x, int& y) override {
        NumericalBoard<int> boardInstance;
        getmove(x, y, boardInstance);
    }

    void getmove(int& x, int& y, NumericalBoard<int>& boardInstance) {
        vector<pair<int, int>> validMoves;

        // Iterate through the board and add valid positions
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (boardInstance.board[i][j] == 0 && globalUsedPositions.find({i, j}) == globalUsedPositions.end()) {
                    validMoves.push_back({i, j});
                }
            }
        }

        if (!validMoves.empty()) {
            // Randomly select a valid position
            int moveIndex = rand() % validMoves.size();
            x = validMoves[moveIndex].first;
            y = validMoves[moveIndex].second;

            // Select a valid number
            int num = -1;
            for (int availableNum : availableNumbers) {
                if (globalUsedNumbers.find(availableNum) == globalUsedNumbers.end()) {
                    num = availableNum;
                    break;
                }
            }

            if (num != -1) {
                // Update the symbol, board, and global sets
                this->symbol = num;
                boardInstance.board[x][y] = num;
                availableNumbers.erase(num);
                globalUsedNumbers.insert(num); // Update globally used numbers
                globalUsedPositions.insert({x, y}); // Update globally used positions

                cout << "Computer chooses: " << num << " at (" << x << ", " << y << ")\n";
            } else {
                cout << "No valid numbers available for the computer. Game Over.\n";
            }
        } else {
            cout << "No valid moves available. Game Over.\n";
        }
    }
};


struct pair_hash {
    template <typename T1, typename T2>
    std::size_t operator ()(const std::pair<T1, T2>& p) const {
        auto h1 = std::hash<T1>{}(p.first);
        auto h2 = std::hash<T2>{}(p.second);

        // Combine the two hash values
        return h1 ^ (h2 << 1); // Shift and xor to combine the hash values
    }
};


#endif // NUMERICAL_TIC_TAC_TOE_H



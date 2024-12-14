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



template <typename T>
class FourInARowMinimaxPlayer : public Player<T> {
private:
    FourInARowBoard<T>* currentBoard = nullptr;
    bool moveMade = false;
    bool boardFull = false; // Tracks whether the board is full

public:
    FourInARowMinimaxPlayer(std::string name, T symbol) : Player<T>(name, symbol) {

    }

    bool isBoardFull(FourInARowBoard<char>* board) {
        for (int col = 0; col < board->getWidth(); ++col) {
            if (board->getCell(0, col) == '.') { // Check top row of each column
                return false; // Found an empty cell
            }
        }
        return true; // All columns are full
    }



    void set_board(FourInARowBoard<T>* board) {
        currentBoard = board;
        moveMade = false;  // Reset move flag when board is set
        std::cout << "Minimax Board set for player: " << this->getname()
                  << " with symbol " << this->getsymbol() << std::endl;
    }

    // Simplified evaluation function
    int evaluate_board(FourInARowBoard<T>& board, T currentSymbol, T opponentSymbol) {
        int score = 0;

        // Prefer moves that create winning opportunities or block opponent
        for (int col = 0; col < board.get_columns(); col++) {
            int row = board.get_first_empty_row(col);
            if (row != -1) {
                // Temporarily place the symbol
                board.update_board(row, col, currentSymbol);

                // Check if this move creates a win
                if (board.is_win()) {
                    board.update_board(row, col, '.');
                    return 1000;
                }

                // Check blocking opponent's potential win
                board.update_board(row, col, opponentSymbol);
                if (board.is_win()) {
                    board.update_board(row, col, '.');
                    return -1000;
                }

                // Undo the move
                board.update_board(row, col, '.');
            }
        }

        return score;
    }

    // Simplified minimax
    int minimax(FourInARowBoard<T>& board, int depth, bool isMaximizing,
            T currentSymbol, T opponentSymbol) {
        if (board.is_win()) {
            return isMaximizing ? -1000 + depth : 1000 - depth;
        }

        if (board.is_draw() || depth == 4) {
            return evaluate_board(board, currentSymbol, opponentSymbol);
        }

        int bestScore = isMaximizing ? std::numeric_limits<int>::min()
                                     : std::numeric_limits<int>::max();

        for (int col = 0; col < board.get_columns(); col++) {
            int row = board.get_first_empty_row(col);
            if (row != -1) {
                // Temporary move using direct cell access
                board.at(row, col) = isMaximizing ? currentSymbol : opponentSymbol;
                board.inc_n(); // Increment moves count

                int score = minimax(board, depth + 1, !isMaximizing,
                                    currentSymbol, opponentSymbol);

                // Undo move
                board.at(row, col) = '.';
                board.dec_n(); // Decrement moves count

                if (isMaximizing) {
                    bestScore = std::max(bestScore, score);
                } else {
                    bestScore = std::min(bestScore, score);
                }
            }
        }

        return bestScore;
    }

    void getmove(int& x, int& y) override {
    // Ensure the board is initialized
    if (!currentBoard) {
        std::cerr << "Critical Error: No board set for AI player" << std::endl;
        throw std::runtime_error("Board not initialized for AI player");
    }

    // Check for available columns
    std::vector<int> availableColumns;
    for (int col = 0; col < currentBoard->get_columns(); col++) {
        if (currentBoard->get_first_empty_row(col) != -1) {
            availableColumns.push_back(col);
        }
    }

    // If no moves are available, terminate
    if (availableColumns.empty()) {
        x = -1;
        y = -1;
        std::cerr << "No valid moves available for AI player " << this->getname() << std::endl;
        return;
    }

    // Set opponent's symbol
    T opponentSymbol = (this->getsymbol() == 'X') ? 'O' : 'X';
    int bestScore = std::numeric_limits<int>::min();
    std::vector<int> bestMoves;

    // Evaluate each available column
    for (int col : availableColumns) {
        int row = currentBoard->get_first_empty_row(col);

        // Simulate move
        currentBoard->at(row, col) = this->getsymbol();
        currentBoard->inc_n();

        // Evaluate using Minimax
        int score = minimax(*currentBoard, 0, false, this->getsymbol(), opponentSymbol);

        // Undo simulated move
        currentBoard->at(row, col) = '.';
        currentBoard->dec_n();

        // Track the best score
        if (score > bestScore) {
            bestScore = score;
            bestMoves.clear();
            bestMoves.push_back(col);
        } else if (score == bestScore) {
            bestMoves.push_back(col);
        }
    }

    // Randomly choose among the best moves
    if (!bestMoves.empty()) {
        y = bestMoves[rand() % bestMoves.size()];
    } else {
        y = availableColumns[rand() % availableColumns.size()];
    }
    x = currentBoard->get_first_empty_row(y);

    std::cout << "AI Player " << this->getname()
              << " chose column: " << y
              << " at row: " << x << std::endl;
}



    // Method to reset the move flag for the next turn
    void reset_move_flag() {
        moveMade = false;
    }
};


template <typename T>
class FourInARowSmartPlayer : public Player<T> {
public:
    FourInARowSmartPlayer(std::string name, T symbol) : Player<T>(name, symbol) {}

    void getmove(int& x, int& y) override {
        if (!currentBoard) {
            std::cerr << "Critical Error: No board set for Smart Player" << std::endl;
            throw std::runtime_error("Board not initialized for Smart Player");
        }

        int bestColumn = -1;
        int maxScore = std::numeric_limits<int>::min();

        // Check if the AI can win or needs to block the opponent
        if (find_critical_move(bestColumn)) {
            y = bestColumn;
            x = get_first_empty_row(bestColumn);
            std::cout << "Smart Player " << this->getname() << " blocks or wins in column: " << y << std::endl;
            return;
        }

        // Iterate through all columns to determine the best move
        for (int col = 0; col < currentBoard->columns; ++col) {
            int row = get_first_empty_row(col);
            if (row != -1) {
                // Temporarily make a move
                currentBoard->board[row][col] = this->getsymbol();
                ++currentBoard->n_moves;

                // Evaluate the move
                int score = evaluate_move(row, col);

                // Undo the move
                currentBoard->board[row][col] = '.';
                --currentBoard->n_moves;

                // Track the best score and column
                if (score > maxScore) {
                    maxScore = score;
                    bestColumn = col;
                }
            }
        }

        if (bestColumn != -1) {
            y = bestColumn;
            x = get_first_empty_row(bestColumn);
        } else {
            std::cerr << "No valid moves available." << std::endl;
            x = y = -1; // No valid move
        }

        std::cout << "Smart Player " << this->getname() << " chooses column: " << y << std::endl;
    }

    void set_board(FourInARowBoard<T>* board) {
        currentBoard = board;
    }

private:
    FourInARowBoard<T>* currentBoard = nullptr;

    // Find the first empty row in a column
    int get_first_empty_row(int col) const {
        for (int row = currentBoard->rows - 1; row >= 0; --row) {
            if (currentBoard->board[row][col] == '.') {
                return row;
            }
        }
        return -1; // No empty row
    }

    // Check for critical moves (win or block)
    bool find_critical_move(int& column) {
        for (int col = 0; col < currentBoard->columns; ++col) {
            int row = get_first_empty_row(col);
            if (row != -1) {
                // Check for AI's win
                currentBoard->board[row][col] = this->getsymbol();
                if (is_winning_move(row, col, this->getsymbol())) {
                    currentBoard->board[row][col] = '.';
                    column = col;
                    return true;
                }
                currentBoard->board[row][col] = '.';

                // Check for opponent's win
                currentBoard->board[row][col] = get_opponent_symbol();
                if (is_winning_move(row, col, get_opponent_symbol())) {
                    currentBoard->board[row][col] = '.';
                    column = col;
                    return true;
                }
                currentBoard->board[row][col] = '.';
            }
        }
        return false;
    }

    // Evaluate the quality of a move
    int evaluate_move(int row, int col) {
        int score = 0;

        // Check horizontal, vertical, and diagonal lines
        score += evaluate_direction(row, col, 0, 1); // Horizontal
        score += evaluate_direction(row, col, 1, 0); // Vertical
        score += evaluate_direction(row, col, 1, 1); // Diagonal \
        score += evaluate_direction(row, col, 1, -1); // Diagonal /

        return score;
    }

    // Evaluate a specific direction
    int evaluate_direction(int row, int col, int dRow, int dCol) {
        int count = 0;
        int score = 0;

        for (int step = -3; step <= 3; ++step) {
            int r = row + step * dRow;
            int c = col + step * dCol;

            if (r >= 0 && r < currentBoard->rows && c >= 0 && c < currentBoard->columns) {
                if (currentBoard->board[r][c] == this->getsymbol()) {
                    ++count;
                } else if (currentBoard->board[r][c] == '.') {
                    continue;
                } else {
                    count = 0; // Opponent symbol breaks the streak
                }

                if (count == 4) { // Win condition
                    score += 1000;
                } else if (count == 3) {
                    score += 10; // Favorable position
                } else if (count == 2) {
                    score += 1; // Minor advantage
                }
            }
        }

        return score;
    }

    // Check if a move is winning
    bool is_winning_move(int row, int col, T symbol) {
        return (evaluate_direction(row, col, 0, 1) >= 1000 || // Horizontal
                evaluate_direction(row, col, 1, 0) >= 1000 || // Vertical
                evaluate_direction(row, col, 1, 1) >= 1000 || // Diagonal
                evaluate_direction(row, col, 1, -1) >= 1000); // Diagonal /
    }

    // Get opponent's symbol
    T get_opponent_symbol()  {
        return (this->getsymbol() == 'X') ? 'O' : 'X';
    }
};



#endif //_FOUR_IN_A_ROW_H

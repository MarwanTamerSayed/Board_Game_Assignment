#ifndef NUMERICAL_TIC_TAC_TOE_H
#define NUMERICAL_TIC_TAC_TOE_H


#include "BoardGame_Classes.h"
#include <iostream>
#include <iomanip>
#include <unordered_set>
#include <utility> // For std::pair
#include <cstdlib>
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

template<typename T>
class NumericalMinMaxPlayer : public Player<T> {
public:
    NumericalMinMaxPlayer(string name, int playerNumber)
        : Player<T>(name, 0), playerPosition(playerNumber) {
        if (playerPosition == 1) {
            availableNumbers = {1, 3, 5, 7, 9};
        } else {
            availableNumbers = {2, 4, 6, 8};
        }
    }

    void getmove(int& x, int& y) override {
    if (!this->boardPtr) {
        std::cout << "Error: Board pointer not set!" << std::endl;
        return;
    }

    // Cast the board pointer to NumericalBoard
    NumericalBoard<int>* numericalBoard = dynamic_cast<NumericalBoard<int>*>(this->boardPtr);
    if (!numericalBoard) {
        std::cout << "Error: Cannot cast board to NumericalBoard!" << std::endl;
        return;
    }

    std::cout << "AI's available numbers before move: ";
    for (int num : availableNumbers) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    // Clean up available numbers (check if used already)
    for (auto it = availableNumbers.begin(); it != availableNumbers.end();) {
        if (globalUsedNumbers.find(*it) != globalUsedNumbers.end()) {
            it = availableNumbers.erase(it);
        } else {
            ++it;
        }
    }

    std::cout << "AI's available numbers after cleaning: ";
    for (int num : availableNumbers) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    // Get the best move and validate
    std::pair<std::pair<int, int>, int> bestMove = getBestMove(*numericalBoard);
    x = bestMove.first.first;
    y = bestMove.first.second;

    // Ensure that the AI does not choose an already used number
    if (globalUsedNumbers.find(bestMove.second) != globalUsedNumbers.end()) {
        std::cout << "AI's selected number " << bestMove.second << " is already used! Skipping turn.\n";
        return;
    }

    // Ensure the move is valid (within bounds and position is empty)
    if (x >= 0 && y >= 0 && numericalBoard->board[x][y] == 0) {
        // Apply the move to the board
        std::cout << "AI chooses: " << bestMove.second << " at (" << x << ", " << y << ")\n";
        numericalBoard->update_board(x, y, bestMove.second); // Ensure update_board works correctly
        availableNumbers.erase(bestMove.second);            // Remove the used number
        globalUsedNumbers.insert(bestMove.second);          // Mark the number as used
        globalUsedPositions.insert({x, y});                  // Track the used position
    } else {
        // If invalid, print an error and skip the move
        std::cout << "Invalid move: Unable to find a valid move for AI. Skipping turn.\n";
    }
}

private:
    unordered_set<int> availableNumbers;
    unordered_set<int> globalUsedNumbers;
    unordered_set<pair<int, int>, pair_hash> globalUsedPositions;
    int playerPosition;

    // Function to calculate the best move using Minimax
    std::pair<std::pair<int, int>, int> getBestMove(NumericalBoard<int>& boardInstance) {
        cout << "Evaluating best move for " << this->name << endl;

        // Log the current board state
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                cout << boardInstance.board[i][j] << " ";
            }
            cout << endl;
        }

        // Log available numbers
        cout << "Available numbers: ";
        for (int num : availableNumbers) {
            cout << num << " ";
        }
        cout << endl;

        // Step 1: Populate the availableMoves container
        std::vector<std::pair<int, int>> availableMoves;

        for (int i = 0; i < boardInstance.rows; i++) {
            for (int j = 0; j < boardInstance.columns; j++) {
                if (boardInstance.board[i][j] == 0) {  // Assuming 0 means an empty spot
                    availableMoves.push_back(std::make_pair(i, j));  // Add available position
                }
            }
        }

        // Step 2: Check the available moves and evaluate
        for (const auto& move : availableMoves) {
            std::cout << "Considering move: " << move.first << ", " << move.second << std::endl;
            // Additional checks...
        }

        // Step 3: Return the first valid move if possible
        for (auto& move : availableMoves) {
            // Select the best available move
            int x = move.first;
            int y = move.second;

            // For now, just return the first available move (can be changed to apply AI logic)
            return {{x, y}, availableNumbers.begin() != availableNumbers.end() ? *availableNumbers.begin() : -1};
        }

        // No valid moves found
        return {{-1, -1}, -1};
    }

    // Recursive function to implement Minimax algorithm
    int calculateMinMax(NumericalBoard<int>& boardInstance, int currentSymbol, bool isMaximizing) {
        if (boardInstance.is_win()) {
            return isMaximizing ? -10 : 10;
        }
        if (boardInstance.is_draw()) {
            return 0;
        }

        int bestValue = isMaximizing ? std::numeric_limits<int>::min() : std::numeric_limits<int>::max();

        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (boardInstance.board[i][j] == 0) {
                    boardInstance.board[i][j] = currentSymbol;

                    int value = calculateMinMax(boardInstance, currentSymbol, !isMaximizing);

                    boardInstance.board[i][j] = 0;  // Undo the move

                    bestValue = isMaximizing ? std::max(bestValue, value) : std::min(bestValue, value);
                }
            }
        }

        return bestValue;
    }
};




// NumericalSmartPlayer class
template<typename T>
class NumericalSmartPlayer : public Player<int> {
private:
    unordered_set<int> availableNumbers;

public:
    NumericalSmartPlayer(string name, unordered_set<int> numbers) : Player<int>(name, 0), availableNumbers(numbers) {}

    void getmove(int& x, int& y) override {
        NumericalBoard<int> boardInstance;
        getmove(x, y, boardInstance);
    }

    void getmove(int& x, int& y, NumericalBoard<int>& boardInstance) {
        displayAvailableNumbers();

        // Try to find the best move
        bool foundMove = false;
        for (int i = 0; i < boardInstance.rows && !foundMove; ++i) {
            for (int j = 0; j < boardInstance.columns && !foundMove; ++j) {
                if (boardInstance.board[i][j] == 0 && globalUsedPositions.find({i, j}) == globalUsedPositions.end()) {
                    // Check if this move would make the player win
                    for (int num : availableNumbers) {
                        if (globalUsedNumbers.find(num) == globalUsedNumbers.end()) {
                            // Check if this number could potentially make the player win
                            boardInstance.board[i][j] = num; // Temporarily place the number
                            if (boardInstance.is_win()) {
                                x = i;
                                y = j;
                                this->symbol = num;
                                boardInstance.board[i][j] = num; // Make the move
                                availableNumbers.erase(num);
                                globalUsedNumbers.insert(num);
                                globalUsedPositions.insert({x, y});
                                foundMove = true;
                                break;
                            }
                            boardInstance.board[i][j] = 0; // Undo the move
                        }
                    }
                }
            }
        }

        // If no winning move, try to block the opponent's winning move
        if (!foundMove) {
            for (int i = 0; i < boardInstance.rows && !foundMove; ++i) {
                for (int j = 0; j < boardInstance.columns && !foundMove; ++j) {
                    if (boardInstance.board[i][j] == 0 && globalUsedPositions.find({i, j}) == globalUsedPositions.end()) {
                        // Check if this move could block the opponent's winning move
                        for (int num : availableNumbers) {
                            if (globalUsedNumbers.find(num) == globalUsedNumbers.end()) {
                                boardInstance.board[i][j] = num; // Temporarily place the number
                                if (boardInstance.is_win()) {
                                    x = i;
                                    y = j;
                                    this->symbol = num;
                                    boardInstance.board[i][j] = num; // Make the move
                                    availableNumbers.erase(num);
                                    globalUsedNumbers.insert(num);
                                    globalUsedPositions.insert({x, y});
                                    foundMove = true;
                                    break;
                                }
                                boardInstance.board[i][j] = 0; // Undo the move
                            }
                        }
                    }
                }
            }
        }

        // If no winning or blocking move found, choose a random valid move
        if (!foundMove) {
            vector<pair<int, int>> validMoves;
            for (int i = 0; i < boardInstance.rows; ++i) {
                for (int j = 0; j < boardInstance.columns; ++j) {
                    if (boardInstance.board[i][j] == 0 && globalUsedPositions.find({i, j}) == globalUsedPositions.end()) {
                        validMoves.push_back({i, j});
                    }
                }
            }

            if (!validMoves.empty()) {
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

                    cout << "Smart Computer chooses: " << num << " at (" << x << ", " << y << ")\n";
                } else {
                    cout << "No valid numbers available for the smart player. Game Over.\n";
                }
            } else {
                cout << "No valid moves available. Game Over.\n";
            }
        }
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




#endif // NUMERICAL_TIC_TAC_TOE_H

/*
template<typename T>
class NumericalMinMaxPlayer : public Player<T> {
public:
    NumericalMinMaxPlayer(string name, int playerNumber)
        : Player<T>(name, 0), playerPosition(playerNumber) {
        if (playerPosition == 1) {
            availableNumbers = {1, 3, 5, 7, 9};
        } else {
            availableNumbers = {2, 4, 6, 8};
        }
    }

    void setBoard(NumericalBoard<int>* board) {
        if (!board) {
            cout << "Error: Cannot set a null board!" << endl;
            return;
        }
        this->boardPtr = board;
    }

    void getmove(int& x, int& y) override {
        if (!this->boardPtr) {
            cout << "Error: Board pointer not set!" << endl;
            return;
        }

        // Attempt to cast to the correct type
        NumericalBoard<int>* numericalBoard = dynamic_cast<NumericalBoard<int>*>(this->boardPtr);
        if (!numericalBoard) {
            cout << "Error: Cannot cast board to NumericalBoard!" << endl;
            return;
        }

        // Call the version of getmove that takes a NumericalBoard
        getmove(x, y, *numericalBoard);
    }

    bool isValidMove(int x, int y, NumericalBoard<int>& board) {
        // Ensure the position is within bounds and the cell is empty
        return (x >= 0 && x < 3 && y >= 0 && y < 3 && board.board[x][y] == 0);
    }

    void getmove(int& x, int& y, NumericalBoard<int>& boardInstance) {
        std::cout << "Ai's available numbers before move: ";
        for (int num : availableNumbers) {
            std::cout << num << " ";
        }
        std::cout << std::endl;

        // Clean up available numbers (check if used already)
        for (auto it = availableNumbers.begin(); it != availableNumbers.end();) {
            if (globalUsedNumbers.find(*it) != globalUsedNumbers.end()) {
                it = availableNumbers.erase(it);
            } else {
                ++it;
            }
        }

        std::cout << "Ai's available numbers after cleaning: ";
        for (int num : availableNumbers) {
            std::cout << num << " ";
        }
        std::cout << std::endl;

        // Get the best move and validate
        std::pair<std::pair<int, int>, int> bestMove = getBestMove(boardInstance);
        x = bestMove.first.first;
        y = bestMove.first.second;

        if (x >= 0 && y >= 0 && boardInstance.board[x][y] == 0) {
            // Apply the move to the board
            std::cout << "Ai chooses: " << this->symbol << " at (" << x << ", " << y << ")\n";
            boardInstance.update_board(x, y, this->symbol); // Ensure update_board works correctly
            availableNumbers.erase(this->symbol);            // Remove the used number
            globalUsedNumbers.insert(this->symbol);          // Mark the number as used
            globalUsedPositions.insert({x, y});              // Track the used position
        } else {
            // If invalid, print an error and skip the move
            std::cout << "Invalid move: Unable to find a valid move for Ai. Skipping turn.\n";
        }
    }




private:
    unordered_set<int> availableNumbers;
    unordered_set<int> globalUsedNumbers;
    unordered_set<pair<int, int>, pair_hash> globalUsedPositions; // Use custom hash function
    int playerPosition;
    std::vector<std::pair<int, int>> availableMoves;

    // Function to calculate the best move using Minimax
    std::pair<std::pair<int, int>, int> getBestMove(NumericalBoard<int>& boardInstance) {
        cout << "Evaluating best move for " << this->name << endl;

        // Log the current board state
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                cout << boardInstance.board[i][j] << " ";
            }
            cout << endl;
        }

        // Log available numbers
        cout << "Available numbers: ";
        for (int num : availableNumbers) {
            cout << num << " ";
        }
        cout << endl;

        // Step 1: Populate the availableMoves container
        std::vector<std::pair<int, int>> availableMoves;

        for (int i = 0; i < boardInstance.rows; i++) {
            for (int j = 0; j < boardInstance.columns; j++) {
                if (boardInstance.board[i][j] == 0) {  // Assuming 0 means an empty spot
                    availableMoves.push_back(std::make_pair(i, j));  // Add available position
                }
            }
        }

        // Step 2: Check the available moves and evaluate
        for (const auto& move : availableMoves) {
            std::cout << "Considering move: " << move.first << ", " << move.second << std::endl;
            // Additional checks...
        }

        // Step 3: Return the first valid move if possible
        for (auto& move : availableMoves) {
            // Select the best available move
            int x = move.first;
            int y = move.second;

            // For now, just return the first available move (can be changed to apply AI logic)
            return {{x, y}, availableNumbers.begin() != availableNumbers.end() ? *availableNumbers.begin() : -1};
        }

        // No valid moves found
        return {{-1, -1}, -1};
    }



    // Recursive function to implement Minimax algorithm
    int calculateMinMax(NumericalBoard<int>& boardInstance, int currentSymbol, bool isMaximizing) {
        if (boardInstance.is_win()) {
            return isMaximizing ? -10 : 10;
        }
        if (boardInstance.is_draw()) {
            return 0;
        }

        int bestValue = isMaximizing ? std::numeric_limits<int>::min() : std::numeric_limits<int>::max();

        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (boardInstance.board[i][j] == 0) {
                    boardInstance.board[i][j] = currentSymbol;

                    int value = calculateMinMax(boardInstance, currentSymbol, !isMaximizing);

                    boardInstance.board[i][j] = 0;  // Undo the move

                    bestValue = isMaximizing ? std::max(bestValue, value) : std::min(bestValue, value);
                }
            }
        }

        return bestValue;
    }
}
*/

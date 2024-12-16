
#include "BoardGame_Classes.h"
#include "Word_tic.h"
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <stdexcept>

using namespace std;

class AIPlayer : public Player<char> {
private:
    map<string, bool> dictionary; // To store the words from the dictionary

    void loadDictionary(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            throw runtime_error("Failed to open dictionary file.");
        }

        string word;
        while (file >> word) {
            dictionary[word] = true;
        }
        file.close();
    }

    bool isWordValid(const string& word) const {
        return dictionary.find(word) != dictionary.end();
    }

    // Helper function to get available moves
    vector<pair<int, int>> getAvailableMoves(WordBoard<char>* board) {
        vector<pair<int, int>> moves;
        for (int i = 0; i < board->getRows(); ++i) {
            for (int j = 0; j < board->getColumns(); ++j) {
                if (board->getCell(i, j) == 0) {
                    moves.emplace_back(i, j);
                }
            }
        }
        return moves;
    }

   // Evaluate a potential move by simulating it with all characters
string evaluateMove(WordBoard<char>* board, int x, int y, char& bestChar) {
    string bestWord;
    for (char c = 'A'; c <= 'Z'; ++c) { // Try all characters
        board->update_board(x, y, c); // Simulate move with character c

        // Evaluate rows
        for (int i = 0; i < board->getRows(); ++i) {
            string rowWord;
            for (int j = 0; j < board->getColumns(); ++j) {
                rowWord += board->getCell(i, j);
            }
            if (isWordValid(rowWord)) {
                if (rowWord.length() > bestWord.length()) {
                    bestWord = rowWord;
                    bestChar = c;
                    this->symbol = c;
                }
            }
        }

        // Evaluate columns
        for (int j = 0; j < board->getColumns(); ++j) {
            string colWord;
            for (int i = 0; i < board->getRows(); ++i) {
                colWord += board->getCell(i, j);
            }
            if (isWordValid(colWord)) {
                if (colWord.length() > bestWord.length()) {
                    bestWord = colWord;
                    bestChar = c;
                    this->symbol = c;
                }
            }
        }

        // Evaluate main diagonal (top-left to bottom-right)
        string mainDiagonalWord;
        for (int i = 0; i < board->getRows() && i < board->getColumns(); ++i) {
            mainDiagonalWord += board->getCell(i, i);
        }
        if (isWordValid(mainDiagonalWord)) {
            if (mainDiagonalWord.length() > bestWord.length()) {
                bestWord = mainDiagonalWord;
                bestChar = c;
                this->symbol = c;
            }
        }

        // Evaluate anti-diagonal (top-right to bottom-left)
        string antiDiagonalWord;
        for (int i = 0; i < board->getRows() && i < board->getColumns(); ++i) {
            antiDiagonalWord += board->getCell(i, board->getColumns() - 1 - i);
        }
        if (isWordValid(antiDiagonalWord)) {
            if (antiDiagonalWord.length() > bestWord.length()) {
                bestWord = antiDiagonalWord;
                bestChar = c;
                this->symbol = c;
            }
        }

        board->update_board(x, y, 0); // Undo move
    }
    return bestWord;
}


public:
    AIPlayer(string name, char symbol, const string& dictionaryFile) : Player<char>(name, symbol) {
        loadDictionary(dictionaryFile);
        srand(static_cast<unsigned int>(time(0))); // Seed for randomness
    }

    void getmove(int& x, int& y) override {
        char c;
        getmove(x, y, c);
    }

    void getmove(int& x, int& y, char& c)  {
        WordBoard<char>* board = static_cast<WordBoard<char>*>(this->boardPtr);

        // Find the best move based on dictionary lookups
        vector<pair<int, int>> availableMoves = getAvailableMoves(board);
        string bestWord;
        pair<int, int> bestMove;
        char bestChar = this->symbol;

        for (const auto& move : availableMoves) {
            char candidateChar;
            string potentialWord = evaluateMove(board, move.first, move.second, candidateChar);
            if (!potentialWord.empty() && potentialWord.length() > bestWord.length()) {
                bestWord = potentialWord;
                bestMove = move;
                bestChar = candidateChar; // Use the best character
            }
        }

        if (bestWord.empty()) {
            // No winning move found, pick randomly
            int randomIndex = rand() % availableMoves.size();
            bestMove = availableMoves[randomIndex];
            bestChar = 'A' + rand() % 26; // Random character
        }

        x = bestMove.first;
        y = bestMove.second;
        c = bestChar;
    }
};

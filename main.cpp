#include <iostream>
#include <string>
#include <algorithm>
#include "BoardGame_Classes.h"
#include "Four-in-a-row.h"
#include "Numerical Tic-Tac-Toe.h"
#include <unordered_set>

using namespace std;

void setupAndRunFourInARow() {
    Board<char>* B = new FourInARowBoard<char>(); // Correct board type
    Player<char>* players[2] = {nullptr, nullptr};
    string playerName;
    int choice;

    try {
        // Setup Player 1
        cout << "Enter Player 1 name: ";
        cin >> playerName;
        cout << "Choose Player 1 type:\n1. Human\n2. Random Computer\n3. Smart Computer (AI)\n";
        cin >> choice;

        if (choice == 1)
            players[0] = new FourInRowPlayer<char>(playerName, 'X');
        else if (choice == 2)
            players[0] = new FourInARowRandomPlayer<char>('X');

        else
            throw std::invalid_argument("Invalid choice for Player 1.");

        // Setup Player 2
        cout << "Enter Player 2 name: ";
        cin >> playerName;
        cout << "Choose Player 2 type:\n1. Human\n2. Random Computer\n3. Smart Computer (AI)\n";
        cin >> choice;

        if (choice == 1)
            players[1] = new FourInRowPlayer<char>(playerName, 'O');
        else if (choice == 2)
            players[1] = new FourInARowRandomPlayer<char>('O');

        else
            throw std::invalid_argument("Invalid choice for Player 2.");

        // Create and run the game
        GameManager<char> game(B, players);
        game.run();

    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }

    // Cleanup
    delete B;
    for (int i = 0; i < 2; ++i) {
        delete players[i];
    }
}

void setupAndRunNumericalTicTacToe() {
    Board<int>* B = new NumericalBoard<int>();
    Player<int>* players[2] = {nullptr, nullptr};
    unordered_set<int> oddNumbers = {1, 3, 5, 7, 9};
    unordered_set<int> evenNumbers = {2, 4, 6, 8};

    string playerName;
    int choice;

    try {
        // Setup Player 1
        cout << "Enter Player 1 name: ";
        cin >> playerName;
        cout << "Choose Player 1 type:\n1. Human\n2. Random Computer\n3. Smart Computer (AI)\n";
        cin >> choice;

        if (choice == 1)
            players[0] = new HumanPlayer<int>(playerName, oddNumbers);
        else if (choice == 2)
            players[0] = new NumericalRandomPlayer<int>(evenNumbers);
        else
            throw std::invalid_argument("Invalid choice for Player 1.");

        // Setup Player 2
        cout << "Enter Player 2 name: ";
        cin >> playerName;
        cout << "Choose Player 2 type:\n1. Human\n2. Random Computer\n3. Smart Computer (AI)\n";
        cin >> choice;

        if (choice == 1)
            players[1] = new HumanPlayer<int>(playerName, oddNumbers);
        else if (choice == 2)
            players[1] = new NumericalRandomPlayer<int>(evenNumbers);
        else
            throw std::invalid_argument("Invalid choice for Player 2.");

        // Create and run the game
        GameManager<int> game(B, players);
        game.run();

    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }

    // Cleanup
    delete B;
    for (int i = 0; i < 2; ++i) {
        delete players[i];
    }
}

int main() {
    int choice;
    do {
        cout << "Select a game to play:\n";
        cout << "1. Four in a Row\n";
        cout << "2. Numerical Tic Tac Toe\n";
        cout << "3. Exit\n";
        cin >> choice;

        switch (choice) {
            case 1:
                setupAndRunFourInARow();
                break;
            case 2:
                setupAndRunNumericalTicTacToe();
                break;
            case 3:
                cout << "Exiting the program.\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 3);

    return 0;
}




#include <iostream>
#include "word_tic.h"

using namespace std;


int main() {
    srand(time(nullptr));  
    int choice;
    Player<char>* players[2] = {nullptr, nullptr};
    WordBoard<char>* board = new WordBoard<char>();
    string player1Name, player2Name;

    cout << "Welcome to Word Tic Tac Toe!\n";

    try {
        // Setup Player 1
        cout << "Enter Player 1 name: ";
        cin >> player1Name;

        cout << "Choose Player 1 type:\n";
        cout << "1. Human\n";
        cout << "2. Random Computer\n";
        cin >> choice;

        if (choice == 1) {
            players[0] = new WordPlayer<char>(player1Name, ' ');
        } else if (choice == 2) {
            players[0] = new Word_Random_Player<char>(' ');
            cout << player1Name << " is a Random Computer Player!\n";
        } else {
            throw invalid_argument("Invalid choice for Player 1. Please enter 1 or 2.");
        }

        // Setup Player 2
        cout << "Enter Player 2 name: ";
        cin >> player2Name;

        cout << "Choose Player 2 type:\n";
        cout << "1. Human\n";
        cout << "2. Random Computer\n";
        cin >> choice;

        if (choice == 1) {
            players[1] = new WordPlayer<char>(player2Name, ' ');
        } else if (choice == 2) {
            players[1] = new Word_Random_Player<char>(' ');
            cout << player2Name << " is a Random Computer Player!\n";
        } else {
            throw invalid_argument("Invalid choice for Player 2. Please enter 1 or 2.");
        }

        // Create the game manager
        GameManager<char> gameManager(board, players);

        // Run the game
        gameManager.run();

    } catch (const invalid_argument& e) {
        cerr << "Error: " << e.what() << endl;
        delete board;
        for (int i = 0; i < 2; ++i) {
            delete players[i];
        }
        return 1;
    }

    // Cleanup
    delete board;
    delete players[0];
    delete players[1];

    return 0;
}

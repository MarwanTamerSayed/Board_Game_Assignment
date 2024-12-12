#include "Ultimate_Tic.h"
#include <iostream>
#include <string>
using namespace std;

int main() {
    srand(time(nullptr));  
    int choice;
    Player<char>* players[2];
    Ultimate_XO_Board<char>* board = new Ultimate_XO_Board<char>();
    string player1Name, player2Name;

    cout << "Welcome to Ultimate Tic Tac Toe!\n";

    // Set up Player 1
    cout << "Enter Player 1 name: ";
    cin >> player1Name;
    cout << "Choose Player 1 type:\n";
    cout << "1. Human\n";
    cout << "2. Random Computer\n";
    cin >> choice;

    switch (choice) {
        case 1:
            players[0] = new Ultimate_Player<char>(player1Name, 'X');
            break;
        case 2:
            players[0] = new Ultimate_Random_Player<char>('X');
            break;
        default:
            cout << "Invalid choice for Player 1. Exiting the game.\n";
            delete board;
            return 1;
    }

    // Set up Player 2
    cout << "Enter Player 2 name: ";
    cin >> player2Name;
    cout << "Choose Player 2 type:\n";
    cout << "1. Human\n";
    cout << "2. Random Computer\n";
    cin >> choice;

    switch (choice) {
        case 1:
            players[1] = new Ultimate_Player<char>(player2Name, 'O');
            break;
        case 2:
            players[1] = new Ultimate_Random_Player<char>('O');
            break;
        default:
            cout << "Invalid choice for Player 2. Exiting the game.\n";
            delete players[0];
            delete board;
            return 1;
    }

    // Create the game manager and start the game
    GameManager<char> game(board, players);
    game.run();

    // Clean up resources
    delete board;
    delete players[0];
    delete players[1];

    return 0;
}

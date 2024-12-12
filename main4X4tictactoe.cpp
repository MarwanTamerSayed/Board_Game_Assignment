#include <iostream>
#include "BoardGame_Classes.h"
#include "4X4_TICTACTOE.h"
#include "MinMaxPlayer.h"

using namespace std;

int main() {
    char choice;
    Player<char>* players[2];
    X_O_Board<char>* B = new X_O_Board<char>();
    string player1Name, player2Name;

    cout << "Welcome to FCAI X-O 4x4 TicTacToe Game. :)\n";

    // Set up Player 1
    cout << "Enter Player X name: ";
    cin >> player1Name;
    cout << "Choose Player X type:\n";
    cout << "1. Human\n";
    cout << "2. Random Computer\n";
    cout << "3. Smart Computer (AI)\n";
    cout << "Enter Choice: ";
    cin >> choice;

    if (choice == 'a') {
        players[0] = new X_O_Human_Player<char>(player1Name, 'X');
    } else if (choice == 'b') {
        players[0] = new X_O_Game_Random_Player<char>('X');
        players[0]->setBoard(B);
    } else if (choice == 'c') {
        players[0] = new X_O_MinMax_Player<char>('X');
        players[0]->setBoard(B);
    } else {
        cout << "Invalid choice for Player X. Exiting the game.\n";
        return 1;
    }

    // Set up Player 2
    cout << "Enter Player O name: ";
    cin >> player2Name;
    cout << "Choose Player O type:\n";
    cout << "1. Human\n";
    cout << "2. Random Computer\n";
    cout << "3. Smart Computer (AI)\n";
    cout << "Enter Choice: ";
    cin >> choice;

    if (choice == 'a') {
        players[1] = new X_O_Human_Player<char>(player2Name, 'O');
    } else if (choice == 'b') {
        players[1] = new X_O_Game_Random_Player<char>('O');
        players[1]->setBoard(B);
    } else if (choice == 'c') {
        players[1] = new X_O_MinMax_Player<char>('O');
        players[1]->setBoard(B);
    } else {
        cout << "Invalid choice for Player O. Exiting the game.\n";
        return 1;
    }

    // Create the game manager and run the game
    GameManager<char> x_o_game(B, players);
    x_o_game.run();

    // Clean up
    delete B;
    for (int i = 0; i < 2; ++i) {
        delete players[i];
    }

    return 0;
}

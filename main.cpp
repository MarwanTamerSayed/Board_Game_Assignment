#include <iostream>
#include "BoardGame_Classes.h"
#include "5x5.h"
#include "MinMaxPlayer.h"

using namespace std;

int main() {
    int choice;
    Player<char> *players[2];
    X_O_Board<char>* B = new X_O_Board<char>();
    string PlayerOneName, PlayerTwoName;
    cout << "Welcome to FCAI 5x5 Tic Tac Toe Game. :)\n";
    // Set up player 1
    cout << "Enter Player one name:";
    cin >> PlayerOneName;
    cout << "Choose Player one type:\n";
    cout << "1- Human\n";
    cout << "2- Random Computer\n";
    cout << "3- Smart Computer (AI)\n";
    cin >> choice;
    switch(choice) {
        case 1:
            players[0] = new X_O_HumanPlayer<char>(PlayerOneName, 'X');
            players[0]->setBoard(B);
            break;
        case 2:
            players[0] = new  X_O_RandomPlayer<char>('X');
            players[0]->setBoard(B);
            break;
        // case 3:
        //     players[0] = new X_O_MinMax_Player<char>('X');
        //     players[0]->setBoard(B);
        //     break;
        default:
            cout << "Invalid choice for Player one Exiting the game.\n";
            return 0;
    }

    // Set up player 2
    cout << "Enter Player two name:";
    cin >> PlayerTwoName;
    cout << "Choose Player two type:\n";
    cout << "1- Human\n";
    cout << "2- Random Computer\n";
    cout << "3- Smart Computer (AI)\n";
    cin >> choice;
    switch(choice) {
        case 1:
            players[1] = new X_O_HumanPlayer<char>(PlayerTwoName, 'O');
            players[1]->setBoard(B);
            break;
        case 2:
            players[1] = new  X_O_RandomPlayer<char>('O');
            players[1]->setBoard(B);
            break;
        // case 3:
        //     players[1] = new X_O_MinMax_Player<char>('O');
        //     players[1]->setBoard(B);
        //     break;
        default:
            cout << "Invalid choice for Player two Exiting the game.\n";
            return 0;
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
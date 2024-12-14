#include <iostream>
#include "BoardGame_Classes.h"
#include "Pyramid_X_O.h"
#include "PyramidMinMaxPlayer.h"

using namespace std;

int main() {
    int choice;
    Player<char>* players[2];
    PyramidBoard<char>* B = new PyramidBoard<char>();
    string playerXName, player2Name;

    cout << "Welcome to FCAI Pyramid X-O Game. :)\n";

    // Set up player 1
    cout << "Enter Player X name: ";
    cin >> playerXName;
    cout << "Choose Player X type:\n";
    cout << "1. Human\n";
    cout << "2. Random Computer\n";
    cout << "3. Smart Computer (AI)\n";
    cin >> choice;

    switch(choice) {
        case 1:
            players[0] = new X_O_Player<char>(playerXName, 'X');
            break;
        case 2:
            players[0] = new X_O_Random_Player<char>('X');
            break;
        case 3:
            players[0] = new Pyramid_MinMax_Player<char>('X');
            players[0]->setBoard(B);
            break;
        default:
            cout << "Invalid choice for Player 1. Exiting the game.\n";
            return 1;
    }

    // Set up player 2
    cout << "Enter Player 2 name: ";
    cin >> player2Name;
    cout << "Choose Player 2 type:\n";
    cout << "1. Human\n";
    cout << "2. Random Computer\n";
    cout << "3. Smart Computer (AI)\n";
    cin >> choice;

    switch(choice) {
        case 1:
            players[1] = new X_O_Player<char>(player2Name, 'O');
            break;
        case 2:
            players[1] = new X_O_Random_Player<char>('O');
            break;
        case 3:
            players[1] = new Pyramid_MinMax_Player<char>('O');
            players[1]->setBoard(B);
            break;
        default:
            cout << "Invalid choice for Player 2. Exiting the game.\n";
            return 1;
    }

    // Create the game manager and run the game
    GameManager<char> pyramid_game(B, players);
    pyramid_game.run();

    // Clean up
    delete B;
    for (int i = 0; i < 2; ++i) {
        delete players[i];
    }

    return 0;
}

#include <iostream>
#include "word_tic.h" 

using namespace std;

int main() {
    srand(time(nullptr));  
    int choice;
    Player<char>* players[2];
    WordBoard<char>* board = new WordBoard<char>();
    string player1Name, player2Name;

    cout << "Welcome to Word Tic Tac Toe!\n";

    // Set up Player 1
    cout << "Enter Player 1 name: ";
    cin >> player1Name;
    cout << "Choose Player 1 type:\n";
    cout << "1. Human\n";
    cout << "2. Random Computer\n";
    cin >> choice;

    switch (choice) {
        case 1:
            players[0] = new WordPlayer<char>(player1Name, ' ');
            break;
        case 2:
            players[0] = new Word_Random_Player<char>(' ');
            cout << player1Name << " is a Random Computer Player!\n";
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
            players[1] = new WordPlayer<char>(player2Name, ' ');
            break;
        case 2:
            players[1] = new Word_Random_Player<char>(' ');
            cout << player2Name << " is a Random Computer Player!\n";
            break;
        default:
            cout << "Invalid choice for Player 2. Exiting the game.\n";
            delete players[0];
            delete board;
            return 1;
    }

    // Start the game
    cout << "Starting the game... Players will input characters to form valid words.\n";
    board->display_board();

    while (!board->game_is_over()) {
        for (int i = 0; i < 2; ++i) {
            int x, y;
            char symbol;

            cout << players[i]->getname() << "'s turn:\n";

            if (dynamic_cast<Word_Random_Player<char>*>(players[i])) {
                cout << "Random Player is making a move...\n";
                static_cast<Word_Random_Player<char>*>(players[i])->getmove(x, y);
                symbol = 'A' + rand() % 26; // Random letter as symbol
            } else {
                cout << "Enter the row and column (0-2) followed by a character: ";
                cin >> x >> y >> symbol;
            }

            while (!board->update_board(x, y, toupper(symbol))) {
                if (dynamic_cast<Word_Random_Player<char>*>(players[i])) {
                    static_cast<Word_Random_Player<char>*>(players[i])->getmove(x, y);
                    symbol = 'A' + rand() % 26;
                } else {
                    cout << "Invalid move. Try again (row column character): ";
                    cin >> x >> y >> symbol;
                }
            }

            board->display_board();

            if (board->is_win()) {
                cout << players[i]->getname() << " wins by forming a valid word! Congratulations!\n";
                delete board;
                for (int j = 0; j < 2; ++j) delete players[j];
                return 0;
            }

            if (board->is_draw()) {
                cout << "It's a draw! No valid words formed.\n";
                delete board;
                for (int j = 0; j < 2; ++j) delete players[j];
                return 0;
            }
        }
    }

    // Clean up resources
    delete board;
    for (int i = 0; i < 2; ++i) delete players[i];

    return 0;
}

#include <iostream>
#include "word_tic.h" // Ensure this includes the WordBoard and player classes.

using namespace std;

int main() {
    Player<char>* players[2];
    WordBoard<char>* board = new WordBoard<char>();
    string playerName;
    int choice;

    cout << "Welcome to Word Tic-Tac-Toe!\n";

    // Setup Player 1
    cout << "For Player 1, enter 1 for Human or 2 for Random Player: ";
    cin >> choice;
    if (choice == 1) {
        cout << "Enter Player 1 name: ";
        cin >> playerName;
        players[0] = new WordPlayer<char>(playerName, ' '); // Symbol is dynamic based on input.
    } else if (choice == 2) {
        players[0] = new Word_Random_Player<char>(' '); // Random Player
        cout << "Player 1 is a Random Computer Player!\n";
    } else {
        cout << "Invalid choice, defaulting Player 1 to Human Player.\n";
        cout << "Enter Player 1 name: ";
        cin >> playerName;
        players[0] = new WordPlayer<char>(playerName, ' ');
    }

    // Setup Player 2
    cout << "For Player 2, enter 1 for Human or 2 for Random Player: ";
    cin >> choice;
    if (choice == 1) {
        cout << "Enter Player 2 name: ";
        cin >> playerName;
        players[1] = new WordPlayer<char>(playerName, ' '); // Symbol is dynamic based on input.
    } else if (choice == 2) {
        players[1] = new Word_Random_Player<char>(' '); // Random Player
        cout << "Player 2 is a Random Computer Player!\n";
    } else {
        cout << "Invalid choice, defaulting Player 2 to Human Player.\n";
        cout << "Enter Player 2 name: ";
        cin >> playerName;
        players[1] = new WordPlayer<char>(playerName, ' ');
    }

    // Start the game
    cout << "Starting the game... Players will input characters to form valid words.\n";
    board->display_board();

    while (!board->game_is_over()) {
        for (int i = 0; i < 2; ++i) {
            int x, y;
            char symbol;

            cout << players[i]->getname() << "'s turn:\n";

            // If it's a Random Player, automatically generate a move
            if (dynamic_cast<Word_Random_Player<char>*>(players[i])) {
                cout << "Random Player is making a move...\n";
                static_cast<Word_Random_Player<char>*>(players[i])->getmove(x, y);
                symbol = 'A' + rand() % 26; // Random letter as symbol
                // cout << "Random Player chose (" << x << ", " << y << ") with symbol " << symbol << endl;
            } else {
                // Human player input
                cout << "Enter the row and column (0-2) followed by a character: ";
                cin >> x >> y >> symbol;
            }

            // Validate and update the board
            while (!board->update_board(x, y, toupper(symbol))) {
                if (dynamic_cast<Word_Random_Player<char>*>(players[i])) {
                    static_cast<Word_Random_Player<char>*>(players[i])->getmove(x, y);
                    symbol = 'A' + rand() % 26;
                    // cout << "Random Player chose (" << x << ", " << y << ") with symbol " << symbol << endl;
                } else {
                    cout << "Invalid move. Try again (row column character): ";
                    cin >> x >> y >> symbol;
                }
            }

            board->display_board();

            // Check if the game has ended
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

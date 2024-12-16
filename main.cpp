#include <bits/stdc++.h>
#include "BoardGame_Classes.h"
#include "Four-in-a-row.h"
#include "Numerical Tic-Tac-Toe.h"
#include "4X4_TICTACTOE.h"
#include "Pyramid_X_O.h"
#include "5X5.h"
#include "word_tic.h"
#include "3x3misereX_O.h"
#include "MinMaxPlayermisere.h"
#include "Ultimate_Tic.h"
#include "3X3_SUS.h"
using namespace std;

void setupAndRun3X3_SUS();
void setupAndRunFourInARow();
void setupAndRunNumericalTicTacToe();
void setupandRun4x4TicTacToe();
void setupAndrunPyramid_X_O();
void setupAndrun5X5();
void setupAndrunwordTic();
void setupAndrunX_OMisere();
void setupAndrunUltimateX_O();

int main() {
    int choice;
    do {
        cout << "Select a game to play:\n";
        cout << "1. Pyramid Fun\n";
        cout << "2. Four in a Row\n";
        cout << "3. 5X5 Tic Tac Toe\n";
        cout << "4. Word Tic Tac Toe\n";
        cout << "5. Numerical Tic Tac Toe\n";
        cout << "6. XO Misere\n";
        cout << "7. 4X4 Tic Tac Toe\n";
        cout << "8. Ultimate Fun\n";
        cout << "9. 3X3 SUS game\n";
        cout << "10. EXit\n";
        cin >> choice;

        switch (choice) {
            case 1:
                setupAndrunPyramid_X_O();
            break;
            case 2:
                setupAndRunFourInARow();
            break;
            case 3:
                setupAndrun5X5();
            break;
            case 4:
                setupAndrunwordTic();
            break;
            case 5:
                setupAndRunNumericalTicTacToe();
            break;
            case 6:
                setupAndrunX_OMisere();
            break;
            case 7:
                setupandRun4x4TicTacToe();
            break;
            case 8:
                setupAndrunUltimateX_O();
            break;
            case 9:
                setupAndRun3X3_SUS();
            break;
            case 10:
                cout << "Exiting the program.\n";
            break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 10);


    return 0;
}


void setupAndRun3X3_SUS() {
    int choice;
    Player<char>* players[2] = {nullptr, nullptr};
    SUS_Board<char>* B = new SUS_Board<char>();
    string player1Name, player2Name;
    cout << "Welcome to SUS Game!\n";
    cout << "Enter Player S name: ";
    cin >> player1Name;
    cout << "Choose Player S type:\n";
    cout << "1. Human\n";
    cout << "2. Random Computer\n";
    cin >> choice;
    switch (choice) {
        case 1:
            players[0] = new SUS_Player<char>(player1Name, 'S',B);
        break;
        case 2:
            players[0] = new SUS_Random_Player<char>('S');
        break;
        default:
            cout << "Invalid choice for Player 1. Exiting the game.\n";
        delete B;
        return;
    }
    cout << "Enter Player U name: ";
    cin >> player2Name;
    cout << "Choose Player U type:\n";
    cout << "1. Human\n";
    cout << "2. Random Computer\n";
    cin >> choice;
    switch (choice) {
        case 1:
            players[1] = new SUS_Player<char>(player2Name, 'U',B);
        break;
        case 2:
            players[1] = new SUS_Random_Player<char>('U');
        break;
        default:
            std::cout << "Invalid choice for Player 2. Exiting the game.\n";
        delete players[0]; // Cleanup for Player 1 if already created
        delete B;
        return;
    }
    // Run the game
    GameManager<char> sus_game(B, players);
    sus_game.run();
    delete B;
    for (int i = 0; i < 2; ++i) {
        delete players[i];
    }
}

void setupAndRunFourInARow() {
    FourInARowBoard<char>* B = new FourInARowBoard<char>(); // Correct board type
    Player<char>* players[2] = {nullptr, nullptr};
    string playerName;
    int choice;

    try {
        // Setup Player 1
        cout << "Enter Player 1 name: ";
        cin >> playerName;
        cout << "Choose Player 1 type:\n1. Human\n2. Random Computer\n3.Smart Player\n";
        cin >> choice;

        if (choice == 1)
            players[0] = new FourInRowPlayer<char>(playerName, 'X');
        else if (choice == 2)
            players[0] = new FourInARowRandomPlayer<char>('X');
        else if (choice == 3) players[0] = new FourInARowHardAIPlayer<char>("Ai",'X',B);
        else
            throw std::invalid_argument("Invalid choice for Player 1.");

        players[0]->setBoard(B);
        std::cout << "Board set for Player 1!" << std::endl;

        // Setup Player 2
        cout << "Enter Player 2 name: ";
        cin >> playerName;
        cout << "Choose Player 2 type:\n1. Human\n2. Random Computer\n3.Smart Player\n";
        cin >> choice;

        if (choice == 1)
            players[1] = new FourInRowPlayer<char>(playerName, 'O');
        else if (choice == 2)
            players[1] = new FourInARowRandomPlayer<char>('O');
        else if (choice == 3) players[1] = new FourInARowHardAIPlayer<char>("Ai",'O',B);
        else
            throw std::invalid_argument("Invalid choice for Player 2.");

        players[1]->setBoard(B);
        std::cout << "Board set for Player 2!" << std::endl;
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
        resetGlobalState();
        string playerName;
        int choice;

        try {
            // Setup Player 1
            cout << "Enter Player 1 name: ";
            cin >> playerName;
            cout << "Choose Player 1 type:\n1. Human\n2. Random Computer\n";
            cin >> choice;

            if (choice == 1)
                players[0] = new HumanPlayer<int>(playerName, oddNumbers);
            else if (choice == 2)
                players[0] = new NumericalRandomPlayer<int>(oddNumbers);
            else
                throw std::invalid_argument("Invalid choice for Player 1.");

            // Setup Player 2
            cout << "Enter Player 2 name: ";
            cin >> playerName;
            cout << "Choose Player 2 type:\n1. Human\n2. Random Computer\n";
            cin >> choice;

            if (choice == 1)
                players[1] = new HumanPlayer<int>(playerName, evenNumbers);
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


void setupandRun4x4TicTacToe() {
    char choice;
    Player<char>* players[2];
    X_O_Board<char>* B = new X_O_Board<char>();
    string player1Name, player2Name;

    cout << "Welcome to FCAI X-O 4x4 TicTacToe Game. :)\n";

    // Set up Player 1
    cout << "Enter Player X name: ";
    cin >> player1Name;
    cout << "Choose Player X type:\n";
    cout << "a. Human\n";
    cout << "b. Random Computer\n";
    cout << "Enter Choice: ";
    cin >> choice;

    if (choice == 'a') {
        players[0] = new X_O_Human_Player<char>(player1Name, 'X',B);
    } else if (choice == 'b') {
        players[0] = new X_O_Game_Random_Player<char>('X',B);
        players[0]->setBoard(B);
    }else {
        cout << "Invalid choice for Player X. Exiting the game.\n";
    }

    // Set up Player 2
    cout << "Enter Player O name: ";
    cin >> player2Name;
    cout << "Choose Player O type:\n";
    cout << "a. Human\n";
    cout << "b. Random Computer\n";
    cout << "Enter Choice: ";
    cin >> choice;

    if (choice == 'a') {
        players[1] = new X_O_Human_Player<char>(player2Name, 'O',B);
    } else if (choice == 'b') {
        players[1] = new X_O_Game_Random_Player<char>('O',B);
        players[1]->setBoard(B);
    } else {
        cout << "Invalid choice for Player O. Exiting the game.\n";
    }
    // Create the game manager and run the game
    GameManager<char> x_o_game(B, players);
    x_o_game.run();

    // Clean up
    delete B;
    for (int i = 0; i < 2; ++i) {
        delete players[i];
    }
}

void setupAndrunPyramid_X_O() {
    int choice;
    Player<char>* players[2];
    PyramidBoard<char>* B = new PyramidBoard<char>();
    string playerXName, player2Name;

    cout << "Welcome to FCAI X-O Game. :)\n";

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
            players[0] = new X_O_MinMax_Player5X5<char>('X');
        players[0]->setBoard(B);
        break;
        default:
            cout << "Invalid choice for Player 1. Exiting the game.\n";
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
            players[1] = new X_O_MinMax_Player5X5<char>('O');
        players[1]->setBoard(B);
        break;
        default:
            cout << "Invalid choice for Player 2. Exiting the game.\n";

    }

    // Create the game manager and run the game
    GameManager<char> x_o_game(B, players);
    x_o_game.run();

    // Clean up
    delete B;
    for (int i = 0; i < 2; ++i) {
        delete players[i];
    }
}

void setupAndrun5X5() {
    int choice;
    Player<char> *players[2];
    X_O_Board5X5<char>* B = new X_O_Board5X5<char>();
    string PlayerOneName, PlayerTwoName;
    cout << "Welcome to FCAI 5x5 Tic Tac Toe Game. :)\n";
    // Set up player 1
    cout << "Enter Player one name:";
    cin >> PlayerOneName;
    cout << "Choose Player one type:\n";
    cout << "1- Human\n";
    cout << "2- Random Computer\n";
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
        default:
            cout << "Invalid choice for Player one Exiting the game.\n";
    }
    // Set up player 2
    cout << "Enter Player two name:";
    cin >> PlayerTwoName;
    cout << "Choose Player two type:\n";
    cout << "1- Human\n";
    cout << "2- Random Computer\n";
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
        default:
            cout << "Invalid choice for Player two Exiting the game.\n";

    }

    // Create the game manager and run the game
    GameManager<char> x_o_game(B, players);
    x_o_game.run();

    // Clean up
    delete B;
    for (int i = 0; i < 2; ++i) {
        delete players[i];
    }
}

void setupAndrunwordTic() {
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
            }

            if (board->is_draw()) {
                cout << "It's a draw! No valid words formed.\n";
                delete board;
                for (int j = 0; j < 2; ++j) delete players[j];
            }
        }
    }

    // Clean up resources
    delete board;
    for (int i = 0; i < 2; ++i) delete players[i];
}

void setupAndrunX_OMisere() {
    int choice1,choice2;
    Player<char>* players[2];
    X_O_Board_misere<char>* B = new X_O_Board_misere<char>();
    string player1Name, player2Name;

    cout << "Welcome to FCAI X-O misere Game. :)\n";

    // Set up player 1
    cout << "Enter Player X name: ";
    cin >> player1Name;
    cout << "Choose Player X type:\n";
    cout << "1. Human\n";
    cout << "2. Random Computer\n";
    cout << "3. Smart Computer (AI)\n";
    cin >> choice1;
    // Set up player 2
    cout << "Enter Player O name: ";
    cin >> player2Name;
    cout << "Choose Player O type:\n";
    cout << "1. Human\n";
    cout << "2. Random Computer\n";
    cout << "3. Smart Computer (AI)\n";
    cin >> choice2;
    switch(choice1) {
        case 1:
            players[0] = new X_O_Player_misere<char>(player2Name, 'X');
        break;
        case 2:
            players[0] = new X_O_Random_Player_misere<char>('X');
        break;
        case 3:
            players[0] = new X_O_MinMax_Player5X5<char>('X');
        players[0]->setBoard(B);
        break;
        default:
            cout << "Invalid choice for Player X. Exiting the game.\n";
    }
    switch(choice2) {
        case 1:
            players[1] = new X_O_Player_misere<char>(player1Name, 'O');
        break;
        case 2:
            players[1] = new X_O_Random_Player_misere<char>('O');
        break;
        case 3:
            players[1] = new X_O_MinMax_Player5X5<char>('O');
        players[1]->setBoard(B);
        break;
        default:
            cout << "Invalid choice for Player O. Exiting the game.\n";
    }

    // Create the game manager and run the game
    GameManager<char> x_o_game(B, players);
    x_o_game.run();

    // Clean up
    delete B;
    for (int i = 0; i < 2; ++i) {
        delete players[i];
    }
}

void setupAndrunUltimateX_O() {
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
    }

    // Create the game manager and start the game
    GameManager<char> game(board, players);
    game.run();

    // Clean up resources
    delete board;
    delete players[0];
    delete players[1];
}

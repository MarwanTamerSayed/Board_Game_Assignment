#include "BoardGame_Classes.h"
#include <bits/stdc++.h>
using namespace std;

template <typename T>
class X_O_Board_misere:public Board<T>
{
public:
    X_O_Board_misere ();
    bool update_board (int x , int y , T symbol);
    void display_board () ;
    bool is_win() ;
    bool is_draw();
    bool game_is_over();
};

template <typename T>
class X_O_Player_misere : public Player<T>
{
public:
    X_O_Player_misere (string name, T symbol);
    void getmove(int& x, int& y) ;
};

template <typename T>
class X_O_Random_Player_misere : public RandomPlayer<T>
{
public:
    X_O_Random_Player_misere (string N ,T symbol);
    void getmove(int &x, int &y) ;
};

//############################# IMPLEMENTATION ###############################################################3

// Constructor for X_O_Board_misere
template <typename T>
X_O_Board_misere<T>::X_O_Board_misere()
{
    this->rows = this->columns = 3;
    this->board = new char*[this->rows];
    for (int i = 0; i < this->rows; i++)
    {
        this->board[i] = new char[this->columns];
        for (int j = 0; j < this->columns; j++)
        {
            this->board[i][j] = 0;
        }
    }
    this->n_moves = 0;
}

template <typename T>
bool X_O_Board_misere<T>::update_board(int x, int y, T mark)
{
    //update if move is valid
    if (!(x < 0 || x >= this->rows || y < 0 || y >= this->columns) && (this->board[x][y] == 0|| mark == 0))
    {
        if (mark == 0)
        {
            this->n_moves--;
            this->board[x][y] = 0;
        }
        else
        {
            this->n_moves++;
            this->board[x][y] = toupper(mark);
        }
        return true;
    }
    return false;
}

// Display the board and the pieces on it
template <typename T>
void X_O_Board_misere<T>::display_board()
{
    for (int i = 0; i < this->rows; i++)
    {
        cout << "\n| ";
        for (int j = 0; j < this->columns; j++)
        {
            cout << "(" << i << "," << j << ")";
            cout << setw(2) << this->board[i][j] << " |";
        }
        cout << "\n-----------------------------";
    }
    cout << endl;
}


template <typename T>
bool X_O_Board_misere<T>::is_win()
{
    // Check rows and columns
    for (int i = 0; i < this->rows; i++)
        {
        if ((this->board[i][0] == this->board[i][1] && this->board[i][1] == this->board[i][2] && this->board[i][0] != 0) ||
            (this->board[0][i] == this->board[1][i] && this->board[1][i] == this->board[2][i] && this->board[0][i] != 0))
        {
            return true;
        }
    }
    // Check diagonals
    if ((this->board[0][0] == this->board[1][1] && this->board[1][1] == this->board[2][2] && this->board[0][0] != 0) ||
        (this->board[0][2] == this->board[1][1] && this->board[1][1] == this->board[2][0] && this->board[0][2] != 0))
    {
        return true;
    }
    return false;
}

// Return true if 9 moves are done and no winner
template <typename T>
bool X_O_Board_misere<T>::is_draw()
{
    return (this->n_moves == 9 && !is_win());
}

template <typename T>
bool X_O_Board_misere<T>::game_is_over()
{
    return is_win() || is_draw();
}

// Constructor for X_O_Player
template <typename T>
X_O_Player_misere<T>::X_O_Player_misere(string name, T symbol) : Player<T>(name, symbol) {}

template <typename T>
void X_O_Player_misere<T>::getmove(int& x, int& y)
{
    while (true)
    {
        cout << "\n>>Please enter your index move row and column (0 to 2) separated by a space:";
        string x1, y1;
        cin >> x1 >> y1;
        bool validInput = true;

        if (x1 == "0" || x1 == "1" || x1 == "2")
        {
            x = stoi(x1);
        }
        else
        {
            cout << "Invalid index move row!" << endl;
            validInput = false;
        }
        if (y1 == "0" || y1 == "1" || y1 == "2")
        {
            y = stoi(y1);
        }
        else
        {
            cout << "Invalid index move column!" << endl;
            validInput = false;
        }
        if (validInput) {
            break;
        }
    }
}

// Constructor for X_O_Random_Player_misere
template <typename T>
X_O_Random_Player_misere<T>::X_O_Random_Player_misere( string N,T symbol) : RandomPlayer<T>(symbol)
{

    this->dimension = 3;
    this->name = N;
    srand(static_cast<unsigned int>(time(0)));
}

template <typename T>
void X_O_Random_Player_misere<T>::getmove(int& x, int& y)
{
    x = rand() % this->dimension;  // Random number between 0 and 2
    y = rand() % this->dimension;
}

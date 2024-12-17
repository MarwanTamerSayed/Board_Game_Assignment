#include "BoardGame_Classes.h"
#include <bits/stdc++.h>
using namespace std;


template <typename T>
class X_O_Board5X5 : public Board<T>
{
public:
    X_O_Board5X5();
    bool update_board(int x, int y, T symbol);
    void display_board();
    bool is_win();
    bool is_draw();
    bool game_is_over();
private:
    int count_threes(T symbol);
};


template <typename T>
class X_O_HumanPlayer : public Player<T>
{
public:
    X_O_HumanPlayer(string name, T symbol);
    void getmove(int& x, int& y);
};

template <typename T>
class X_O_RandomPlayer : public RandomPlayer<T>
{
public:
    X_O_RandomPlayer(T symbol);
    void getmove(int& x, int& y) override;
};


//############# IMPLEMENTATION ##############################################################################


template <typename T>
X_O_Board5X5<T>::X_O_Board5X5()
{
    this->rows = this->columns = 5;
    this->board = new T*[this->rows];
    for (int i = 0; i < this->rows; i++)
    {
        this->board[i] = new T[this->columns];
        for (int j = 0; j < this->columns; j++)
        {
            this->board[i][j] = 0;
        }
    }
    this->n_moves = 0;
}


template <typename T>
bool X_O_Board5X5<T>::update_board(int x, int y, T mark)
{
    if (x >= 0 && x < this->rows && y >= 0 && y < this->columns && this->board[x][y] == 0)
    {
        this->board[x][y] = mark;
        this->n_moves++;
        return true;
    }
    return false;
}


template <typename T>
void X_O_Board5X5<T>::display_board()
{
    for (int i = 0; i < this->rows; i++)
    {
        cout << "\n| ";
        for (int j = 0; j < this->columns; j++)
        {
            cout << "(" << i << "," << j << ")";
            cout << setw(2) << this->board[i][j] << " |";
        }
        cout << "\n--------------------------------------------------";
    }
    cout << endl;
}


template <typename T>
int X_O_Board5X5<T>::count_threes(T symbol)
{
    int countThree = 0;
    for (int i = 0; i < this->rows; i++)// Check rows
    {
        for (int j = 0; j <= this->columns - 3; j++)
        {
            if (this->board[i][j] == symbol && this->board[i][j + 1] == symbol && this->board[i][j + 2] == symbol)
            {
                countThree++;
            }
        }
    }
    for (int j = 0; j < this->columns; j++)// Check columns
    {
        for (int i = 0; i <= this->rows - 3; i++)
        {
            if (this->board[i][j] == symbol && this->board[i + 1][j] == symbol && this->board[i + 2][j] == symbol)
            {
                countThree++;
            }
        }
    }
    for (int i = 0; i <= this->rows - 3; i++)// Check diagonals from left
    {
        for (int j = 0; j <= this->columns - 3; j++)
        {
            if (this->board[i][j] == symbol && this->board[i + 1][j + 1] == symbol && this->board[i + 2][j + 2] == symbol)
            {
                countThree++;
            }
        }
    }
    for (int i = 0; i <= this->rows - 3; i++)// Check diagonals from right
    {
        for (int j = 2; j < this->columns; j++)
        {
            if (this->board[i][j] == symbol && this->board[i + 1][j - 1] == symbol && this->board[i + 2][j - 2] == symbol)
            {
                countThree++;
            }
        }
    }
    return countThree;
}


template <typename T>
bool X_O_Board5X5<T>::is_win()
{
    int X_score = count_threes('X');
    int O_score = count_threes('O');
    cout << "->Scores:\n";
    cout << "- Player X: " << X_score << " points\n";
    cout << "- Player O: " << O_score << " points\n";
    if (X_score > O_score && this->n_moves == 25 )
    {
        cout << "**Player X wins!**\n";
        return true;
    }
    else if (O_score > X_score && this->n_moves == 24)
    {
        cout << "**Player O wins!**\n";
        return true;
    }
    else if (O_score == X_score && this->n_moves == 24)
    {
        cout << "*It's a draw!*\n";
        return false;
    }
    else
    {
        return false;
    }
}


template <typename T>
bool X_O_Board5X5<T>::is_draw()
{
    int X_Score = count_threes('X');
    int O_Score = count_threes('O');
    return (O_Score == X_Score && this->n_moves == 24);
}


template <typename T>
bool X_O_Board5X5<T>::game_is_over()
{
    if (is_draw() || is_win())
    {
        return true;
    }
    return false;
}


template <typename T>
X_O_HumanPlayer<T>::X_O_HumanPlayer(string name, T symbol) : Player<T>(name, symbol) {}


template <typename T>
void X_O_HumanPlayer<T>::getmove(int& x, int& y)
{
    cout << ">>Enter row and column for your move," << this->name << "(" << this->symbol << "):";
    while (true)
    {
        cout << "\n>>Please enter your index move row and column (0 to 4) separated by a space: ";
        string validRow, validColumn;
        cin >> validRow >> validColumn;

        bool validInput = true;

        if (validRow == "0" || validRow == "1" || validRow == "2" || validRow == "3" || validRow == "4" )
        {
            x = stoi(validRow);
        }
        else
        {
            cout << "Invalid index move row!" << endl;
            validInput = false;
        }
        if (validColumn == "0" || validColumn == "1" || validColumn == "2" || validColumn == "3" || validColumn == "4" )
        {
            y = stoi(validColumn);
        }
        else
        {
            cout << "Invalid index move column!" << endl;
            validInput = false;
        }
        if (validInput)
        {
            break;
        }
    }
}


template <typename T>
X_O_RandomPlayer<T>::X_O_RandomPlayer(T symbol) : RandomPlayer<T>(symbol)
{
    this->dimension = 5;
    this->name = "Random Computer Player";
    srand(static_cast<unsigned int>(time(0)));
}


template <typename T>
void X_O_RandomPlayer<T>::getmove(int& x, int& y)
{
    x = rand() % this->dimension;  // Random number between 0 and 2
    y = rand() % this->dimension;
}

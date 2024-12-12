#include "boardgame_classes.h"
#include <string>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
using namespace std;

// Tracks the total number of moves made across all boards
int f_moves = 0;

// Small Tic Tac Toe Board Class
template <typename T>
class Small_XO_Board : public Board<T>
{
private:
    char SymbolWinner;

public:
    Small_XO_Board();
    bool update_board(int x, int y, T symbol) override;
    void display_board() override;
    bool is_win() override;
    bool is_draw() override;
    bool game_is_over() override;
    T get_cell(int row, int col) const;
    T get_char() const;
};

// Ultimate Tic Tac Toe Board Class : 3x3 grid of 3x3 small boards
template <typename T>
class Ultimate_XO_Board : public Board<T>
{
private:
    Small_XO_Board<T> *subBoards[3][3];

public:
    Ultimate_XO_Board();
    ~Ultimate_XO_Board();
    bool update_board(int x, int y, T symbol) override;
    void display_board() override;
    bool is_win() override;
    bool is_draw() override;
    bool game_is_over() override;
};

// Player Classes
template <typename T>
class Ultimate_Player : public Player<T>
{
public:
    Ultimate_Player(string name, T symbol);
    void getmove(int &x, int &y) override;
};

template <typename T>
class Ultimate_Random_Player : public RandomPlayer<T>
{
public:
    Ultimate_Random_Player(T symbol);
    void getmove(int &x, int &y) override;
};

// ===========================Implementation of Small_XO_Board==================

template <typename T>
Small_XO_Board<T>::Small_XO_Board()
{
    this->rows = this->columns = 3;
    this->board = new char *[this->rows];
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
bool Small_XO_Board<T>::update_board(int x, int y, T symbol)
{   
    // Only update if move is valid
    if (!(x < 0 || x >= this->rows || y < 0 || y >= this->columns) && (this->board[x][y] == 0 || symbol == 0))
    {
        if (symbol == 0)
        {
            this->n_moves--;
            this->board[x][y] = 0;
        }
        else
        {
            this->n_moves++;
            this->board[x][y] = toupper(symbol);
        }

        return true;
    }
    return false;
}

//! mlhaaaash lazma 
//! mlhaaaash lazma 
//! mlhaaaash lazma 
template <typename T>
void Small_XO_Board<T>::display_board()
{
    int index = 0; // Start from (0,0) and increment
    for (int i = 0; i < this->rows; i++)
    {
        for (int j = 0; j < this->columns; j++)
        {
            // Display the index as (row, col)
            cout << "(" << index / 3 << ", " << index % 3 << ") ";
            index++;
        }
        cout << endl;
    }
}

template <typename T>
bool Small_XO_Board<T>::is_win()
{
    // Check rows

    for (int i = 0; i < this->rows; i++)
    {
        if ((this->board[i][0] == this->board[i][1] && this->board[i][1] == this->board[i][2] && this->board[i][0] != 0 && this->board[i][0] != '-'))
        {
            // Mark all empty cells as '-' and increment filled moves 
            for (int x = 0; x < this->rows; x++)
            {
                for (int c = 0; c < this->columns; c++)
                {
                    if (this->board[x][c] == 0)
                    {
                        this->board[x][c] = '-';
                        f_moves++;
                        this->n_moves++;
                    }
                }
            }

            SymbolWinner = this->board[i][0];    // Set the winner symbol
            return true;
        }
    }

    // Check columns
    for (int i = 0; i < this->columns; i++)
    {
        if ((this->board[0][i] == this->board[1][i] && this->board[1][i] == this->board[2][i] && this->board[0][i] != 0 && this->board[0][i] != '-'))
        {

            for (int x = 0; x < this->rows; x++)
            {
                for (int c = 0; c < this->columns; c++)
                {
                    if (this->board[x][c] == 0)
                    {
                        this->board[x][c] = '-';
                        f_moves++;
                        this->n_moves++;
                    }
                }
            }

            SymbolWinner = this->board[0][i];
            return true;
        }
    }

    // Check diagonals
    if ((this->board[0][0] == this->board[1][1] && this->board[1][1] == this->board[2][2] && this->board[0][0] != 0 && this->board[0][0] != '-'))
    {

        for (int x = 0; x < this->rows; x++)
        {
            for (int c = 0; c < this->columns; c++)
            {
                if (this->board[x][c] == 0)
                {
                    this->board[x][c] = '-';
                    f_moves++;
                    this->n_moves++;
                }
            }
        }

        SymbolWinner = this->board[0][0];

        return true;
    }

    // Check second diagonal
    if ((this->board[0][2] == this->board[1][1] && this->board[1][1] == this->board[2][0] && this->board[0][2] != 0 && this->board[0][2] != '-'))
    {

        for (int x = 0; x < this->rows; x++)
        {
            for (int c = 0; c < this->columns; c++)
            {
                if (this->board[x][c] == 0)
                {
                    this->board[x][c] = '-';
                    f_moves++;
                    this->n_moves++;
                }
            }
        }

        SymbolWinner = this->board[0][2];

        return true;
    }

    return false;
}

template <typename T>
bool Small_XO_Board<T>::is_draw()
{
    return (this->n_moves == 9 && !is_win());
}

template <typename T>
bool Small_XO_Board<T>::game_is_over()
{
    return is_win() || is_draw();
}

// Return the value stored in the specified cell of the board
template <typename T>
T Small_XO_Board<T>::get_cell(int row, int col) const
{
    return this->board[row][col];
}
// Return the symbol (X or O) that won the board, or 0 if there's no winner yet
template <typename T>
T Small_XO_Board<T>::get_char() const
{
    return SymbolWinner;
}

//  =============Implementation of Ultimate_XO_Board=================
template <typename T>
Ultimate_XO_Board<T>::Ultimate_XO_Board()
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            subBoards[i][j] = new Small_XO_Board<T>();
        }
    }
}

template <typename T>
Ultimate_XO_Board<T>::~Ultimate_XO_Board()
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            delete subBoards[i][j];
        }
    }
}

template <typename T>
bool Ultimate_XO_Board<T>::update_board(int x, int y, T symbol)
{
    // Calculate the sub-board coordinates (3x3 grid) based on the input x, y
    int subBoardX = x / 3;  // Determine the sub-board's row (0-2)
    int subBoardY = y / 3;  // Determine the sub-board's column (0-2)

    // Calculate the local coordinates within the sub-board (0-2)
    int localX = x % 3;
    int localY = y % 3;

    if (subBoards[subBoardX][subBoardY]->update_board(localX, localY, symbol))
    {
        f_moves++;
        return true;
    }
}

template <typename T>
void Ultimate_XO_Board<T>::display_board()
{
    // Display the top-level large XO board
    cout << "Big Board Status (3x3):\n";
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (subBoards[i][j]->is_win())
            {
                char winner = subBoards[i][j]->get_char(); // Get the winner of the small board
                cout << winner << " ";          
            }
            else if (subBoards[i][j]->is_draw())
            {
                cout << 'D' << " "; // Draw in the small board
            }
            else
            {
                cout << '.' << " "; // Small board is still ongoing
            }
        }
        cout << endl;
    }

    cout << "\nDetails of 3x3 Boards:\n";

    // Display all the small boards with indices
    for (int gridRow = 0; gridRow < 3; gridRow++)
    {
        for (int row = 0; row < 3; row++)
        {
            for (int gridCol = 0; gridCol < 3; gridCol++)
            {
                for (int col = 0; col < 3; col++)
                {
                    // Get the cell value from each small board
                    T cell = subBoards[gridRow][gridCol]->get_cell(row, col);

                    // If the cell is empty, display the index as (row, col)
                    if (cell == 0)
                    {
                        cout << "(" << (gridRow * 3 + row) << ", " << (gridCol * 3 + col) << ") ";
                    }
                    else
                    {
                        // Display the player move 'X' or 'O'
                        cout << "   " << cell << "   ";
                    }
                }
                if (gridCol < 2)
                    cout << "|| "; 
            }
            cout << endl;
        }
        if (gridRow < 2)
            cout << "==================================================================\n"; // Separator between rows of small boards
    }
}

// *note
// We use subBoards[x][y]->is_win() and subBoards[x][y]->get_char()
// because subBoards is an array of pointers to Small_XO_Board<T>.
// We can't compare objects of type Small_XO_Board<T>* directly for equality using ==.

template <typename T>
bool Ultimate_XO_Board<T>::is_win()
{
    // Check rows and columns of the big board
    for (int i = 0; i < 3; i++)
    {
        // Check row
        if (subBoards[i][0]->is_win() &&
            subBoards[i][1]->is_win() &&
            subBoards[i][2]->is_win() &&
            subBoards[i][0]->get_char() != 0 &&
            subBoards[i][0]->get_char() == subBoards[i][1]->get_char() &&
            subBoards[i][1]->get_char() == subBoards[i][2]->get_char())
        {
            return true;
        }

        // Check column
        if (subBoards[0][i]->is_win() &&
            subBoards[1][i]->is_win() &&
            subBoards[2][i]->is_win() &&
            subBoards[0][i]->get_char() != 0 &&
            subBoards[0][i]->get_char() == subBoards[1][i]->get_char() &&
            subBoards[1][i]->get_char() == subBoards[2][i]->get_char())
        {
            return true;
        }
    }

    // Check diagonals of the big board
    if (subBoards[0][0]->is_win() &&
        subBoards[1][1]->is_win() &&
        subBoards[2][2]->is_win() &&
        subBoards[0][0]->get_char() != 0 &&
        subBoards[0][0]->get_char() == subBoards[1][1]->get_char() &&
        subBoards[1][1]->get_char() == subBoards[2][2]->get_char())
    {
        return true;
    }

    if (subBoards[0][2]->is_win() &&
        subBoards[1][1]->is_win() &&
        subBoards[2][0]->is_win() &&
        subBoards[0][2]->get_char() != 0 &&
        subBoards[0][2]->get_char() == subBoards[1][1]->get_char() &&
        subBoards[1][1]->get_char() == subBoards[2][0]->get_char())
    {
        return true;
    }

    return false;
}

template <typename T>
bool Ultimate_XO_Board<T>::is_draw()
{
    return (f_moves == 81 && !is_win());
}

template <typename T>
bool Ultimate_XO_Board<T>::game_is_over()
{
    return is_win() || is_draw();
}

// Implementation of Ultimate_Player
template <typename T>
Ultimate_Player<T>::Ultimate_Player(string name, T symbol) : Player<T>(name, symbol) {}

template <typename T>
void Ultimate_Player<T>::getmove(int &x, int &y)
{
    cout << "\nPlease enter your move x and y (0 to 8) separated by spaces: ";
    cin >> x >> y;
}

// Implementation of Ultimate_Random_Player
template <typename T>
Ultimate_Random_Player<T>::Ultimate_Random_Player(T symbol) : RandomPlayer<T>(symbol) {}

template <typename T>
void Ultimate_Random_Player<T>::getmove(int &x, int &y)
{
    x = rand() % 9;
    y = rand() % 9;
}

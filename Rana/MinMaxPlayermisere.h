#include "BoardGame_Classes.h"
#include <bits/stdc++.h>
using namespace std;

template <typename T>
class X_O_MinMax_Player5X5 : public Player<T> {
public:
    X_O_MinMax_Player5X5(T symbol);
    void getmove(int& x, int& y) override;
private:
    int calculateMinMax(T s, bool isMaximizing);
    pair<int, int> getBestMove();
};

//############################# IMPLEMENTATION #################################33

template <typename T>
X_O_MinMax_Player5X5<T>::X_O_MinMax_Player5X5(T symbol) : Player<T>(symbol) {
    this->name = "AI Player";
}

template <typename T>
void X_O_MinMax_Player5X5<T>::getmove(int& x, int& y)
{
    pair<int, int> bestMove = getBestMove();
    x = bestMove.first;
    y = bestMove.second;
}

template <typename T>
int X_O_MinMax_Player5X5<T>::calculateMinMax(T s, bool isMaximizing)
{
    if (this->boardPtr->is_win()) {
        return isMaximizing ? -10 : 10;
    }
    else if (this->boardPtr->is_draw())
    {
        return 0;
    }
    int bestValue = isMaximizing ? numeric_limits<int>::min() : numeric_limits<int>::max();
    T opponentSymbol = (s == 'X') ? 'O' : 'X';
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (this->boardPtr->update_board(i, j, s))
            {
                int value = calculateMinMax(opponentSymbol, !isMaximizing);
                this->boardPtr->update_board(i, j, 0); // Undo move

                if (isMaximizing)
                {
                    bestValue = max(bestValue, value);
                }
                else
                {
                    bestValue = min(bestValue, value);
                }
            }
        }
    }
    return bestValue;
}

template <typename T>
pair<int, int> X_O_MinMax_Player5X5<T>::getBestMove()
{
    int bestValue = numeric_limits<int>::min();
    pair<int, int> bestMove = {-1, -1};
    for (int x = 0; x < 3; x++)
    {
        for (int y = 0; y < 3; y++)
        {
            if (this->boardPtr->update_board(x, y, this->symbol))
            {
                if (this->boardPtr->is_win())
                {
                    this->boardPtr->update_board(x, y, 0);
                    continue; 
                }
                int moveValue = calculateMinMax(this->symbol, false);
                this->boardPtr->update_board(x, y, 0);
                if (moveValue > bestValue)
                {
                    bestMove = {x, y};
                    bestValue = moveValue;
                }
            }
        }
    }
    return bestMove;
}

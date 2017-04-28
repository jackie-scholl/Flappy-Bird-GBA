#ifndef TICTACTOE_H
#define TICTACTOE_H

#include <vector>

class TicTacToe
{
    std::vector<int> boardState;
    short sumWithStride( short startAt, short stride ) const;
    char  getWinner( short sum ) const ; // returns 'X','O' if sum=3,-3 respectively, else returns ' '
    //short getOpenCount() const;
    short userInput();

public:
    TicTacToe();
    ~TicTacToe();

    char getAt(short c, short r) const; // DONE TODO: (8) add "const" everywhere that is appropriate
    void setAt(short c, short r, char l);

    short getOpenCount() const;
    char getWinner() const; // returns 'X' or 'O' respectively if there is a winner, else ' '
    void printState() const;
    void playGameOnCommandLine();

    bool operator==(const TicTacToe &other) const;
};

short getSquare(TicTacToe board);

#endif // TICTACTOE_H

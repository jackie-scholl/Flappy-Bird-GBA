#include "TicTacToe.h"
#include "TicTacToeVisuals.hpp"
//#include <iostream>

#include <unordered_map>

std::vector<TicTacToe> getPossibleNexts(TicTacToe board, char player);
short getValue(TicTacToe board, char player);


/*int main( )
{
}*/

struct KeyHasher {
  std::size_t operator()(const TicTacToe& board) const {
    using std::size_t;
    using std::hash;

    int sum = 0;
    for (int i=0; i<3; i++) {
      for (int j=0; j<3; j++) {
        char val = board.getAt(i, j);
        int temp = (val == 'X')? 1 : (val == 'O')? 2 : 0;
        sum += temp;
        sum *= 3;
      }
    }

    return hash<int>()(sum);
    // Compute individual hash values for first,
    // second and third and combine them using XOR
    // and bit shifting:

    //return ((hash<string>()(k.first)
    //         ^ (hash<string>()(k.second) << 1)) >> 1)
    //         ^ (hash<int>()(k.third) << 1);
  }
};

static std::unordered_map<TicTacToe, short, KeyHasher> values_map;


int main(int argc, char **argv) {
    while(1) {
      TicTacToe myGame;
      myGame.playGameOnCommandLine();
    }

    return 0;
}

short getSquare(TicTacToe board) {
  if (board.getOpenCount() == 9) {
    return 0;
  } else if (board.getOpenCount() == 8) {
    if (board.getAt(1, 1) == ' ') {
      return 4;
    } else {
      return 0;
    }
  }
  char player = board.getOpenCount()%2==1? 'X' : 'O';

  short expectedVal = getValue(board, player); // assume it's the player's turn

  for (int i=0; i<3; i++) {
    for (int j=0; j<3; j++) {
      char val = board.getAt(i, j);
      if (val == ' ') {
        TicTacToe next = board;
        next.setAt(i, j, player);

        char opposite_player = (player == 'X' ? 'O' : 'X');
        short val = getValue(next, opposite_player); // assume it's the other player's turn
        if (val == expectedVal) {
          // we've found something good
          return 3*i+j;
        }
      }
    }
  }

  //std::cout << "nothing good" << std::endl;
  return -1;
}

short getNegamax(TicTacToe board) {
  if (values_map.find(board) != values_map.end()) {
    return values_map[board];
  }

  char player = board.getOpenCount()%2==1? 'X' : 'O';

  char winner = board.getWinner();
  if (winner != ' ') {
    short val = ((winner == player) ? 1 : -1);
    return val;
  }
  if (board.getOpenCount() == 0) {
    return 0;
  }

  //char opposite_player = (player == 'X' ? 'O' : 'X');
  std::vector<TicTacToe> possibleNext = getPossibleNexts(board, player);

  short best = -1000;
  for (TicTacToe next : possibleNext) {
    short val = - getNegamax(next/*, opposite_player*/);
    best = std::max(val, best);
  }

  values_map[board] = best;
  //values_map.insert(board, best);

  return best;
}

short getValue(TicTacToe board, char player) {
  short val = getNegamax(board/*, player*/);
  if (player == 'X') {
    return val;
  } else {
    return -val;
  }
}

std::vector<TicTacToe> getPossibleNexts(TicTacToe board, char player) {
  std::vector<TicTacToe> nexts;
  for (short i=0; i<3; i++) {
    for (short j=0; j<3; j++) {
      char val = board.getAt(i, j);
      if (val == ' ') {
        TicTacToe next = board;
        next.setAt(i, j, player);
        nexts.push_back(next);
      }
    }
  }
  return nexts;
}

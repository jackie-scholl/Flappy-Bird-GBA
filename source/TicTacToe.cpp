//#include <iostream>
#include "TicTacToe.h"
//#include "TicTacToeVisuals.hpp"
#include "TicTacToeVisuals2.hpp"
//#include "tonc_demo_lib/input.h"
#include "tonc_demo_lib/toolbox.h"

#include "TicTacToeAI.hpp"

#include "TicTacToeInput.hpp"

#include "Timer.hpp"

TicTacToe::TicTacToe()
{
    // Note, the locations on the tic-tac-toe board are mapped (indexed) as below:

    //    |   |
    //  0 | 1 | 2
    // ___________
    //    |   |
    //  3 | 4 | 5
    // ___________
    //    |   |
    //  6 | 7 | 8
    //    |   |

    // DONE TODO: (2) Initialize 'boardState' so that spaces are returned by any valid getAt call
    boardState.resize(9,0);
}

TicTacToe::~TicTacToe()
{
}

short TicTacToe::sumWithStride( short startAt, short stride ) const {
    short sum = 0;
    // DONE TODO: (6) There is a conceptual bug below. Fix it!!!
    //for ( unsigned i = startAt; i < boardState.size(); i+=stride ) {
    //    sum += boardState[i];
    //}
    sum  = boardState[startAt         ];
    sum += boardState[startAt+  stride];
    sum += boardState[startAt+2*stride];
    return sum;
}

char TicTacToe::getAt(short c, short r) const {
    short valAtLoc = boardState[3*r+c];
    if (0>c || c>2)        {return '?';}
    if (0>r || r>2)        {return '?';}
    if (  valAtLoc ==  0 ) {return ' ';}
    if (  valAtLoc ==  1 ) {return 'X';}
    if (  valAtLoc == -1 ) {return 'O';}
    return                         ' '; // TODO: (7) If there is a better way, do so.
}

short TicTacToe::getOpenCount() const {
    short sum = 0;
    for( unsigned i=0; i<boardState.size(); i++ ) {
        if ( boardState[i] == 0 ) sum++;
    }
    return sum;
}

void TicTacToe::setAt(short c, short r, char l) {
    // DONE TODO: (1) Fill this in such that it is consistent with the above function
    if      (l == ' ') { boardState[3*r+c] =  0; }
    else if (l == 'X') { boardState[3*r+c] =  1; }
    else if (l == 'O') { boardState[3*r+c] = -1; }
}

// DONE TODO (3) add an implementation of getWinner( short sum ) here
char TicTacToe::getWinner( short sum ) const { // returns 'X','O' if sum=3,-3 respectively, else returns ' '
    if      ( sum >=  3 ) return 'X';
    else if ( sum <= -3 ) return 'O';
    else                  return ' ';
}

char TicTacToe::getWinner() const {
    char check;
    // check columns
    check = getWinner( sumWithStride(0,3) ); if ( check != ' ' ) { return check; }
    check = getWinner( sumWithStride(1,3) ); if ( check != ' ' ) { return check; }
    check = getWinner( sumWithStride(2,3) ); if ( check != ' ' ) { return check; }

    // check rows
    // DONE TODO: (4)
    check = getWinner( sumWithStride(0,1) ); if ( check != ' ' ) { return check; }
    check = getWinner( sumWithStride(3,1) ); if ( check != ' ' ) { return check; }
    check = getWinner( sumWithStride(6,1) ); if ( check != ' ' ) { return check; }

    // check diagonals
    check = getWinner( sumWithStride(2,2) ); if ( check != ' ' ) { return check; }
    // DONE TODO: (5) check the other diagonal
    check = getWinner( sumWithStride(0,4) ); if ( check != ' ' ) { return check; }

    // return if nothing found
    return ' ';
}

bool TicTacToe::operator==(const TicTacToe &other) const {
  return (boardState == other.boardState);
}

/*#define REG_VCOUNT *(vu16*)0x04000006

void vid_vsync() {
    while(REG_VCOUNT >= 160) {}   // wait till VDraw
    while(REG_VCOUNT < 160) {}    // wait till VBlank
}*/

void TicTacToe::printState() const {
    md5_update();
    md5_print();
    vid_vsync();

    for (int i=0; i<3; i++) {
      for (int j=0; j<3; j++) {
        char atPosition = getAt(i, j);
        if (atPosition == 'X') {
          draw_x2(i, j);
        } else if (atPosition == 'O') {
          draw_o2(i, j);
        }
      }
    }

}

void TicTacToe::playGameOnCommandLine() {
    // Not the nicest looking code. This is a quickly thrown together proof of concept.
    // Also, note, that the code in places is durty on purpose so that you may see some concepts that may not be common.
    short openCount, c, r;

    init_time_and_md5();
    init_screen2();

    while( (openCount=getOpenCount())>0 and getWinner() == ' ' ) {
        printState();

        if (openCount%2==1) { /*std::cout << "It is X's move " << std::endl;*/ }
        else                { /*std::cout << "It is O's move " << std::endl;*/ }

        if (openCount%2==1) {
          //std::cout << "Please enter column and row numbers separated by space, e.g., 1 2 or 3 1 " << std::endl;
          //std::cin >> c >> r;
          //c = 0;
          //r = 0;
          //highlight(0, 1, CLR_LIME);
          short val = getUserInputSquare(*this);
          //short val = getSquare(*this); // 'X'
          c = val / 3;
          r = val % 3;
          //highlight(0, 2, CLR_LIME);
        } else  {
          //highlight(3, 2, CLR_MAG);
          short val = getSquare(*this); // 'O'
          c = val / 3;
          r = val % 3;
          //highlight(3, 2, CLR_WHITE);
        }

        if ( getAt(c,r) == ' ' ) {
            // record the move
            if (openCount%2==1) { setAt(c,r,'X'); }
            else                { setAt(c,r,'O'); }
        }
        //highlight(3, 2, CLR_YELLOW);
        /*for (int i=0; i<60; i++) {
          //vid_vsync();
          printState();
        }*/
        //highlight(2, 2, CLR_WHITE);
    }
    printState();
    if     ( getWinner() == 'X' ) { game_end2(+1); }
    else if( getWinner() == 'O' ) { game_end2(-1); }
    else                          { game_end2( 0); }

    do {
      key_poll();
    } while (!key_hit(KEY_START) && !key_hit(KEY_SELECT));

}

#include "TicTacToe.h"

#include "TextDemo.hpp"
#include "Timer.hpp"

#include "TicTacToeInput.hpp"

int main(int argc, char **argv) {
    //print_time();
    while(1) {
      TicTacToe myGame;
      myGame.playGameOnCommandLine();
    }

    return 0;
}

#include "TicTacToe.h"

#include "TextDemo.hpp"
#include "Timer.hpp"

int main(int argc, char **argv) {
    while(1) {
      TicTacToe myGame;
      myGame.playGameOnCommandLine();
    }

    return 0;
}

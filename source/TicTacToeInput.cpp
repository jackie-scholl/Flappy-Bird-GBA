#include <stdio.h>

#include "tonc_demo_lib/toolbox.h"
//#include "tonc_demo_lib/input.h"

#include "TicTacToe.h"
#include "TicTacToeVisuals2.hpp"

#include "TicTacToeInput.hpp"

static int cursor_x = 0, cursor_y = 0;

short getUserInputSquare(TicTacToe board) {
  key_poll();
  while (!key_hit(KEY_A)) {
    cursor_x += bit_tribool(key_hit(-1), KI_RIGHT, KI_LEFT);
    cursor_y += bit_tribool(key_hit(-1), KI_DOWN, KI_UP);

    // if (key_hit(KEY_LEFT))
    //   cursor_x -= 1;
    // if (key_hit(KEY_RIGHT))
    //   cursor_x += 1;
    // if (key_hit(KEY_DOWN))
    //   cursor_y += 1;
    // if (key_hit(KEY_UP))
    //   cursor_y -= 1;

    cursor_x = (cursor_x + 3) % 3;
    cursor_y = (cursor_y + 3) % 3;

    board.printState();
    draw_highlight2(cursor_x, cursor_y);

    key_poll();
  }
  return 3*cursor_x + cursor_y;
}

void wait_for_restart() {
  do {
    key_poll();
  } while (!key_is_down(KEY_START) && !key_is_down(KEY_SELECT));
}

#include <gba_console.h>
#include <gba_video.h>
#include <gba_interrupt.h>
#include <gba_systemcalls.h>

#include <stdio.h>

#include "TextDemo.hpp"


/*int main_1() {
//---------------------------------------------------------------------------------

	// the vblank interrupt must be enabled for VBlankIntrWait() to work
	// since the default dispatcher handles the bios flags no vblank handler
	// is required
	irqInit();
	irqEnable(IRQ_VBLANK);

	consoleDemoInit();

	// ansi escape sequence to clear screen and home cursor
	// /x1b[line;columnH
	iprintf("\x1b[2J");

	// ansi escape sequence to set print co-ordinates
	// /x1b[line;columnH
	iprintf("\x1b[10;10HHello World!");

	// ansi escape sequence to move cursor up
	// /x1b[linesA
	iprintf("\x1b[10ALine 0");

	// ansi escape sequence to move cursor left
	// /x1b[columnsD
	iprintf("\x1b[28DColumn 0");

	// ansi escape sequence to move cursor down
	// /x1b[linesB
	iprintf("\x1b[19BLine 19");

	// ansi escape sequence to move cursor right
	// /x1b[columnsC
	iprintf("\x1b[5CColumn 20");

	while (1) {
		VBlankIntrWait();
	}
}*/

void init_text() {
  irqInit();
  irqEnable(IRQ_VBLANK);

  consoleInit(	0,		// charbase
					4,		// mapbase
					1,		// background number
					NULL,	// font
					0, 		// font size
					15		// 16 color palette
	);

	// set the screen colors, color 0 is the background color
	// the foreground color is index 1 of the selected 16 color palette
	BG_COLORS[0]=RGB8(58,110,165);
	//BG_COLORS[241]=RGB5(31,31,31);
  BG_COLORS[241]=RGB5(17,17,17);

	//SetMode(MODE_0 | BG0_ON);

	REG_DISPCNT |= BG1_ON; // Background 1 on

  REG_BG1CNT |= /*BG_CBB(CBB_0) | BG_SBB(SBB_0) | BG_REG_32x32 |*/ BG_PRIORITY(1);//BG_REG_64x64;

  //iprintf("\x1b[2J");
}

// void printText(char* text) {
//   iprintf(text);
// }

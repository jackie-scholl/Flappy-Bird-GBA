#include <stdio.h>

#include "tonc_demo_lib/toolbox.h"

#include "Timer.hpp"
#include "TextDemo.hpp"
#include "md5.h"

#include <string.h>

void md5_init();

void init_time_and_md5() {
	  REG_TM0D = 0;
	  REG_TM0CNT = TM_ENABLE | TM_FREQ_SYS;

	  REG_TM1D = 0;
	  REG_TM1CNT = TM_ENABLE | TM_FREQ_SYS | TM_CASCADE;

	  REG_TM2D = 0;
	  REG_TM2CNT = TM_ENABLE | TM_FREQ_SYS | TM_CASCADE;

		md5_init();

}

void print_time() {
  u16 timer0 = REG_TM0D;
  u16 timer1 = REG_TM1D;
  u16 timer2 = REG_TM2D;

  printf("\x1b[2;0H%04X \n%04X \n%04X \n", timer0, timer1, timer2);
}

void reset_time() {
  REG_TM2CNT ^= TM_ENABLE;
  REG_TM2CNT ^= TM_ENABLE;

  REG_TM1CNT ^= TM_ENABLE;
  REG_TM1CNT ^= TM_ENABLE;

  REG_TM0CNT ^= TM_ENABLE;
  REG_TM0CNT ^= TM_ENABLE;
}

char data [100];
uint8_t result [16];

void md5_init() {
	memset(data, 0, sizeof(data));
	memset(result, 0, sizeof(result));
}

void md5_update() {
	memcpy(data, result, 16);
	//const size_t REG_BASE = 0x04000000;
	memcpy(data + 16, (void*)&(REG_TM0D), 16); // Timers
	memcpy(data + 32, (void*)&(REG_KEYINPUT), 2);  // Keypad
	md5_2((const uint8_t*) data, sizeof(data), result);
}


void md5_print() {
	// uint8_t *p;
	//
	// p=(uint8_t *)&h0;
	// printf("%2.2x%2.2x%2.2x%2.2x\n", p[0], p[1], p[2], p[3]);
	//
	// p=(uint8_t *)&h1;
	// printf("%2.2x%2.2x%2.2x%2.2x\n", p[0], p[1], p[2], p[3]);
	//
	// p=(uint8_t *)&h2;
	// printf("%2.2x%2.2x%2.2x%2.2x\n", p[0], p[1], p[2], p[3]);
	//
	// p=(uint8_t *)&h3;
	// printf("%2.2x%2.2x%2.2x%2.2x\n", p[0], p[1], p[2], p[3]);
	// //puts("");
	//
	//
	// printf("\x1b[4A");
}

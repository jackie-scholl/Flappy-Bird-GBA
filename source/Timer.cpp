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

static void print_single_hex(uint8_t nybble) {
	putchar(nybble >= 0xA ? nybble + 0x37 : nybble + 0x30);
}

static void print_single_time(u16 time) {
	print_single_hex((time >> 12) & 0xF);
	print_single_hex((time >>  8) & 0xF);
	print_single_hex((time >>  4) & 0xF);
	print_single_hex((time >>  0) & 0xF);
	putchar('\n');
}

static
void print_time() {
  u16 timer0 = REG_TM0D;
  u16 timer1 = REG_TM1D;
  u16 timer2 = REG_TM2D;

	//printf("hello\n");

	fputs("\x1b[2;0H", stdout);
	//print_single_time(0x1234);
	//print_single_time(0x9ABC);
	print_single_time(timer0);
	print_single_time(timer1);
	print_single_time(timer2);
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

static char data [100];
static uint8_t result [16];

void md5_init() {
	memset(data, 0, sizeof(data));
	memset(result, 0, sizeof(result));
}

void md5_update() {
	memcpy(data, result, 16);
	memcpy(data + 16, (void*)&(REG_TM0D), 16); // Timers
	memcpy(data + 32, (void*)&(REG_KEYINPUT), 2);  // Keypad
	md5_2((const uint8_t*) data, sizeof(data), result);
}

static void print_single_md5_byte(const uint8_t byte) {
	print_single_hex((byte >> 4) & 0xF);
	print_single_hex((byte >> 0) & 0xF);
}

void md5_print() {
	fputs("\x1b[2;0H", stdout);
	for (int i=0; i<16; i++) {
		print_single_md5_byte(result[i]);
		if (i % 4 == 3) {
			putchar('\n');
		}
	}
	//putchar('\n');
}

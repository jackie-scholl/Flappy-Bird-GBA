//
// second.c
// Cleaner version of 'first.c'
//
// (20060211 - 20060922, cearn)

#include "TicTacToeVisuals2.hpp"
#include "tonc_demo_lib/toolbox.h"

#include "TextDemo.hpp"

#include <string.h>
#include <stdio.h>

#define CBB_0  0
#define SBB_0 28

#define CBB_1  1
#define SBB_1 29

BG_POINT bg0_pt= { 0, 0 };
SCR_ENTRY *bg0_map= se_mem[SBB_0];
SCR_ENTRY *bg1_map= se_mem[SBB_1];


uint se_index(uint tx, uint ty, uint pitch)
{
    uint sbb= ((tx>>5)+(ty>>5)*(pitch>>5));
    return sbb*1024 + ((tx&31)+(ty&31)*32);
}

#define ARRAY_SIZE(foo) (sizeof(foo)/sizeof(foo[0]))

void init_map0(int paletteBank) {
    // initialize a background
    REG_BG0CNT= BG_CBB(CBB_0) | BG_SBB(SBB_0) | BG_REG_32x32 | BG_PRIO(2);//BG_REG_64x64;
    REG_BG0HOFS= 0;
    REG_BG0VOFS= 0;

    // (1) create the tiles: basic tile and a cross
    const TILE tiles[]=
    {
        {{0x11111111, 0x11111111, 0x11111111, 0x11111111,
          0x11111111, 0x11111111, 0x11111111, 0x11111111}},
        {{0x00000000, 0x00000000, 0x00000000, 0x00000000,
          0x00000000, 0x00000000, 0x00000000, 0x00000000}},
        {{0x00000011, 0x00000011, 0x00000011, 0x00000011,  // vertical left
          0x00000011, 0x00000011, 0x00000011, 0x00000011}},
        {{0x00001100, 0x00001100, 0x00001100, 0x00001100,  // vertical right
          0x00001100, 0x00001100, 0x00001100, 0x00001100}},
        {{0x00000000, 0x00000000, 0x00000000, 0x00000000,  // horizontal top
          0x00000000, 0x11111111, 0x11111111, 0x00000000}},
        {{0x00000000, 0x00000000, 0x11111111, 0x11111111,  // horizontal bottom
          0x00000000, 0x00000000, 0x00000000, 0x00000000}},
    };
    for (unsigned int i = 0; i < ARRAY_SIZE(tiles); i++) {
      tile_mem[CBB_0][i] = tiles[i];
    }

    // (2) create a palette
    pal_bg_bank[0][1]= RGB15(31,  0,  0);
    pal_bg_bank[1][1]= RGB15(15, 15, 15);
    pal_bg_bank[2][1]= RGB15( 0,  0, 31);
    pal_bg_bank[3][1]= RGB15(31, 31, 31);
    pal_bg_bank[4][1]= RGB15( 0,  0,  0);

    SCR_ENTRY *pse= bg0_map;
    for(int i=0; i<32; i++) {
      for (int j=0; j<32; j++) {
        SCR_ENTRY tile;
        bool b = j < 5 || j >= 25 || j == 11 || j == 18 || i == 6 || i == 13;

        tile = SE_PALBANK(b? paletteBank : 3) | 0;
        *pse++= tile;
      }
    }
}

void set_up_x() {
    int base = 0;
    for (int i=0; i < 16; i++) {
      if ( i / 4 == 3 || i % 4 == 3) {
        continue;
      }
      TILE t = tile_mem[4][base+i];

      if (i / 4 == i % 4) {
        for (int j=0; j<8; j++) {
          if (i != 0 || j > 2) {
            t.data[j] |= (1 << j*4);
          }
        }
      }

      tile_mem[4][base+i] = t;
    }
}

void set_up_highlight() {
    int base = 32;
    for (int i=0; i < 16; i++) {
      if ( i / 4 == 3 || i % 4 == 3) {
        continue;
      }
      TILE t = tile_mem[4][base+i];

      if (i % 4 == 0) {
        for (int j=0; j<8; j++) {
          t.data[j] |= 0x00000001;
        }
      }
      if (i / 4 == 0) {
        t.data[0] |= 0x11111111;
      }

      tile_mem[4][base+i] = t;
    }
}

void putpixel(int x, int y) {
  if (x == 2 || y == 2) { // cut off tips
    return;
  }

  tile_mem[4][16 + (x/8) + (y/8)*4].data[y%8] |= (1 << ((x%8)*4));
}

void drawcircle(int x0, int y0, int radius) {
    int x = radius;
    int y = 0;
    int err = 0;

    while (x >= y) {
        putpixel(x0 - x, y0 - y);
        putpixel(x0 - y, y0 - x);

        if (err <= 0) {
            y += 1;
            err += 2*y + 1;
        } else {
            x -= 1;
            err -= 2*x + 1;
        }
    }
}

void set_up_o() {
    drawcircle(24, 24, 22);
}

OBJ_ATTR obj_buffer[128];
OBJ_AFFINE *obj_aff_buffer= (OBJ_AFFINE*)obj_buffer;


static const unsigned int metrPal2[]=
{
	0x001F4210,0x008C0000,0x04B629DA,0x0000004D,0x031C0218,0x00005F5D,0x01800100,0x00000240,
	0x46314210,0x4E734A52,0x56B55294,0x5EF75AD6,0x67396318,0x6F7B6B5A,0x77BD739C,0x7FFF7BDE,

	0x001F0000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000, // RED
	0x7C000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000, // BLUE
	0x03E00000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000, // GREEN
};

void init_sprites() {
  // (1) Places the tiles of a 4bpp boxed metroid sprite
  //   into LOW obj memory (cbb == 4)
  memcpy(pal_obj_mem, metrPal2, sizeof(metrPal2));
  memset(&tile_mem[4][0], 0, 2048);

  // (2) Initialize all sprites
  oam_init(obj_buffer, 128);

  set_up_x();
  set_up_o();
  set_up_highlight();
}


void draw_thing(int i, int j, int paletteBank, int baseTid, bool isHighlight) {
  int x = 40 + 56*i;
  int y =  0 + 56*j;
  u32 tid = baseTid;

  int obj_buffer_index = isHighlight? 0 : 4 + (i + j*3)*4;

  for (int i=0; i<4; i++) {
    OBJ_ATTR *metr= &obj_buffer[obj_buffer_index+i];

    u16 flippy = (i%2 ? ATTR1_HFLIP : 0) | (i/2 ? ATTR1_VFLIP : 0);

    obj_set_attr(metr,
        ATTR0_SQUARE,              // Square, regular sprite
        ATTR1_SIZE_32 | flippy,              // 32x32p,
        ATTR2_PALBANK(paletteBank) | tid | ATTR2_PRIO(2));   // palbank paletteBank, tile 0

    obj_set_pos(metr, x + (i%2)*16, y + (i/2)*16);
  }

  oam_copy(oam_mem+obj_buffer_index, obj_buffer+obj_buffer_index, 4);   // (6) Update OAM (only one now)
}

void draw_x2(int i, int j) {
  draw_thing(i, j, 3, 0, false);
}

void draw_o2(int i, int j) {
  draw_thing(i, j, 2, 16, false);
}

void draw_highlight2(int i, int j) {
  draw_thing(i, j, 4, 32, true);
}

void init_screen2() {
  init_text();

  init_map0(4);

	REG_DISPCNT = (REG_DISPCNT & DCNT_BG1) | DCNT_MODE0 | DCNT_BG0 /*| DCNT_BG1*/ | DCNT_OBJ | DCNT_OBJ_1D;

  init_sprites();

  printf("Hello\nWorld\n");

}

void game_end2(int result) {
  int paletteBank = result + 1;
  init_map0(paletteBank);
  printf("GAME\nEND\n");

}

int main_0() {
	init_screen2();

  draw_x2(0, 1);
  draw_o2(1, 1);
  draw_highlight2(0, 0);


	while(1) {
    vid_vsync();
    key_poll();
	}

	while(1);

	return 0;
}

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

//#define BLANK_FILL ((SCREEN_WIDTH-SCREEN_HEIGHT)/2 + 1)

#define CBB_0  0
#define SBB_0 28

#define CBB_1  1
#define SBB_1 29

#define CROSS_TX 15
#define CROSS_TY 10

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
    //pal_bg_bank[0][1]= RGB15(31, 31, 31);
    pal_bg_bank[0][1]= RGB15(31,  0,  0);

    //pal_bg_bank[1][1]= RGB15(31, 31, 31);
    pal_bg_bank[1][1]= RGB15(15, 15, 15);

    //pal_bg_bank[2][1]= RGB15(31, 31, 31);
    pal_bg_bank[2][1]= RGB15( 0,  0, 31);

    //pal_bg_bank[3][1]= RGB15(31, 31, 31);
    pal_bg_bank[3][1]= RGB15(31, 31, 31);

    pal_bg_bank[4][1]= RGB15( 0,  0,  0);

    // (3) Create a map: four contingent blocks of
    //   0x0000, 0x1000, 0x2000, 0x3000.
    SCR_ENTRY *pse= bg0_map;
    for(int i=0; i<32; i++) {
      for (int j=0; j<32; j++) {
        SCR_ENTRY tile;
        //tile = SE_PALBANK(3) | ((j / 5) % ARRAY_SIZE(tiles));
        //bool b = j < 5 || j >= 25 || j == 11 || j == 18 || i == 6 || i == 13;
        //int b = (j < 5 || j >= 25)? 2 : (j == 11 || j == 18 || i == 6 || i == 13)? 1 : 0;

        //tile = SE_PALBANK(b? paletteBank : 0);
        if (j < 5 || j >= 25) {
          tile = SE_PALBANK(paletteBank) | 0;
        } else if (j == 11 || j == 18 || i == 6 || i == 13) {
          tile = SE_PALBANK(paletteBank) | 0;
        } else {
          tile = SE_PALBANK(3) | 0;
        }
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

	//0x001F0000,0x03E07C00,0x18C61084,0x294A2108,0x00000000,0x00000000,0x00000000,0x00000000,
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
  setup();

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
  //main_();

	init_screen2();

  draw_x2(0, 1);
  draw_o2(1, 1);
  draw_highlight2(0, 0);

  //game_end2(-1);

	while(1) {
    vid_vsync();
    key_poll();
	}

	while(1);

	return 0;
}














/*void screen_fill(COLOR color) {
	for (int x = 0; x < SCREEN_WIDTH; x++) {
		for (int y=0; y < SCREEN_HEIGHT; y++) {
			m3_plot(x, y, CLR_WHITE);
		}
	}
}

void horizontal_line(int y, COLOR color) {
	for (int x=0; x < SCREEN_WIDTH; x++) {
		m3_plot(x, y, color);
	}
}

void vertical_line(int x, COLOR color) {
	for (int y=0; y < SCREEN_HEIGHT; y++) {
		m3_plot(x, y, color);
	}
}

void draw_x(int i, int j, COLOR color) {
	int base_x = BLANK_FILL + 54*i;
	int base_y = 54*j;
	for (int i=4; i<48; i++) {
		m3_plot(base_x+i, base_y+i, color);
		m3_plot(base_x+52-i, base_y+i, color);
	}
}

void draw_o(int i, int j, COLOR color) {
	int base_x = BLANK_FILL + 54*i;
	int base_y = 54*j;
	for (int i=0; i<52; i++) {
		for (int j=0; j<52; j++) {
			int t = (i-26)*(i-26) + (j-26)*(j-26);
			int radius = 20;
			int radiusSquared = radius*radius;
			int allowance = radius;
			int diff = abs(t - radiusSquared);
			if (diff < allowance) {
				m3_plot(base_x+i, base_y+j, color);
			}
		}
	}
}

void highlight(int i, int j, COLOR color) {
	int base_x = BLANK_FILL + 54*i;
	int base_y = 54*j;
	for (int i=0; i<52; i++) {
		m3_plot(base_x+i, base_y, color);
		m3_plot(base_x+i, base_y+51, color);
		m3_plot(base_x, base_y+i, color);
		m3_plot(base_x+51, base_y+i, color);
	}
}

void init_screen() {
	REG_DISPCNT= DCNT_MODE3 | DCNT_BG2;

	screen_fill(CLR_WHITE);

	int blank_fill = BLANK_FILL; // 40
	for (int i=0; i<blank_fill; i++) {
		vertical_line(i, CLR_BLACK);
		vertical_line(SCREEN_WIDTH - i, CLR_BLACK);
	}

	int lines[] = {52, 53, 106, 107};
	// lines at 52, 53, 106, 107
	for (int i : lines) {
		horizontal_line(i, CLR_BLACK);
		vertical_line(blank_fill + i, CLR_BLACK);
	}
}

void game_end(COLOR color) {
	//int blank_fill = (SCREEN_WIDTH-SCREEN_HEIGHT)/2 + 1; // 40
	for (int i=0; i<BLANK_FILL; i++) {
		vertical_line(i, color);
		vertical_line(SCREEN_WIDTH - i, color);
	}
}*/

/*void init_map1()
{
    // initialize a background
    REG_BG1CNT= BG_PRIO(1) | BG_CBB(CBB_1) | BG_SBB(SBB_1) | BG_REG_32x32;//BG_REG_64x64;
    REG_BG1HOFS = 0;
    REG_BG1VOFS = 0;

    // (1) create the tiles: basic tile and a cross
    const TILE tiles[]=
    {
        {{0x11111111, 0x01111111, 0x01111111, 0x01111111,
          0x01111111, 0x01111111, 0x01111111, 0x00000001}},
        {{0x00000000, 0x00100100, 0x01100110, 0x00011000,
          0x00011000, 0x01100110, 0x00100100, 0x00000000}},
        {{0x11111111, 0x11111111, 0x11111111, 0x11111111,
          0x11111111, 0x11111111, 0x11111111, 0x11111111}},
        {{0x00000000, 0x00000000, 0x00000000, 0x00000000,
          0x00000000, 0x00000000, 0x00000000, 0x00000000}},
        {{0x00011000, 0x00011000, 0x00011000, 0x00011000,
          0x00011000, 0x00011000, 0x00011000, 0x00011000}},
        {{0x01100000, 0x01100000, 0x01100000, 0x01100000,
          0x01100000, 0x01100000, 0x01100000, 0x01100000}},
        {{0x00000000, 0x00000000, 0x00000000, 0x11111111,
          0x11111111, 0x00000000, 0x00000000, 0x00000000}},
        {{0x00000000, 0x11111111, 0x11111111, 0x00000000,
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
      tile_mem[CBB_1][i] = tiles[i];
    }

    // (2) create a palette
    pal_bg_bank[0][1]= RGB15(31,  0,  0);
    pal_bg_bank[1][1]= RGB15( 0, 31,  0);
    pal_bg_bank[2][1]= RGB15( 0,  0, 31);
    pal_bg_bank[3][1]= RGB15(31, 31, 31);
    pal_bg_bank[4][1]= RGB15( 0,  0,  0);

    // (3) Create a map: four contingent blocks of
    //   0x0000, 0x1000, 0x2000, 0x3000.
    SCR_ENTRY *pse= bg1_map;

    for(int i=0; i<32; i++) {
      for (int j=0; j<32; j++) {
        SCR_ENTRY tile;
        if (j == 11) {
          tile = SE_PALBANK(4) | 4;
        } else if (j == 18) {
          tile = SE_PALBANK(4) | 5;
        } else if (i == 6){
          tile = SE_PALBANK(4) | 6;
        } else if (i == 13) {
          tile = SE_PALBANK(4) | 7;
        } else {
          tile = SE_PALBANK(1) | 3;
        }

        *pse++= tile;
      }
    }
}*/
/*
OBJ_ATTR *metr = metr2;
while(1)
{
    vid_vsync();
    key_poll();

    // (5) Do various interesting things
    // move left/right
    x += 2*key_tri_horz();
    // move up/down
    y += 2*key_tri_vert();

    // increment/decrement starting tile with R/L
    tid2 += bit_tribool(key_hit(-1), KI_R, KI_L);

    // flip
    if(key_hit(KEY_A))  // horizontally
        metr->attr1 ^= ATTR1_HFLIP;
    if(key_hit(KEY_B))  // vertically
        metr->attr1 ^= ATTR1_VFLIP;

    // make it glow (via palette swapping)
    //pb2= key_is_down(KEY_SELECT) ? 1 : 0;

    // toggle mapping mode
    if(key_hit(KEY_START))
        REG_DISPCNT ^= DCNT_OBJ_1D;

    // Hey look, it's one of them build macros!
    metr->attr2= ATTR2_BUILD(tid2, pb2, 0);
    obj_set_pos(metr, x, y);

    //oam_copy(oam_mem, obj_buffer, 1);   // (6) Update OAM (only one now)
    oam_copy(oam_mem, obj_buffer, 2);   // (6) Update OAM (only one now)
}*/
//&tile_mem[4][64]

/*int x= 96, y= 32;
//u32 tid1= 0, pb1= 0;      // (3) tile id, pal-bank
u32 tid2 = 80, pb2= 3;      // (3) tile id, pal-bank
OBJ_ATTR *metr1= &obj_buffer[0];

obj_set_attr(metr1,
    ATTR0_SQUARE,              // Square, regular sprite
    ATTR1_SIZE_64,              // 64x64p,
    ATTR2_PALBANK(pb1) | tid1);   // palbank 0, tile 0

// (4) position sprite (redundant here; the _real_ position
// is set further down
//obj_set_pos(metr, x, y);
obj_set_pos(metr1, 0, 0);

obj_hide(metr1);


OBJ_ATTR *metr2= &obj_buffer[1];

obj_set_attr(metr2,
    ATTR0_SQUARE,              // Square, regular sprite
    ATTR1_SIZE_32,              // 64x64p,
    ATTR2_PALBANK(pb2) | tid2);   // palbank 0, tile 0

// (4) position sprite (redundant here; the _real_ position
// is set further down
obj_set_pos(metr2, 0, 0);

OBJ_ATTR *metr = metr2;

while(1)
{
    vid_vsync();
    key_poll();

    // (5) Do various interesting things
    // move left/right
    x += 2*key_tri_horz();
    // move up/down
    y += 2*key_tri_vert();

    // increment/decrement starting tile with R/L
    tid2 += bit_tribool(key_hit(-1), KI_R, KI_L);

    // flip
    if(key_hit(KEY_A))  // horizontally
        metr->attr1 ^= ATTR1_HFLIP;
    if(key_hit(KEY_B))  // vertically
        metr->attr1 ^= ATTR1_VFLIP;

    // make it glow (via palette swapping)
    //pb2= key_is_down(KEY_SELECT) ? 1 : 0;

    // toggle mapping mode
    if(key_hit(KEY_START))
        REG_DISPCNT ^= DCNT_OBJ_1D;

    // Hey look, it's one of them build macros!
    metr->attr2= ATTR2_BUILD(tid2, pb2, 0);
    obj_set_pos(metr, x, y);

    //oam_copy(oam_mem, obj_buffer, 1);   // (6) Update OAM (only one now)
    oam_copy(oam_mem, obj_buffer, 2);   // (6) Update OAM (only one now)
}*/
/*obj_set_attr(metr1,
    ATTR0_SQUARE,              // Square, regular sprite
    ATTR1_SIZE_64,              // 64x64p,
    ATTR2_PALBANK(pb1) | tid1);   // palbank 0, tile 0

// (4) position sprite (redundant here; the _real_ position
// is set further down
//obj_set_pos(metr, x, y);
obj_set_pos(metr1, 0, 0);

obj_hide(metr1);*/
/*if (j < 5 || j >= 25) {
  tile = SE_PALBANK(3) | 1;
} /else if (j == 11) {
  tile = SE_PALBANK(3) | 1;
} else if (j == 18) {
  tile = SE_PALBANK(3) | 1;
} else if (i == 6){
  tile = SE_PALBANK(3) | 1;
} else if (i == 13) {
  tile = SE_PALBANK(3) | 1;
}/ else {
  tile = SE_PALBANK(3) | 0;
}*/
/*memcpy(&tile_mem[4][0], metr_boxTiles, metr_boxTilesLen);
memcpy(pal_obj_mem, metrPal, metrPalLen);

memcpy(&tile_mem[4][64], my_boxTiles, sizeof(my_boxTiles));

memset(&tile_mem[4][80], 0, metr_boxTilesLen);*/
//memset(&tile_mem[4][80], 0, metr_boxTilesLen);
/*if (i % 8 == 1) {
  for (int j=0; j<8; j++) {
    t.data[j] |= 0x00000001;
  }
}
if (i % 8 == 6) {
  for (int j=0; j<8; j++) {
    t.data[j] |= 0x10000000;
  }
}
if (i / 8 == 1) {
  t.data[0] |= 0x11111111;
}
if (i / 8 == 6) {
  t.data[7] |= 0x11111111;
}*/
/*if (i / 8 == (7 - (i % 8))) {
  for (int j=0; j<8; j++) {
    t.data[7-j] |= (1 << j*4);
  }
}*/
/*for (int i=0; i < 16; i++) {
  if ( i / 4 == 0 || i / 4 == 7 || i % 4 == 0 || i % 4 == 7) {
    continue;
  }
  TILE t = tile_mem[4][base + i];

  if (i / 4 == i % 4) {
    for (int j=0; j<8; j++) {
      if (i != 5 || j > 2) {
        t.data[j] |= (1 << j*4);
      }
    }
  }

  tile_mem[4][base + i] = t;
}*/
/*int radius = 20;
int x = radius;
int y = 0;
int err = 0;

while (x >= y)
{
    putpixel(x0 + x, y0 + y);
    putpixel(x0 + y, y0 + x);
    /putpixel(x0 - y, y0 + x);
    putpixel(x0 - x, y0 + y);
    putpixel(x0 - x, y0 - y);
    putpixel(x0 - y, y0 - x);
    putpixel(x0 + y, y0 - x);
    putpixel(x0 + x, y0 - y);/

    if (err <= 0)
    {
        y += 1;
        err += 2*y + 1;
    }
    if (err > 0)
    {
        x -= 1;
        err -= 2*x + 1;
    }
}*/

/*int base = 16;
int tileIndex = (x/8) + (y/8)*4;
TILE t = tile_mem[4][base + tileIndex];
int row = y % 8;
int column = x % 8;
t.data[row] |= 1 << column*4;
tile_mem[4][base + tileIndex] = t;*/

  //OBJ_ATTR *metr= &obj_buffer[4];

  //u16 flippy = (i%2 ? ATTR1_HFLIP : 0) | (i/2 ? ATTR1_VFLIP : 0);

  /*obj_set_attr(metr,
      ATTR0_SQUARE,              // Square, regular sprite
      ATTR1_SIZE_32 /| flippy/,              // 32x32p,
      //ATTR2_PALBANK(paletteBank) | tid);   // palbank paletteBank, tile 0

  //obj_set_pos(metr, x /+ (i%2)*32/, y /+ (i/2)*32)/);/
  /void draw_x(int i, int j, int paletteBank) {
    int x = 40 + 56*i;
    int y =  0 + 56*j;
    u32 tid = 0;

    for (int i=0; i<4; i++) {
      OBJ_ATTR *metr= &obj_buffer[i];

      u16 flippy = (i%2 ? ATTR1_HFLIP : 0) | (i/2 ? ATTR1_VFLIP : 0);

      obj_set_attr(metr,
          ATTR0_SQUARE,              // Square, regular sprite
          ATTR1_SIZE_32 | flippy,              // 32x32p,
          ATTR2_PALBANK(paletteBank) | tid);   // palbank paletteBank, tile 0

      obj_set_pos(metr, x + (i%2)*16, y + (i/2)*16);
      //obj_set_pos(metr, x + (i%2)*32, y + (i/2)*32);
    }

    oam_copy(oam_mem, obj_buffer, 4);   // (6) Update OAM (only one now)
  }

  void draw_o(int i, int j, int paletteBank) {
    int x = 40 + 56*i;
    int y =  0 + 56*j;
    u32 tid = 16;


    for (int i=0; i<4; i++) {
      OBJ_ATTR *metr= &obj_buffer[4+i];

      u16 flippy = (i%2 ? ATTR1_HFLIP : 0) | (i/2 ? ATTR1_VFLIP : 0);

      obj_set_attr(metr,
          ATTR0_SQUARE,              // Square, regular sprite
          ATTR1_SIZE_32 | flippy,              // 32x32p,
          ATTR2_PALBANK(paletteBank) | tid);   // palbank paletteBank, tile 0

      obj_set_pos(metr, x + (i%2)*16, y + (i/2)*16);
    }

    oam_copy(oam_mem, obj_buffer, 8);   // (6) Update OAM (only one now)
  }*/
/*

void obj_test() {
    // x: 32, 88, 144
    // y: -8, 48, 104
    int x= 88, y= -8;
    //u32 tid1= 0, pb1= 0;      // (3) tile id, pal-bank
    u32 tid2= 0, pb2= 3;      // (3) tile id, pal-bank
    //OBJ_ATTR *metr1= &obj_buffer[0];


    for (int i=0; i<4; i++) {
      OBJ_ATTR *metr= &obj_buffer[i];

      u16 flippy = (i%2 ? ATTR1_HFLIP : 0) | (i/2 ? ATTR1_VFLIP : 0);

      obj_set_attr(metr,
          ATTR0_SQUARE,              // Square, regular sprite
          ATTR1_SIZE_32 | flippy,              // 64x64p,
          ATTR2_PALBANK(pb2) | tid2);   // palbank 0, tile 0

      // (4) position sprite (redundant here; the _real_ position
      // is set further down
      obj_set_pos(metr, x + (i%2)*32, y + (i/2)*32);
    }

    oam_copy(oam_mem, obj_buffer, 4);   // (6) Update OAM (only one now)
}
*/


/*if (i % 8 == 1) {
  for (int j=0; j<8; j++) {
    t.data[j] |= 0x00000001;
  }
}
if (i % 8 == 6) {
  for (int j=0; j<8; j++) {
    t.data[j] |= 0x10000000;
  }
}
if (i / 8 == 1) {
  t.data[0] |= 0x11111111;
}
if (i / 8 == 6) {
  t.data[7] |= 0x11111111;
}*/
/*if (i / 8 == (7 - (i % 8))) {
  for (int j=0; j<8; j++) {
    t.data[7-j] |= (1 << j*4);
  }
}*/
/*void md5_loop() {
      static const char STR[] = "STAY FOOLISH"; // static to ensure it wont end up in RAM
      const size_t STR_LEN = sizeof(STR) - 1;
      md5((const uint8_t*) STR, STR_LEN);

      char data [100];
      memset(data, 0, sizeof(data));

      uint8_t result [16];
      memset(result, 0, sizeof(result));

      for (int i=0; i<1000; i++) {
        memcpy(data, result, 16);
        const size_t REG_BASE = 0x04000000;
        memcpy(data + 16, (void*)(REG_BASE+0x0100), 16);
        memcpy(data + 32, (void*)(REG_BASE+0x0130), 2);
        md5_2((const uint8_t*) data, sizeof(data), result);
      }
}*/
//puts("hello2");

/*vid_vsync();
reset_time();
vid_vsync();
print_time();*/

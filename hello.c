/*
A simple "hello world" example.
Set the screen background color and palette colors.
Then write a message to the nametable.
Finally, turn on the PPU to display video.
*/
#define NES_MIRRORING 1
#include "neslib.h"
#include "vrambuf.h"
#include "bcd.h"
#include "apu.h"
// define a 2x2 metasprite
#define DEF_METASPRITE_2X2(name,code,pal)\
const unsigned char name[]={\
        0,      0,      (code)+0,   pal, \
        0,      8,      (code)+16,   pal, \
        8,      0,      (code)+1,   pal, \
        8,      8,      (code)+17,   pal, \
        128};

// define a 2x2 metasprite, flipped horizontally
#define DEF_METASPRITE_2X2_FLIP(name,code,pal)\
const unsigned char name[]={\
        8,      0,      (code)+0,   (pal)|OAM_FLIP_H, \
        8,      8,      (code)+16,   (pal)|OAM_FLIP_H, \
        0,      0,      (code)+1,   (pal)|OAM_FLIP_H, \
        0,      8,      (code)+17,   (pal)|OAM_FLIP_H, \
        128};

#define DEF_METASPRITE_2X1(name,code,pal)\
const unsigned char name[]={\
	0,	0,	(code)+0,   pal,\
        0,	8,	(code)+16,  pal,\
        128};

#define DEF_METASPRITE_2X1_FLIP(name,code,pal)\
const unsigned char name[]={\
	0,	0,	(code)+0,   (pal)|OAM_FLIP_H,\
        0,	8,	(code)+16,  (pal)|OAM_FLIP_H,\
        128};

#define PLAYER_Y_MIN 48
#define PLAYER_Y_MAX 208
#define PLAYER_X_MIN 8
#define PLAYER_X_MAX 240
#define ZERO_TILE 0xDA
#define INVINCIBILITY_FRAMES 50
#define MAX_ACTORS 5

const unsigned char nametable[668]={
0x03,0x6e,0x6e,0x6f,0x63,0x64,0x65,0x03,0x02,0x60,0x61,0x62,0x1c,0x1c,0x8b,0x62,
0x63,0x64,0x9b,0x65,0x65,0x62,0x63,0x9b,0x65,0x62,0x63,0x64,0x6e,0x03,0x07,0x07,
0x03,0x04,0x08,0x1c,0x03,0x02,0x62,0x8b,0x62,0x63,0x64,0x9b,0x75,0x75,0x65,0x1c,
0x9b,0x75,0x72,0x73,0x74,0x6e,0x03,0x0c,0x07,0x07,0x08,0x60,0x60,0x8b,0x63,0x64,
0x65,0x9b,0x85,0x85,0x75,0x1c,0x9b,0x85,0x82,0x00,0x01,0x03,0x05,0x6e,0x03,0x09,
0x6f,0x70,0x70,0x8c,0x73,0x74,0x75,0x9b,0x95,0x95,0x85,0x1c,0x9b,0x65,0x92,0x20,
0x21,0x11,0x03,0x04,0x01,0x03,0x0a,0x80,0x80,0x81,0x83,0x84,0x85,0x9b,0xa5,0xa5,
0x95,0x1c,0x9b,0x75,0xa2,0xa3,0x9e,0x20,0x21,0x11,0x03,0x02,0x10,0x11,0x6e,0x03,
0x07,0x6f,0x90,0x60,0x61,0x63,0x64,0x65,0x59,0xb5,0xb5,0xa5,0xa5,0x5a,0x85,0xb2,
0xb3,0xb4,0xb5,0x9b,0x10,0x11,0x11,0x10,0x11,0x6e,0x03,0x08,0x07,0x18,0x71,0x73,
0x74,0x75,0x5b,0x1e,0x1f,0x1f,0x0f,0x5c,0x95,0xa3,0xa4,0xa5,0x71,0x9b,0x10,0x11,
0x11,0x10,0x11,0x6e,0x03,0x09,0x07,0x07,0x18,0x64,0x65,0x9b,0xb1,0xb2,0xb3,0xb4,
0x9b,0xa5,0xb3,0xb4,0xb5,0x81,0x9b,0x20,0x11,0x11,0x10,0x11,0x6e,0x6e,0xaf,0xbf,
0x6e,0x03,0x07,0x6f,0x74,0x75,0x4d,0xb0,0xb1,0xb2,0x60,0x9b,0x62,0x63,0x64,0x65,
0x91,0x9b,0xa0,0x10,0x11,0x10,0x11,0x6e,0x6e,0xbf,0xaf,0x6e,0x30,0x31,0x03,0x05,
0x32,0x84,0x85,0x80,0xb0,0xb1,0xb2,0x60,0x5e,0x07,0x03,0x04,0x5f,0x63,0x20,0x11,
0x10,0x11,0xaf,0xbf,0xaf,0xbf,0xaf,0xbf,0x11,0x03,0x02,0x12,0x8c,0x9b,0x1c,0x94,
0x95,0x90,0xb0,0xb1,0x60,0x61,0x62,0x3d,0x4f,0x03,0x02,0x3e,0x63,0x64,0x1c,0x10,
0x10,0x11,0xbf,0xaf,0xbf,0xaf,0xbf,0xaf,0x11,0x03,0x02,0x22,0xa0,0x9b,0xa3,0xa4,
0xa5,0xa0,0xb0,0xb1,0x60,0x61,0x62,0x63,0x64,0x65,0x85,0x64,0x65,0x74,0x75,0x20,
0x10,0x11,0x01,0x02,0xaf,0xbf,0x6e,0x6e,0x11,0x11,0x12,0x90,0xb0,0x4f,0xb3,0xb4,
0xb5,0xb0,0x60,0x61,0x62,0x63,0x64,0x65,0x03,0x02,0x95,0x74,0x75,0x1c,0x1c,0x95,
0x10,0x11,0x6e,0x6e,0xbf,0xaf,0x6e,0x6e,0x11,0x11,0x22,0xa0,0x60,0x61,0x63,0x64,
0x65,0xb0,0x70,0x71,0x72,0x73,0x74,0x75,0x03,0x02,0xa5,0x84,0x1c,0x03,0x02,0xa5,
0x10,0x11,0x6e,0x6e,0xaf,0xbf,0x6e,0x6e,0x11,0x3b,0xb1,0xb0,0x70,0x71,0x73,0x74,
0x75,0x60,0x61,0x62,0x63,0x64,0x65,0x85,0x03,0x02,0xb5,0x94,0x38,0x03,0x02,0x1c,
0x10,0x11,0x6e,0x6e,0xbf,0xaf,0x00,0x01,0x11,0x4e,0x80,0xb0,0x80,0x81,0x8e,0x84,
0x85,0x60,0x61,0x62,0x63,0x8e,0x65,0x95,0x03,0x02,0x1c,0x38,0x38,0x28,0x38,0x38,
0x10,0x11,0x6e,0x03,0x05,0x11,0x63,0x90,0xa0,0x60,0x61,0x1f,0x0e,0x1f,0x03,0x03,
0x0e,0x1f,0x62,0x63,0x64,0x65,0x28,0x38,0x28,0x38,0x38,0x28,0x10,0x11,0x01,0x02,
0x6e,0x03,0x03,0x22,0x60,0xa0,0xb0,0x70,0x71,0x73,0xbd,0x60,0x70,0x9f,0x70,0xbd,
0x71,0x72,0x73,0x74,0x75,0x28,0x38,0x38,0x42,0x38,0x38,0x10,0x11,0x6e,0x03,0x05,
0x6f,0x70,0xb0,0xb1,0x80,0x60,0x62,0xbd,0x70,0x80,0x9f,0x80,0xbd,0x60,0x61,0x62,
0x63,0x64,0x8c,0x38,0x9e,0x42,0x8b,0x8c,0x10,0x11,0x6e,0x03,0x05,0x6f,0x80,0xa0,
0xa1,0x90,0x70,0x72,0xbd,0x1c,0x90,0x8f,0x90,0xbd,0x70,0x60,0x61,0x62,0x63,0x64,
0x65,0x91,0x42,0x8c,0xb1,0x10,0x11,0x01,0x03,0x06,0x90,0x9d,0xb1,0xa0,0xad,0x62,
0x0a,0x9d,0x1c,0x9f,0xa0,0xbd,0x80,0x70,0x71,0x72,0x73,0x74,0x8d,0x1c,0x42,0x1c,
0xb1,0x0c,0x03,0x12,0x0d,0x09,0x1c,0x03,0x05,0x0b,0x0c,0x0c,0x0d,0xae,0x1c,0x61,
0x62,0x70,0x90,0x03,0x03,0xb0,0xb1,0xb2,0xb3,0x80,0x60,0x60,0x61,0x70,0xb0,0xb1,
0xb2,0x0c,0x0d,0x90,0xad,0x71,0x9d,0x73,0x1b,0x75,0x60,0xb0,0x0d,0x1c,0x61,0x62,
0x60,0xa0,0x03,0x03,0xa1,0xa2,0xa3,0xa4,0x90,0x70,0x60,0x61,0x80,0xb0,0xb1,0xb2,
0xb3,0xb0,0x0c,0x03,0x04,0x63,0x64,0x70,0xb0,0xb1,0x03,0x00
};


const unsigned char nametable2[617]={
0x05,0x70,0x71,0x72,0x73,0x74,0x00,0x21,0x05,0x04,0x11,0x6e,0x05,0x0b,0x7f,0x85,
0x85,0x20,0x21,0x11,0x05,0x02,0x80,0x81,0x82,0x00,0x21,0x21,0x6e,0x05,0x0d,0x6f,
0x80,0x81,0x80,0x81,0x80,0x81,0x82,0x6d,0x11,0x05,0x02,0x90,0x00,0x21,0x21,0x6e,
0x05,0x0f,0x6f,0x90,0x91,0x90,0x91,0x90,0x91,0x92,0x6d,0x11,0x05,0x02,0x21,0x21,
0x6e,0x05,0x03,0xaf,0x05,0x07,0x6e,0x01,0x05,0x07,0x02,0xa1,0xa0,0xa1,0xa2,0x6d,
0x11,0x05,0x02,0x6e,0x05,0x05,0x9b,0xbf,0x6e,0x05,0x03,0xbf,0x9b,0x6e,0x05,0x03,
0x11,0x05,0x03,0x22,0x9b,0xb1,0xb0,0xb1,0xb2,0x6d,0x6e,0x05,0x08,0x9b,0xbf,0x07,
0x05,0x03,0xbf,0x9b,0x6e,0x05,0x04,0x11,0x11,0x22,0xb6,0x9b,0xb4,0xb5,0xb6,0xb7,
0x6d,0xbf,0xaf,0x6e,0x05,0x06,0x9b,0xbf,0x6e,0x05,0x03,0xbf,0x9b,0x6e,0x05,0x05,
0x12,0x9b,0xa6,0x9b,0xb4,0xb5,0xb6,0xb7,0x6d,0xaf,0xbf,0x6e,0x05,0x06,0x9b,0xbf,
0x6e,0x05,0x03,0xbf,0x9b,0x6e,0x05,0x05,0x12,0x9b,0x74,0x9b,0x76,0x05,0x03,0x6d,
0x6e,0x05,0x07,0x06,0x07,0x05,0x07,0x08,0x6e,0x05,0x04,0x12,0x9b,0x76,0x9b,0x86,
0x05,0x03,0x03,0x04,0x05,0x02,0x6e,0x05,0x13,0x12,0x9b,0x86,0x9b,0x96,0x05,0x04,
0x23,0x14,0x14,0x6e,0x05,0x13,0x12,0x9b,0x96,0x9b,0xa6,0x05,0x05,0x13,0x14,0x01,
0x05,0x04,0x02,0x1e,0x1f,0xa5,0x85,0xa6,0xa6,0x1f,0x0f,0x01,0x05,0x05,0x12,0x9b,
0xa6,0x9b,0x13,0x14,0x05,0x06,0x11,0x05,0x03,0x12,0x75,0x76,0x77,0x76,0x0e,0x0e,
0x80,0xa0,0xa1,0x20,0x11,0x05,0x04,0x22,0x9b,0xa6,0x9b,0x23,0x24,0x25,0x9e,0x3c,
0x13,0x14,0x14,0x11,0x05,0x03,0x22,0x85,0x86,0x71,0x71,0xbd,0xbd,0x74,0x75,0x76,
0xb2,0x10,0x59,0x11,0x59,0x12,0x6b,0x9b,0xb6,0x9b,0x70,0x3b,0x73,0xa1,0x4c,0x23,
0x24,0x14,0x11,0x39,0x3a,0x12,0x74,0x75,0x76,0x81,0x71,0xbd,0xbd,0x71,0x05,0x03,
0x10,0x5b,0x11,0x5b,0x12,0x71,0x9b,0x72,0x9b,0x74,0x4b,0x91,0xa1,0xa2,0xa3,0xa4,
0x13,0x11,0x49,0x4a,0x12,0x84,0x85,0x86,0x91,0x81,0xbd,0xbd,0x81,0x05,0x03,0x20,
0x11,0x05,0x02,0x22,0x81,0x9b,0x72,0x9b,0x74,0x75,0xa1,0xa1,0xa2,0xa3,0xa4,0x23,
0x11,0x05,0x02,0x12,0x74,0x75,0x76,0xa1,0x91,0xbd,0xbd,0x91,0x05,0x04,0x10,0x11,
0x12,0x91,0x71,0x9b,0x73,0x9b,0x75,0x75,0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0x11,0x11,
0x21,0x22,0x84,0x85,0x86,0x87,0xa1,0xbd,0xbd,0xa1,0x05,0x02,0x71,0x72,0x10,0x11,
0x12,0x76,0x76,0x9b,0x76,0x9b,0x76,0x81,0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0x11,0x12,
0x8c,0x73,0x74,0x75,0x76,0x77,0xb7,0xbd,0xbd,0x71,0x71,0x38,0x71,0x71,0x10,0x59,
0x12,0x71,0x71,0x5e,0x07,0x5f,0x71,0x91,0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0x11,0x12,
0x82,0x83,0x84,0x85,0x86,0x87,0xb7,0xbd,0xbd,0x81,0x81,0x38,0x81,0x81,0x10,0x5b,
0x12,0x81,0x05,0x02,0x4d,0x81,0x81,0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0x03,0x11,0x12,
0x72,0x73,0x74,0x75,0x76,0x77,0xb7,0xbd,0xbd,0x91,0x91,0x42,0x91,0x91,0x10,0x11,
0x12,0x91,0x05,0x05,0xa1,0xa2,0xa3,0xa4,0xa5,0x03,0x14,0x11,0x12,0x4f,0x9c,0x9c,
0x3c,0x9c,0x05,0x02,0x4d,0x4d,0x9c,0x9c,0x3c,0x9c,0x9c,0x10,0x11,0x12,0x9c,0x05,
0x06,0x4d,0x9c,0x9c,0x13,0x14,0x14,0x39,0x3a,0x39,0x3a,0x39,0x3a,0x39,0x3a,0x39,
0x3a,0x39,0x3a,0x39,0x3a,0x39,0x3a,0x39,0x3a,0x39,0x3a,0x39,0x3a,0x39,0x3a,0x39,
0x3a,0x39,0x3a,0x39,0x3a,0x39,0x3a,0x49,0x4a,0x49,0x4a,0x49,0x4a,0x49,0x4a,0x49,
0x4a,0x49,0x4a,0x49,0x4a,0x49,0x4a,0x49,0x4a,0x49,0x4a,0x49,0x4a,0x49,0x4a,0x49,
0x4a,0x49,0x4a,0x49,0x4a,0x49,0x4a,0x05,0x00
};

DEF_METASPRITE_2X2(emptyMeta, 0xEE, 0);
DEF_METASPRITE_2X2(snakeIdle, 0x40, 0);

typedef enum ActorState
{
  STANDING, RUNNING, JUMPING, FALLING, ATTACKING
};

typedef enum ActorType
{
  PLAYER, SNAKE, OTHER
};

struct actor_attr
  {
  enum ActorType type;
  bool is_alive;
  short pos_x;
  short pos_y;
  short vel_x;
  short vel_y;
  short dir;
  short life;
  short invin;
  byte state;
  const unsigned char* meta;
  };

struct actor_attr empty_actor = {OTHER, false, 120, 100, 0, 0, 0, 3, 0, STANDING, emptyMeta};
struct actor_attr snake = {SNAKE, true, 100, 100, 0, 0, 1, 1, 0, STANDING, snakeIdle};

const struct actor_attr stage_one_enemies[4] = {{SNAKE, true, 200, 103, 0, 0, 1, 1, 0, STANDING, snakeIdle}, {SNAKE, true, 20, 64, 0, 0, 1, 1, 0, STANDING, snakeIdle}};
const struct actor_attr stage_two_enemies[4] = {{SNAKE, true, 140, 120, 0, 0, 1, 1, 0, STANDING, snakeIdle}, {SNAKE, true, 140, 56, 0, 0, 1, 1, 0, STANDING, snakeIdle}};

// Add a list of enemies using actor attr.
struct level_data
{
  const unsigned char* map;
  const struct actor_attr* enemies;
  int player_spawn_x;
  int player_spawn_y;
  int portal_x;
  int portal_y;
  short num_enemies;
};

#define LEVEL_COUNT 2
struct level_data level_list[LEVEL_COUNT] = {{nametable, stage_one_enemies, 120, 100, 200, 50, 2}, 
                                             {nametable2, stage_two_enemies, 32, 72, 235, 180, 2}};



// link the pattern table into CHR ROM
//#link "potential.s"
//#link "vrambuf.c"
//#link "apu.c"

DEF_METASPRITE_2X1(playerRunR0, 0x00, 0);
DEF_METASPRITE_2X1(playerRunR1, 0x01, 0);
DEF_METASPRITE_2X1(playerRunR2, 0x02, 0);
DEF_METASPRITE_2X1(playerRunR3, 0x03, 0);
DEF_METASPRITE_2X1(playerRunR4, 0x04, 0);
DEF_METASPRITE_2X1(playerRunR5, 0x05, 0);

DEF_METASPRITE_2X1_FLIP(playerRunL0, 0x00, 0);
DEF_METASPRITE_2X1_FLIP(playerRunL1, 0x01, 0);
DEF_METASPRITE_2X1_FLIP(playerRunL2, 0x02, 0);
DEF_METASPRITE_2X1_FLIP(playerRunL3, 0x03, 0);
DEF_METASPRITE_2X1_FLIP(playerRunL4, 0x04, 0);
DEF_METASPRITE_2X1_FLIP(playerRunL5, 0x05, 0);

DEF_METASPRITE_2X1(playerJumpR0, 0x06, 0);
DEF_METASPRITE_2X1(playerJumpR1, 0x07, 0);
DEF_METASPRITE_2X1(playerJumpR2, 0x08, 0);
DEF_METASPRITE_2X1(playerJumpR3, 0x09, 0);

DEF_METASPRITE_2X1_FLIP(playerJumpL0, 0x06, 0);
DEF_METASPRITE_2X1_FLIP(playerJumpL1, 0x07, 0);
DEF_METASPRITE_2X1_FLIP(playerJumpL2, 0x08, 0);
DEF_METASPRITE_2X1_FLIP(playerJumpL3, 0x09, 0);

DEF_METASPRITE_2X1(playerSwingR0, 0x20, 0);
DEF_METASPRITE_2X2(playerSwingR1, 0x22, 0);
DEF_METASPRITE_2X2(playerSwingR2, 0x24, 0);
DEF_METASPRITE_2X2(playerSwingR3, 0x26, 0);
DEF_METASPRITE_2X2(playerSwingR4, 0x28, 0);

DEF_METASPRITE_2X1_FLIP(playerSwingL0, 0x20, 0);
DEF_METASPRITE_2X2_FLIP(playerSwingL1, 0x22, 0);
DEF_METASPRITE_2X2_FLIP(playerSwingL2, 0x24, 0);
DEF_METASPRITE_2X2_FLIP(playerSwingL3, 0x26, 0);
DEF_METASPRITE_2X2_FLIP(playerSwingL4, 0x28, 0);

int anim_number = 0;
int anim_loop = 0;
int hundreds = 5;
int tens = 5;
int ones = 5;
int life_pos = 9;
bool called = false;
int cur_level = 0;
short has_attacked = 0;
struct actor_attr actors[MAX_ACTORS];

// Set first 3 rows to be collideable tiles, this will remove for loop every frame.
const unsigned char collideables[31] = {0x6c, 0x6D, 0x6E, 0x04, 0x01, 0x44, 0x45, 0x46, 0x47, 0x11,
                                  0x14, 0x06, 0x07, 0x08, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35,
                                  0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x4E, 0x4F, 0x2C, 0x39, 0x1E};

const unsigned char* const playerRunning[24] = 
{
  playerRunR0, playerRunR1, playerRunR2, playerRunR3,
  playerRunR4, playerRunR5, playerRunR0, playerRunR1, playerRunR2, playerRunR3,
  playerRunR4, playerRunR5,
  playerRunL0, playerRunL1, playerRunL2, playerRunL3,
  playerRunL4, playerRunL5, playerRunL0, playerRunL1, playerRunL2, playerRunL3,
  playerRunL4, playerRunL5
};

const unsigned char* const playerJumping[8] = 
{
  playerJumpR0, playerJumpR1, playerJumpR2, playerJumpR3,
  playerJumpL0, playerJumpL1, playerJumpL2, playerJumpL3
};

const unsigned char* const playerSwinging[10] =
{
  playerSwingR0, playerSwingR1, playerSwingR2,
  playerSwingR3, playerSwingR4, playerSwingL0, playerSwingL1, 
  playerSwingL2, playerSwingL3, playerSwingL4
};

void add_velocity(struct actor_attr* player)
{
  if(player->pos_x < PLAYER_X_MAX && player->pos_x > PLAYER_X_MIN)
        player->pos_x += player->vel_x;
  
  if(player->pos_y < PLAYER_Y_MAX && player->pos_y > PLAYER_Y_MIN)
        player->pos_y += player->vel_y;
}

void decrement_life(struct actor_attr* player)
{
  if(player->life > 0 && player->invin <= 0)
  {
    player->invin = INVINCIBILITY_FRAMES;
    APU_ENABLE(ENABLE_PULSE0|ENABLE_PULSE1);
    APU_PULSE_DECAY(PULSE_CH0, 172, 128, 2, 4);
    APU_PULSE_SWEEP(PULSE_CH0, 3, 1, 0);
    APU_PULSE_DECAY(PULSE_CH1, 1907, 192, 7, 11);
    APU_PULSE_SWEEP(PULSE_CH1, 3, 4, 0);
    //APU_NOISE_DECAY(12|128, 9, 8);
    vram_adr(NTADR_A(life_pos, 2));
    vram_put(0xA9);
    life_pos -= 2;
    player->life--;
    vram_adr(0);
  }
}

void kill_enemy(int index)
{
  APU_ENABLE(ENABLE_PULSE1|ENABLE_NOISE);
  APU_PULSE_DECAY(PULSE_CH1, 240, 64, 2, 15);
  APU_PULSE_SWEEP(PULSE_CH1, 4, 4, 0);
  APU_NOISE_DECAY(12|128, 9, 8);
  actors[index] = empty_actor;
}

void handle_collision(struct actor_attr* player)
{
  short i = 0;
  unsigned char bot_tile;
  unsigned char top_tile;
  unsigned char left_tile;
  unsigned char right_tile;
  int tile_x = (player->pos_x / 8);
  int tile_y = (player->pos_y / 8);
  tile_x += 1;
  tile_y += 2;
  
  vram_adr(NTADR_A(tile_x, tile_y));
  //vram_put(0x20);
  vram_read(&bot_tile, 1);
  vram_adr(NTADR_A(tile_x, tile_y - 2));
  vram_read(&top_tile, 1);
  vram_adr(NTADR_A(tile_x, tile_y - 1));
  vram_read(&right_tile, 1);
  vram_adr(NTADR_A(tile_x - 1, tile_y - 1));
  vram_read(&left_tile, 1);
  
  vram_adr(NTADR_A(0, 0));
  
  if(bot_tile <= 0x5f && bot_tile != 0x1C && player->state != JUMPING)
  {
    player->pos_y -= 1;
    player->state = STANDING;
  }
  else if(player->state != JUMPING)
    player->state = FALLING;
  if(top_tile <= 0x5f && top_tile != 0x1C && top_tile != 0x07)
  {
    player->state = FALLING;
  }
  if(left_tile <= 0x5f && left_tile != 0x1C)
  {
    player->vel_x = 0;
    player->pos_x += 1;
  }
  if(right_tile <= 0x5f && right_tile != 0x1C)
  {
    player->vel_x = 0;
    player->pos_x -= 1;
  }
  
  if(has_attacked > 0)
    has_attacked--;
  
  while(i < level_list[cur_level].num_enemies)
  {
    if(player->pos_x + 8 > actors[i].pos_x && player->pos_x < actors[i].pos_x + 16 &&
       player->pos_y + 16 > actors[i].pos_y && player->pos_y < actors[i].pos_y + 16)
    {
      if(actors[i].is_alive)
      {
      	if(has_attacked <= 0)
      		decrement_life(player);
      	else
      	  kill_enemy(i);
      }
    }
    i++;
  }
  
}

void handle_pad(struct actor_attr* player)
 {
  char pad_result = pad_poll(0);
  
  if(player->invin > 0)
    player->invin--;
  if(has_attacked <= 0)
  	player->vel_x = 0;
  if(player->pos_y <= PLAYER_Y_MIN)
  {
    player->pos_y = PLAYER_Y_MIN + 1;
    player->state = FALLING;
  }
  if(player->vel_y < 1 && player->state == FALLING)
  {
    if(player->pos_y <= PLAYER_Y_MIN)
       player->pos_y = PLAYER_Y_MIN + 1;
    
    player->vel_y += 1;
  }
  else if(player->pos_y >= PLAYER_Y_MAX - 1 && player->state != ATTACKING)
  {
    player->state = STANDING;
  }
    
  if(pad_result & PAD_RIGHT)
    {
     if(player->pos_x <= PLAYER_X_MIN)
       player->pos_x = PLAYER_X_MIN + 1;
    
     if(player->state != JUMPING && player->state != FALLING)
       player->state = RUNNING;
     player->dir = 1;
     player->vel_x = 1;
    }
  if(pad_result & PAD_LEFT)
    {
     if(player->pos_x >= PLAYER_X_MAX)
       player->pos_x = PLAYER_X_MAX - 1;
    
     if(player->state != JUMPING && player->state != FALLING)
       player->state = RUNNING;
     player->dir = 0;
     player->vel_x = -1;
    }
  if(pad_result & PAD_DOWN)
    {
     if(player->pos_y <= PLAYER_Y_MIN)
       player->pos_y = PLAYER_Y_MIN + 1;
    
     player->vel_y = 1;
    }
  if(pad_result & PAD_UP)
    {
     if(player->pos_y >= PLAYER_Y_MAX)
       player->pos_y = PLAYER_Y_MAX - 1;
    
     player->vel_y = -1;
    }
  if(pad_result & PAD_START)
    {
    }
  if(pad_result & PAD_SELECT)
    {
    	if(player->invin <= 0)
    		decrement_life(player);
    }
  if(pad_result & PAD_B)
    {
    	if(has_attacked <= 0)
          player->vel_x = (player->dir?-1:1);
    	APU_ENABLE(ENABLE_PULSE1);
    	APU_PULSE_DECAY(PULSE_CH1, 2029, 64, 15, 8);
    	APU_PULSE_SWEEP(PULSE_CH1, 5, 3, 0);
    
    	player->state = ATTACKING;  
    }
  if(pad_result & PAD_A)
    {
      if(player->state == STANDING || player->state == RUNNING)
      {
        APU_ENABLE(ENABLE_PULSE0);
    	APU_PULSE_DECAY(PULSE_CH0, 362, 128, 15, 9);
    	APU_PULSE_SWEEP(PULSE_CH0, 3, 3, 1);
    	player->state = JUMPING;
      }
    }
  
  add_velocity(player);
  
  return;
 }

void handle_anim(struct actor_attr* player)
{ 
  switch(player->state)
  {
    case STANDING:
    	player->meta = playerSwinging[0 + (player->dir?0:5)];
    	break;
    case RUNNING:
        player->meta = playerRunning[((player->pos_x >> 1) & 11) + (player->dir?0:12)];
        break;
    case ATTACKING:
      player->invin = 15;
      has_attacked = 15;
      if(player->dir)
      	player->vel_x = 2;
      else
        player->vel_x = -2;
      if(nesclock()%5 == 0)
    	{
      	anim_number++;
        anim_number = anim_number % 5;
    	player->meta = playerSwinging[anim_number + (player->dir?0:5)];
          if(anim_number == 4)
          {
            player->state = STANDING;
            anim_number = 0;
            player->meta = playerSwinging[0 + (player->dir?0:5)];
          }
    	}
        break;
    case JUMPING:
      	player->pos_y -= 2;
        anim_loop++;
        if(anim_loop%10 == 0)
        {
          anim_number++;
          anim_number = anim_number % 2;
          player->meta = playerJumping[anim_number + (player->dir?0:4)];
          if(anim_loop >= 35)
          {
            player->state = FALLING;
            anim_loop = 0;
          }
        }
        break;
    case FALLING:
        if(nesclock()%10 == 0)
        {
          anim_number++;
          anim_number = anim_number % 4;
          player->meta = playerJumping[anim_number + (player->dir?0:4)];
          if(anim_number == 3)
          {
            anim_number = 0;
          }
        }
        break;
  }
}

void handle_time()
{
  vram_adr(NTADR_A(7, 3));
  vram_put(ZERO_TILE + hundreds);
  vram_put(ZERO_TILE + tens);
  vram_put(ZERO_TILE + ones);
  //vram_adr(NTADR_A(0, 0));
  
  if(ones <= 0 && tens > 0)
  {
    tens--;
    ones = 9;
  }
  else if(tens <= 0 && hundreds > 0)
  {
    hundreds--;
    tens = 9;
  }
  
  if(ones > 0 && nesclock() % 2 == 0)
  	ones--;
  
}


void draw_status_info()
{
  handle_time();
}

void load_level(struct level_data* level, struct actor_attr* player)
{ 
  short i;
  player->pos_x = level->player_spawn_x;
  player->pos_y = level->player_spawn_y;
  
  APU_ENABLE(ENABLE_PULSE0);
  APU_PULSE_DECAY(PULSE_CH0, 587, 128, 6, 6);
  APU_PULSE_SWEEP(PULSE_CH0, 4, 3, 1);
  APU_NOISE_DECAY(12|128, 3,13);
  
  ppu_off();
  vrambuf_clear();
  
  for(i = 0; i < MAX_ACTORS; i++) // "Clear" the enemies.
    actors[i] = empty_actor;
  
  for(i = 0; i < level->num_enemies; i++) // Reload them based on level data.
  {
    actors[i] = level->enemies[i];
  }
  
  set_vram_update(updbuf);
  
  // Give some extra time for reaching the next level
  hundreds += 3;
  
  // Put the next stage into memory.
  pal_bright(0);
  vram_adr(NTADR_A(0, 6));
  vram_unrle(level->map);
  pal_bright(4);
  ppu_on_all();
  
}

int draw_actors(int cur_oam)
{
  int i;
  int oam_count = cur_oam;
  for(i = 0; i < MAX_ACTORS; i++)
  {
    oam_count = oam_meta_spr(actors[i].pos_x, actors[i].pos_y, oam_count, actors[i].meta);
  };
  return oam_count;
};

////////////// main function, run after console reset /////////////////////////////////////////////////////////////////////////////
void main(void) {
  // ACTOR_TYPE, IS_ALIVE, X_POS, Y_POS, VEL_X, VEL_Y, DIR, LIFE, INVINC, STATE
  struct actor_attr p1 = {PLAYER, true, 120, 100, 0, 0, 0, 3, 0, FALLING};
  
  // set palette colors.
  pal_col(0,0x0F);	
  pal_col(1,0x0C);	
  pal_col(2,0x2E);	
  pal_col(3,0x04);
  
  // Character palette colors.
  pal_col(17, 0x06);
  pal_col(18, 0x36);
  pal_col(19, 0x28);
  
  bank_bg(0);
  
  vram_adr(NTADR_A(0, 0));
  vram_fill(0xA9, 32);
  vram_fill(0xA9, 128);
  vram_adr(NTADR_A(0, 5));
  vram_fill(0xB9, 32);
    
  ////// HEALTH /////////////
  vram_adr(NTADR_A(19, 4));
  vram_put(0xD3);
  vram_put(0xC8);
  vram_put(0xCC);
  vram_put(0xC4);
  vram_put(0xF9);
  vram_adr(NTADR_A(19, 2));
  vram_put(0xC7);
  vram_put(0xCF);
  vram_put(0xF9);
  vram_put(0xAC);
  vram_put(0xA9);
  vram_put(0xAC);
  vram_put(0xA9);
  vram_put(0xAC);
  vram_put(0xA9);
  //////////////////////////
  
  
  // enable PPU rendering (turn on screen)
  
  oam_clear();
  
  vrambuf_clear();
  set_vram_update(updbuf);
  
  load_level(&level_list[cur_level], &p1);
  
  ppu_on_all();
  
  // infinite loop
  while (1)
  {
    const unsigned char* meta = 0;
    char cur_oam = 0;
    //cur_oam = oam_spr(0, 48, 0x20, 0, cur_oam);
    
    handle_pad(&p1);
    
    handle_anim(&p1);
      
    bank_spr(1);
    cur_oam = oam_meta_spr(p1.pos_x, p1.pos_y, cur_oam, p1.meta);
    cur_oam = oam_spr(level_list[cur_level].portal_x, level_list[cur_level].portal_y, 0x4B, 0, cur_oam);
    
    
    if(p1.pos_x >= level_list[cur_level].portal_x && p1.pos_x <= level_list[cur_level].portal_x + 8
        && p1.pos_y >= level_list[cur_level].portal_y && p1.pos_y <= level_list[cur_level].portal_y + 8)
    {
      if(cur_level < LEVEL_COUNT - 1)
    	cur_level++;
      load_level(&level_list[cur_level], &p1);
    }
    
    cur_oam = draw_actors(cur_oam);
    cur_oam = cur_oam = oam_meta_spr(120, 16, cur_oam, playerSwinging[2]);
    
    
    ppu_wait_nmi();
    vrambuf_clear();
    handle_collision(&p1);    
    //draw_status_info();
  }
}


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


// link the pattern table into CHR ROM
//#link "chr_generic.s"
//#link "vrambuf.c"

///// METASPRITES
#define TILE 0xd8
#define ATTR 0

void handle_pad(int* pos_x, int* pos_y, int* cam_x, int* cam_y)
 {
    char pad_result = pad_poll(0);
  
  if(pad_result & 128) // Right
    {
      if(*cam_x < 0 && *pos_x == 120)
        *cam_x += 1;
      else
      {
        if(*pos_x + 8 < 240)
          *pos_x += 1;
      }
    }
  if(pad_result & 64) // Left
    {
      if(*cam_x > 0 && *pos_x == 120)
      	*cam_x -= 1;
      else
      {
        if(*pos_x - 8 > 0)
          *pos_x -= 1;
      }
    }
  if(pad_result & 32) // Down
    {
      if(*cam_y < 0 && *pos_y == 120)
        *cam_y += 1;
      else
      {
        if(*pos_y + 32 < 240)
          *pos_y += 1;
      }
    }
  if(pad_result & 16) // Up
    {
      if(*cam_y > -240 && *pos_y == 120)
      	*cam_y -= 1;
      else
      {
        if(*pos_y - 8> 0)
          *pos_y -= 1;
      }
    }
  if(pad_result & 8) // Start
    {
    }
  if(pad_result & 4) // Select
    {
    }
  if(pad_result & 2) // B
    {
    }
  if(pad_result & 1) // A
    {
    }
  return;
 }

// main function, run after console reset
void main(void) {
  int player_x = 120;
  int player_y = 120;
  int cam_x = 0;
  int cam_y = 0;
  char attributes = 0;
  int i;
  char door[12] = "On the door";
  unsigned char metasprite[]={
    0, 0, TILE+0, ATTR,
    0, 8, TILE+1, ATTR,
    8, 0, TILE+2, ATTR,
    8, 8, TILE+3, ATTR,
    128};
  
  // set palette colors.
  pal_col(0,0x06);	// set screen to dark blue
  pal_col(1,0x17);	// fuchsia
  pal_col(2,0x28);	// grey
  pal_col(3,0x0f);	// white
  
  // Character palette colors.
  pal_col(17, 0x11);
  pal_col(18, 0x2d);
  pal_col(19, 0x32);

  // write text to name table
  // Used to set "Static" Tiles for map creation.
  vram_adr(NTADR_A(1,1));		// set address
  vram_write("This is", 7);	// write bytes to video RAM
  vram_adr(NTADR_A(1,2));
  vram_write("Ethan Brown's", 13);
  vram_adr(NTADR_A(1,3));
  vram_write("First NES 'Game'!", 18);
  vram_adr(NTADR_A(29,18));
  vram_put('\xc4');
  vram_put('\xc6');
  vram_adr(NTADR_A(29,19));
  vram_put('\xc5');
  vram_put('\xc7');
  vram_put(' ');
  for(i = 0; i < 31; i++)
    vram_put('\xc0');
  
  // enable PPU rendering (turn on screen)
  ppu_on_all();
  
  vrambuf_clear();
  set_vram_update(updbuf);
  
  
  // infinite loop
  while (1)
  {
    char cur_oam = 0;
    
      //metasprite[3] ^= 1 << 6;
      //metasprite[7] ^= 1 << 6;
      //metasprite[11] ^= 1 << 6;
      //metasprite[15] ^= 1 << 6;
      //meta_sprite[0] = 0;
      //meta_sprite[4] = 0;
      //meta_sprite[8] = 8;
      //meta_sprite[12] = 8;
      
    handle_pad(&player_x, &player_y, &cam_x, &cam_y);
    
    scroll(cam_x, cam_y);
    
    oam_meta_spr(player_x, player_y, attributes, metasprite);
    
    
    vrambuf_flush();
  }
}

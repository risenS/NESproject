
/*
A simple "hello world" example.
Set the screen background color and palette colors.
Then write a message to the nametable.
Finally, turn on the PPU to display video.
*/

#include "neslib.h"
#include "vrambuf.h"
#define NES_MIRRORING 1
#include "bcd.h"


// link the pattern table into CHR ROM
//#link "chr_generic.s"
//#link "vrambuf.c"

///// METASPRITES
#define TILE 0xd8
#define ATTR 0

void handle_pad(int* x_pos, int* y_pos)
  {
    char pad_result = pad_poll(0);
  
    if(pad_result & 128) // Down
    {
      *x_pos += 1;
    }
    if(pad_result & 64) // Up
    {
      *x_pos -= 1;
    }
    if(pad_result & 32) // Right
    {
      *y_pos += 1;
    }
    if(pad_result & 16) // Left
    {
      *y_pos -= 1;
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
  }

// main function, run after console reset
void main(void) {
  int sprite_x = 16;
  int sprite_y = 142;
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
    
    if(sprite_x + 16 >= 248)
    {
      sprite_x -= 1;
    }
    else if(sprite_x <= 8)
    {
      sprite_x += 1;
      //metasprite[3] ^= 1 << 6;
      //metasprite[7] ^= 1 << 6;
      //metasprite[11] ^= 1 << 6;
      //metasprite[15] ^= 1 << 6;
      //meta_sprite[0] = 0;
      //meta_sprite[4] = 0;
      //meta_sprite[8] = 8;
      //meta_sprite[12] = 8;
    }
    
    if(sprite_x + 16 >= 232 && sprite_x + 16 <= 248 && sprite_y + 8 >= 144 && sprite_y <= 160)
    {
      vrambuf_put(NTADR_A(1, 4), door, 12);
    }
    else
    {
      vrambuf_put(NTADR_A(1, 4), "            ", 12);
    }
    
    handle_pad(&sprite_x, &sprite_y); // Input Handling.
    
    oam_meta_spr(sprite_x, sprite_y, attributes, metasprite); // Drawing Character to the screen.
    
    vrambuf_flush();
  }
}

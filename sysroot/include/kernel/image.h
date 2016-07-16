#ifndef IMAGE_H
#define IMAGE_H

#define PIXEL_ALPHA -1

#include <kernel/vgascreen.h>
#include <kernel/types.h>

void image_draw(vga_screen *scr,
  dword xorigin, dword yorigin,
  dword width, dword height,
  sbyte *buf);
  

#endif
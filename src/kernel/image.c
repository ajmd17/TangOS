#include <kernel/image.h>

void image_draw(vga_screen *scr,
  dword xorigin, dword yorigin,
  dword width, dword height,
  sbyte *buf) {

  for (dword x = 0; x < width; x++) {
    for (dword y = 0; y < height; y++) {
      dword pixel_index = width * y + x;
      sbyte pixel = buf[pixel_index];
      if (pixel != PIXEL_ALPHA) {
        vga_set_pixel(scr, x + xorigin, y + yorigin, (byte)pixel);
      }
    }
  }
}

void image_draw_subimage(vga_screen *scr,
  dword xoffset, dword yoffset,
  dword subwidth, dword subheight,
  dword xorigin, dword yorigin,
  dword width, dword height,
  sbyte *buf) {

  for (dword x = xoffset; x < subwidth + xoffset; x++) {
    for (dword y = yoffset; y < subheight + yoffset; y++) {
      dword pixel_index = width * y + x;
      sbyte pixel = buf[pixel_index];
      if (pixel != PIXEL_ALPHA) {
        vga_set_pixel(scr, x + xorigin, y + yorigin, (byte)pixel);
      }
    }
  }
}
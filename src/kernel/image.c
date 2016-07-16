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
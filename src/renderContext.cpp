#include "renderContext.hpp"
#include "font_data.hpp"
RenderContext::RenderContext(int width, int height) : scr(width, height) {
  this->width = width;
  this->height = height;
  this->count = width * height;
}

void RenderContext::clear() {
  for (int i = 0; i < count; i++)
    scr.native()[i] = {0, 0, 0};
}

void RenderContext::writeChar(int ch, int x, int y, color c) {
  for (int i = 0; i < 8; i++) {
    int yy = y + 8 - i;
    for (int w = 0; w < 8; w++) {
      int xx = x + w;
      u val = ((font8x8_basic[ch][i] >> w) & 1);
      scr[xx][yy] = {u(val * c[0]), u(val * c[1]), u(val * c[2])};
    }
  }
}

void RenderContext::drawBox(int x, int y, int width, int height, int c) {

  for (int i = x; i < (width + x); i++) {
    for (int w = 0; w < c; w++) {
      color col = {u(w * 10), u(w * 20), u(w * 30)};

      scr[i][y + w] = col;
      scr[i][y + height - w] = col;
    }
  }

  for (int i = y; i < (height + y); i++) {
    for (int w = 0; w < c; w++) {
      color col = {u(w * 10), u(w * 20), u(w * 30)};

      scr[x + w][i] = col;
      scr[x + width - w][i] = col;
    }
  }
}

void RenderContext::writeStr(color c, int x, int y, const char *str) {

  char ch;
  int originalX = x;
  while (ch = *str) {
    if (ch == '\n') {
      y -= 8;
      x = originalX;
      if (y < 0)
        break;

      continue;
    }
    writeChar(ch, x, y, c);
    x += 8;
    str++;
  }
}

#include "renderContext.hpp"
#include "font_data.hpp"

void RenderContext::clear() {
  for (int i = 0; i < scr.count(); i++)
    scr.native()[i] = {0, 0, 0};
}

void RenderContext::writeChar(int ch, int x, int y) {
  for (int i = 0; i < 8; i++) {
    int yy = y + 8 - i;
    for (int w = 0; w < 8; w++) {
      int xx = x + w;
      u val = ((font8x8_basic[ch][i] >> w) & 1);
      scr[xx][yy] = {u(val * brushColor[0]), u(val * brushColor[1]),
                     u(val * brushColor[2])};
    }
  }
}

void RenderContext::drawBox(int x, int y, int width, int height, int c) {

  color f{u(brushColor[0] / c), u(brushColor[1] / c), u(brushColor[2] / c)};

  for (int i = x; i < (width + x); i++) {
    for (int w = 0; w < c; w++) {
      color col = mul(Color(w, w, w), f);

      scr[i][y + w] = col;
      scr[i][y + height - w] = col;
    }
  }

  for (int i = y; i < (height + y); i++) {
    for (int w = 0; w < c; w++) {
      color col = mul(Color(w, w, w), f);

      scr[x + w][i] = col;
      scr[x + width - w][i] = col;
    }
  }

  for (int i = 0; i < c; i++) {
    for (int j = 0; j < c; j++) {
      int w = c - std::max(c - i, c - j);
      color re = mul(Color(w, w, w), f);

      scr[x + i][y + j] = re;
      scr[x + width - i][y + j] = re;
      scr[x + width - i][y + height - j] = re;
      scr[x + i][y + height - j] = re;
    }
  }
}

void RenderContext::fillBox(int x, int y, int width, int height) {
  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      scr[i + x][j + y] = brushColor;
    }
  }
}

void RenderContext::writeStr(int x, int y, const char *str) {

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
    writeChar(ch, x, y);
    x += 8;
    str++;
  }
}

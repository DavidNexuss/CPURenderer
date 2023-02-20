#include "renderContext.hpp"
#include "font_data.hpp"
#include <thread>
using u = unsigned char;

void RenderContext::clear() const {
  for (int i = 0; i < scr.count(); i++)
    scr.native()[i] = {0, 0, 0};
}

void RenderContext::writeChar(int ch, int x, int y) const {
  for (int i = 0; i < 8; i++) {
    int yy = y + 8 - i;
    for (int w = 0; w < 8; w++) {
      int xx = x + w;
      u val = ((font8x8_basic[ch][i] >> w) & 1);
      rgb col = rgb{val, val, val};
      scr[xx][yy] = brushColor * col;
    }
  }

  if (writeCharDelay != std::chrono::milliseconds::zero()) {
    std::this_thread::sleep_for(writeCharDelay);
  }
}

void RenderContext::drawBox(int x, int y, int width, int height, int c) const {

  rgb f{u(brushColor[0] / c), u(brushColor[1] / c), u(brushColor[2] / c)};

  for (int i = x; i < (width + x); i++) {
    for (int w = 0; w < c; w++) {
      rgb col = f * rgb(w);

      scr[i][y + w] = col;
      scr[i][y + height - w] = col;
    }
  }

  for (int i = y; i < (height + y); i++) {
    for (int w = 0; w < c; w++) {
      rgb col = f * rgb(w);

      scr[x + w][i] = col;
      scr[x + width - w][i] = col;
    }
  }

  for (int i = 0; i < c; i++) {
    for (int j = 0; j < c; j++) {
      int w = c - std::max(c - i, c - j);
      rgb re = f * rgb(w);

      scr[x + i][y + j] = re;
      scr[x + width - i][y + j] = re;
      scr[x + width - i][y + height - j] = re;
      scr[x + i][y + height - j] = re;
    }
  }
}

void RenderContext::fillBox(int x, int y, int width, int height) const {
  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      scr[i + x][j + y] = brushColor;
    }
  }
}

void RenderContext::writeStr(int x, int y, const char *str) const {

  char ch;
  int originalX = x;
  while (ch = *str) {
    if (ch == '\n') {
      y -= 8;
      x = originalX;
      if (y < 0)
        break;

    } else {
      writeChar(ch, x, y);
      x += 8;
    }
    str++;
  }
}

#include <stb/stb_image.h>

void RenderContext::writeData(int x, int y, int width, int height,
                              unsigned char *data) {
  for (int j = 0; j < height; j++) {
    for (int i = 0; i < width; i++) {
      scr[i + x][j + y] = {data[(j * width + i) * 3],
                           data[(j * width + i) * 3 + 1],
                           data[(j * width + i) * 3 + 2]

      };
    }
  }
}
void RenderContext::writeTexture(int x, int y, const char *texturePath) {
  int width;
  int height;
  int channels;
  stbi_set_flip_vertically_on_load(true);
  unsigned char *data = stbi_load(texturePath, &width, &height, &channels, 3);
  writeData(x, y, width, height, data);
  stbi_image_free(data);
}

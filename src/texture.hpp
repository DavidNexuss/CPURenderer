#pragma once
struct TextureFormat {
  int width;
  int height;
  int channels;
  int channelsSize;
  void *source;

  TextureFormat() {}

  TextureFormat(int width, int height) {
    this->width = width;
    this->height = height;
  }
};

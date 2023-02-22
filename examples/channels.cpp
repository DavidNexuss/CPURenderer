#include <player.hpp>
#include <renderContext.hpp>
#include <cstddef>
/***
 * This is a basic program to show the use of multiple texture rendering with
 * different number of channels and HDR
 */

/***
 * This is code is fore rendering a filled square with a variadic number of channels and channel types
 */

/* Type Trait trick to get max channel color with both 8 bit channels and 32 bit float channels */
template <typename T>
struct maxColor { };

template <>
struct maxColor<float> { inline static float max = 1.0f; };

template <>
struct maxColor<unsigned char> {inline static unsigned char max = 255; };

/**
 * This function will return the color white in any number of channels or format
 */
template <typename T, int channels>
static vec<T, channels> white() {
  vec<T, channels> brush;
  for(int i = 0; i < channels;i++)brush[i] = maxColor<T>::max;
  return brush;
}

/* Generic square drawing function */
template <typename T, int channels>
void drawSquare(int x, int y, Screen<T, channels>& scr) {
  const static vec<T, channels> brush = white<T, channels>();

  int l = 10;
  for(int i = -l/2; i < l/2; i++) {
    for(int j = -l/2; j < l/2; j++) {
      scr[i + x][j + y] = brush;
    }
  }
}

/***
 * Actual program begins here
 */

int main() {

  // We start by declaring our Player and ScreenBuffers

  const int width = 1920 / 2;
  const int height = 1080 / 2;

  Player player(width, height);
  ScreenBuffer<float, 3> RGBScreen(width, height);    //3 channel HDR texture
  ScreenBuffer<uint8_t, 3> rgbScreen(width, height);  //3 channel LDR texture
  ScreenBuffer<uint8_t, 2> rgScreen(width, height);   //2 channel LDR texture
  ScreenBuffer<uint8_t, 1> rScreen(width, height);    //1 channel LDR texture

  //Draw squares to each texture
  drawSquare(50,50, rgbScreen);
  drawSquare(100,100, rgScreen);
  drawSquare(150,50, rScreen);
  drawSquare(200,200, RGBScreen);

  //Draw some text
  RenderContext ctx(rgbScreen);
  ctx.writeStr(20, height - 20, "This is the multiple channel texture example");

  //Upload textures to the GPU
  player.uploadFrame(0,rgbScreen);
  player.uploadFrame(1, rgScreen);
  player.uploadFrame(2, rScreen);
  player.uploadFrame(3, RGBScreen);

  bool showFrames[] = {true, true, true, true};

  while(!player.shouldClose()) {

    //Window needs to be manually cleared for multi texture rendering
    player.clear();

    // Draw textures
    if(showFrames[0]) player.drawFrame(0, true);
    if(showFrames[1]) player.drawFrame(1, true);
    if(showFrames[2]) player.drawFrame(2, true);
    if(showFrames[3]) player.drawFrame(3, true);

    //Some UI options
    player.beginImgui();
    if(ImGui::Begin("Texture Channel Manager")) {
      ImGui::Text("In this window you can change the visibility options for each texture");
      ImGui::Checkbox("RGB texture", &showFrames[0]);
      ImGui::Checkbox("RG texture", &showFrames[1]);
      ImGui::Checkbox("R texture", &showFrames[2]);
      ImGui::Checkbox("RGB texture (HDR)", &showFrames[3]);
      ImGui::End();
    }
    player.endImgui();

    player.swapBuffers();
  }
}

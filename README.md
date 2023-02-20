# Basic raw rgb displayer

The basic idea behind this utility is to render raw rgb24 bits video input from stdin:
Useful for prototyping and playing with CPU rendering. This program uses OpenGL to stream each frame
the texture from the main memory.

## Interpreter mode


## Library mode
The program has also been extended to a library that you can link with your main program. Examples of the api
are in the examples directory.

By doing so, drawing something into screen becomes as trivial as writing to a specific memory locations, such:

```c++ 
//Set pixel at coordinates 25 100 to white
screen[25][100] = {255,255,255}

```
Of course this comes at the price of efficiency, constant streaming of a texture to the GPU and CPU rendering, but it can cover
cases where efficiency is not an important concern.

For easier use, it also comes with an optional CPU rendering class (RenderContext) with some premade functions for 
basic needs (render a box, render text...)


Example code in C++
``` c++ 

  int width = 1920 / 2;
  int height = 1080 / 2;
  // Create screen buffer
  std::vector<color> screenBuffer(width * height);

  // Create player
  Player player(width, height, 3);

  // Create render context (CPU rendering utilities)
  RenderContext ctx;
  ctx.scr = Screen{width, height, screenBuffer.data()};
  ctx.brushColor = {255, 255, 255};

  // Render loop
  while (!player.shouldClose()) {
    // Clear screen buffer
    ctx.clear();
    // Printf in memory position
    ctx.printf(20, height - 35,
               " Esta pantalla es de %dx%d:\n  Este es un mensaje de prueba.",
               width, height);

    // Draw box
    ctx.drawBox(10, height - 50, 250, 40, 10);

    // Upload screen buffer to the GPU
    player.uploadFrame((char *)screenBuffer.data());
    player.drawFrame();

    // Write a white pixel into coordinates (20,100)
    ctx.scr[20][100] = {255, 255, 255};

    // Sleep some time
    std::this_thread::sleep_for(16ms);
  }
```

# Basic raw rgb displayer

The basic idea behind this utility is to render raw rgb24 bits video input from stdin:
Useful for prototyping and playing with CPU rendering. This program uses OpenGL to stream each frame
the texture from the main memory.


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


Example code in examples folder.

## Build instructions

This is a cmake project so:

``` bash
cmake . -B build
cd build
make
```

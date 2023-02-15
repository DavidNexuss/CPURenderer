# Basic raw rgb displayer

The basic idea behind this utility is to render raw rgb24 bits video input from stdin:
Useful for prototyping and playing with CPU rendering:

Usage in C++:

Your application:



```
using color = array<unsigned char, 3>;

{
int width;
int height;
int channels;

cout >> width >> height >> channels; // Configure player

vector<color> screen(width, height, channels);

//Render to screen

int i = 0;
while(true) { 
  //Do drawing stuff
  screen[i % screen.size()] = {i * 2, i * 5, i * 8}; //Example drawing
  //Upload frame
  write(1, screen.data(), screen.size() * sizeof(color));
}
}
```

Now attach the player

./myprogram | ./player

And you will see the result with a gui

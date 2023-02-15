#version 330 core
in vec2 pos;
out vec2 fs_pos;
void main() { 
  fs_pos = pos;
  gl_Position = vec4(pos.x, pos.y, 0, 1);
}

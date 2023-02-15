#version 330 core
in vec2 fs_pos;
out vec3 color;

uniform sampler2D video;
void main() { 
  vec2 uv = (fs_pos + 1.0) * 0.5;
  color = texture2D(video, uv).xyz;
}

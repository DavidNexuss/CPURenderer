#pragma once
static const char *video_vs = "\
#version 330 core\n\
in vec2 pos;\n\
out vec2 fs_pos;\n\
void main() {\n\
  fs_pos = pos;\n\
  gl_Position = vec4(pos.x, pos.y, 0, 1);\n\
}\n\
";
static const char *video_fs = "\
#version 330 core\n\
out vec4 color;\n\
uniform sampler2D tex;\n\
in vec2 fs_pos;\n\
void main() {\n\
  color = texture(tex, fs_pos * 0.5 + 0.5);\n\
  color.w = 1.0; \n\
}\
";

const static float screenMesh[] = {-1, -1, -1, 1, 1, 1, 1, 1, -1, -1, 1, -1};

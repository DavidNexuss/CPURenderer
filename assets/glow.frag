#version 330 core

out vec3 color;
uniform sampler2D tex;
in vec2 fs_pos;

void main() {

  vec2 uv = fs_pos * 0.5 + 0.5;
  vec3 avg_glow = vec3(0.0);
  for(int i = -3; i <= 3; i++) { 
    for(int j = -3; j <= 3; j++) { 
      vec2 st = uv + vec2(float(i), float(j)) * 0.001;
      avg_glow += texture(tex, st).xyz;
    }
  }

  avg_glow = avg_glow / 20.0f;
  color = texture(tex, uv).xyz  + avg_glow * 0.6;
  color.x = texture(tex, uv + vec2(0.0001,0.0001)).x;
}

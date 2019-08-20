#version 430 core

in vec2 tex_coord;

out vec4 frag_color;

uniform sampler2D tex;

void main()
{
  // Grayscale intensity scaled by 10
  frag_color = vec4(vec3(texture(tex, tex_coord).r * 10.f), 1.f);
}

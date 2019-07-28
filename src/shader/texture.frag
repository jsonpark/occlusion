#version 430 core

in vec2 tex_coord;

out vec4 frag_color;

uniform sampler2D tex;

void main()
{
  frag_color = vec4(texture(tex, tex_coord).rgb, 1.f);
}

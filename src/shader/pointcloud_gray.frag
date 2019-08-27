#version 430 core

in vec3 frag_color;

out vec4 out_color;

void main()
{
  //out_color = vec4(0.5f, 0.5f, 0.5f, 1.0f);
  out_color = vec4(frag_color, 1.0f);
}

#version 430

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec3 frag_color;

void main()
{
  gl_Position = projection * view * model * vec4(position, 1.f);
  frag_color = color;
}

#version 430 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoordFlipped;

out vec2 tex_coord;

void main()
{
  gl_Position = vec4(aPos.x, aPos.y, 0.f, 1.f);

  // Depth image is given column-major
  tex_coord = vec2(1.f - aTexCoordFlipped.y, aTexCoordFlipped.x);
}

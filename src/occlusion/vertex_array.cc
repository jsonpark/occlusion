#include "occlusion/vertex_array.h"

#include <glad/glad.h>

namespace occlusion
{
VertexArray::VertexArray()
{
}

VertexArray::~VertexArray()
{
  if (generated_)
    glDeleteVertexArrays(1, &id_);
}

void VertexArray::Generate()
{
  glGenVertexArrays(1, &id_);
  generated_ = true;
}

void VertexArray::Bind()
{
  if (!generated_)
    Generate();

  glBindVertexArray(id_);
}

void VertexArray::Unbind()
{
  if (!generated_)
    return;

  glBindVertexArray(0);
}

void VertexArray::Draw()
{
  Bind();
  glDrawElements(DrawModeToGlEnum(mode_), count_, GL_UNSIGNED_INT, (void*)(sizeof(GLuint) * offset_));
  Unbind();
}
}

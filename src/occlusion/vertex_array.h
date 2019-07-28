#ifndef OCCLUSION_VERTEX_ARRAY_H_
#define OCCLUSION_VERTEX_ARRAY_H_

#include "occlusion/buffer.h"

namespace occlusion
{
class VertexArray
{
public:
  enum class DrawMode
  {
    Lines,
    LineStrip,
    Triangles,
    TriangleStrip,
    TriangleFan,
  };

  static constexpr auto DrawModeToGlEnum(DrawMode mode)
  {
    switch (mode)
    {
    case DrawMode::Lines:
      return GL_LINES;

    case DrawMode::LineStrip:
      return GL_LINE_STRIP;

    case DrawMode::Triangles:
      return GL_TRIANGLES;

    case DrawMode::TriangleStrip:
      return GL_TRIANGLE_STRIP;

    case DrawMode::TriangleFan:
      return GL_TRIANGLE_FAN;
    }
  }

public:
  VertexArray();
  ~VertexArray();

  void Bind();
  void Unbind();

  template<typename T, BufferType type, BufferUsage usage>
  void VertexAttribPointer(Buffer<T, type, usage>& buffer, int idx, int size, int stride, int offset)
  {
    if (!generated_)
      Generate();

    Bind();
    buffer.Bind();
    glVertexAttribPointer(idx, size, BaseTypeToGlEnum<T>(), GL_FALSE, stride * sizeof(T), (void*)(offset * sizeof(T)));
    glEnableVertexAttribArray(idx);
    Unbind();
    buffer.Unbind();
  }

  template<typename T, BufferUsage usage>
  void ElementPointer(Buffer<T, BufferType::ElementArray, usage>& buffer)
  {
    Bind();
    buffer.Bind();
    Unbind();
    buffer.Unbind();
  }

  template<typename T, BufferUsage usage>
  void SetupDrawElements(Buffer<T, BufferType::ElementArray, usage>& buffer, DrawMode mode, int offset, int count)
  {
    ElementPointer(buffer);

    mode_ = mode;
    offset_ = offset;
    count_ = count;
  }

  void Draw();

private:
  void Generate();

  bool generated_ = false;

  DrawMode mode_ = DrawMode::Lines;
  int offset_ = 0;
  int count_ = 0;

  // GL variables
  GLuint id_ = 0;
};
}

#endif // OCCLUSION_VERTEX_ARRAY_H_

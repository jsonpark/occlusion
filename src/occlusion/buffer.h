#ifndef OCCLUSION_BUFFER_H_
#define OCCLUSION_BUFFER_H_

#include <vector>

#include "occlusion/shader.h"

#include <glad/glad.h>

namespace occlusion
{
enum class BufferType
{
  Array,
  ElementArray,
};

enum class BufferUsage
{
  StaticDraw,
  DynamicDraw,
};

static constexpr auto BufferTypeToGlEnum(BufferType type) noexcept
{
  switch (type)
  {
  case BufferType::Array:
    return GL_ARRAY_BUFFER;

  case BufferType::ElementArray:
    return GL_ELEMENT_ARRAY_BUFFER;
  }
}

static constexpr auto BufferUsageToGlEnum(BufferUsage usage) noexcept
{
  switch (usage)
  {
  case BufferUsage::StaticDraw:
    return GL_STATIC_DRAW;

  case BufferUsage::DynamicDraw:
    return GL_DYNAMIC_DRAW;
  }
}

template<typename T>
constexpr GLenum BaseTypeToGlEnum() noexcept
{
  return 0;
}

template<>
constexpr GLenum BaseTypeToGlEnum<float>() noexcept
{
  return GL_FLOAT;
}

template<>
constexpr GLenum BaseTypeToGlEnum<unsigned int>() noexcept
{
  return GL_UNSIGNED_INT;
}

template<typename T, BufferType type, BufferUsage usage>
class Buffer
{
public:
  Buffer() = delete;

  Buffer(int n)
    : n_(n), buffer_(n, static_cast<T>(0))
  {
  }

  ~Buffer()
  {
    if (allocated_)
      Deallocate();
  }

  void Allocate()
  {
    if (allocated_)
      Deallocate();

    allocated_ = true;

    gl_type_ = BufferTypeToGlEnum(type);

    glGenBuffers(1, &id_);

    glBindBuffer(gl_type_, id_);
    glBufferData(gl_type_, sizeof(T) * n_, 0, BufferUsageToGlEnum(usage));
    glBindBuffer(gl_type_, 0);
  }

  void Deallocate()
  {
    if (!allocated_)
      return;

    glDeleteBuffers(1, &id_);
    allocated_ = false;
  }

  void Bind()
  {
    if (!allocated_)
      Allocate();

    glBindBuffer(gl_type_, id_);
  }

  void Unbind()
  {
    if (!allocated_)
      return;

    glBindBuffer(gl_type_, 0);
  }

  void Update()
  {
    if (!allocated_)
      Allocate();

    glBindBuffer(gl_type_, id_);
    glBufferSubData(gl_type_, 0, sizeof(T) * buffer_.size(), &buffer_[0]);
    glBindBuffer(gl_type_, 0);
  }

  T& operator [] (int idx)
  {
    return buffer_[idx];
  }

  const T& operator[] (int idx) const
  {
    return buffer_[idx];
  }

private:
  bool allocated_ = false;
  std::vector<T> buffer_;
  int n_;

  // GL variables
  GLuint id_ = 0;
  GLenum gl_type_;
};
}

#endif // OCCLUSION_BUFFER_H_

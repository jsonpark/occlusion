#ifndef OCCLUSION_ENGINE_H_
#define OCCLUSION_ENGINE_H_

#include "occlusion/program.h"
#include "occlusion/vertex_array.h"
#include "occlusion/texture.h"

struct GLFWwindow;

namespace occlusion
{
class Engine
{
public:
  Engine();

  void Run();

  void Resize(int width, int height);

private:
  void Initialize();
  void Draw();

  // Window
  GLFWwindow* window_;
  int width_;
  int height_;

  // Shaders
  void LoadShaders();

  Program shader_color_;

  // Objects
  Buffer<float, BufferType::Array, BufferUsage::StaticDraw> rectangle_buffer_{ 16 };
  Buffer<unsigned int, BufferType::ElementArray, BufferUsage::StaticDraw> rectangle_index_buffer_{ 4 };
  VertexArray rectangle_;

  Texture texture_;
};
}

#endif // OCCLUSION_ENGINE_H_

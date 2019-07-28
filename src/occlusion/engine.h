#ifndef OCCLUSION_ENGINE_H_
#define OCCLUSION_ENGINE_H_

#include "occlusion/program.h"
#include "occlusion/texture.h"
#include "occlusion/dataset_utkinect.h"

struct GLFWwindow;

namespace occlusion
{
class Engine
{
public:
  Engine();
  ~Engine();

  void Run();

  void Resize(int width, int height);
  void Keyboard(int key, int scancode, int action, int mods);

private:
  void Initialize();
  void Draw();

  // Window
  GLFWwindow* window_ = 0;
  int width_ = 800;
  int height_ = 600;

  bool redraw_ = true;
  bool animation_ = false;
  double animation_absolute_time_ = 0.;
  double animation_time_ = 0.;

  // Shaders
  void LoadShaders();

  Program shader_color_;

  // Objects
  GLuint rectangle_vbo_ = 0;
  GLuint rectangle_ibo_ = 0;
  GLuint rectangle_vao_ = 0;
  GLuint texture_ = 0;

  UtKinect utkinect_;
};
}

#endif // OCCLUSION_ENGINE_H_

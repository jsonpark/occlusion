#ifndef OCCLUSION_ENGINE_H_
#define OCCLUSION_ENGINE_H_

#include "occlusion/program.h"
#include "occlusion/texture.h"
#include "occlusion/dataset_utkinect.h"
#include "occlusion/dataset_wnp.h"

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
  Program shader_depth_;

  // Objects
  GLuint color_rectangle_vbo_ = 0;
  GLuint color_rectangle_vao_ = 0;
  GLuint depth_rectangle_vbo_ = 0;
  GLuint depth_rectangle_vao_ = 0;
  GLuint rectangle_ibo_ = 0;
  GLuint color_texture_ = 0;
  GLuint depth_texture_ = 0;

  // Dataset
  UtKinect utkinect_;
  Wnp wnp_;

  Dataset* dataset_;
};
}

#endif // OCCLUSION_ENGINE_H_

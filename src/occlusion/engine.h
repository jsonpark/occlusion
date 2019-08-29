#ifndef OCCLUSION_ENGINE_H_
#define OCCLUSION_ENGINE_H_

#include "occlusion/program.h"
#include "occlusion/texture.h"
#include "occlusion/dataset_utkinect.h"
#include "occlusion/dataset_wnp.h"
#include "occlusion/kinect_v2.h"
#include "occlusion/camera.h"
#include "occlusion/robot_model_loader.h"
#include "occlusion/robot_model.h"
#include "occlusion/light.h"
#include "occlusion/mesh_object.h"

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
  void CursorPos(float xpos, float ypos);
  void MouseButton(int button, int action, int mods, float xpos, float ypos);

private:
  void Initialize();
  void Draw();

  void DrawColorDepthImages(const std::vector<unsigned char>& color, const std::vector<unsigned short>& depth);
  void DrawPointCloud(const std::vector<float>& point_cloud, const std::vector<float>& point_cloud_color);
  void DrawRobot();

  // Window
  GLFWwindow* window_ = 0;
  int width_ = 800;
  int height_ = 600;

  bool redraw_ = true;
  bool animation_ = false;
  double animation_absolute_time_ = 0.;
  double animation_time_ = 0.;

  bool mouse_button_status_[3];
  float mouse_last_x_;
  float mouse_last_y_;

  Camera camera_;

  // Shaders
  void LoadShaders();

  Program shader_color_;
  Program shader_depth_;
  Program shader_point_cloud_;
  Program shader_robot_;

  // Objects
  GLuint color_rectangle_vao_ = 0;
  GLuint color_rectangle_vbo_ = 0;
  GLuint depth_rectangle_vao_ = 0;
  GLuint depth_rectangle_vbo_ = 0;
  GLuint rectangle_ibo_ = 0;
  GLuint color_texture_ = 0;
  GLuint depth_texture_ = 0;

  GLuint point_cloud_vao_ = 0;
  GLuint point_cloud_vbo_ = 0;
  GLuint point_cloud_color_vbo_ = 0;

  // Dataset
  UtKinect utkinect_;
  Wnp wnp_;

  Dataset* dataset_;

  // Kinect V2
  KinectV2 kinect_;

  // Robot
  void LoadRobotModel();
  void LoadRobotMeshObjects(std::shared_ptr<RobotLink> link);

  RobotModelLoader robot_model_loader_;
  std::shared_ptr<RobotModel> robot_model_;

  // Rendering lights
  void LoadShaderLightUniforms(Program& shader);

  std::vector<Light> lights_;
};
}

#endif // OCCLUSION_ENGINE_H_

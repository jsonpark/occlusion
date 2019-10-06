#ifndef OCCLUSION_ENGINE_H_
#define OCCLUSION_ENGINE_H_

#include "occlusion/program.h"
#include "occlusion/dataset_utkinect.h"
#include "occlusion/dataset_wnp.h"
#include "occlusion/kinect_v2.h"
#include "occlusion/camera.h"
#include "occlusion/robot_model_loader.h"
#include "occlusion/robot_model.h"
#include "occlusion/light.h"
#include "occlusion/mesh_object.h"
#include "occlusion/robot_state.h"
#include "occlusion/texture_object.h"
#include "occlusion/widget/widget.h"

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

  void UseColorImageShader();
  void UseGrayImageShader();

private:
  void Initialize();
  void Draw();

  void UpdateColorDepthImages(const std::vector<unsigned char>& color, const std::vector<unsigned short>& depth);
  void DrawPointCloud(const std::vector<float>& point_cloud, const std::vector<float>& point_cloud_color);
  void DrawRobot();

  void InitializeRobotState();

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

  // Widgets
  std::vector<std::shared_ptr<Widget>> widgets_;

  // Shaders
  void LoadShaders();

  Program shader_color_;
  Program shader_depth_;
  Program shader_point_cloud_;
  Program shader_robot_;

  // Objects
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
  void ViewFromRobotCamera();

  RobotModelLoader robot_model_loader_;
  std::shared_ptr<RobotModel> robot_model_;
  std::shared_ptr<RobotState> robot_state_;

  std::unordered_map<std::string, MeshObject> mesh_objects_;

  // Textures
  std::unordered_map<std::string, TextureObject> texture_objects_;

  // Rendering lights
  void LoadShaderLightUniforms(Program& shader);

  std::vector<Light> lights_;
};
}

#endif // OCCLUSION_ENGINE_H_

#ifndef OCCLUSION_ENGINE_H_
#define OCCLUSION_ENGINE_H_

#include "occlusion/shader/shaders.h"
#include "occlusion/dataset_utkinect.h"
#include "occlusion/dataset_wnp.h"
#include "occlusion/kinect_v2.h"
#include "occlusion/camera.h"
#include "occlusion/robot_model_loader.h"
#include "occlusion/robot_model.h"
#include "occlusion/robot_state.h"
#include "occlusion/object/mesh_object.h"
#include "occlusion/object/texture_object.h"
#include "occlusion/object/point_cloud_object.h"
#include "occlusion/widget/widget.h"
#include "occlusion/data/light.h"
#include "occlusion/data/point_cloud.h"
#include "occlusion/scene/scene.h"

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

  void UseShader(const std::string& name);

private:
  void Initialize();
  void Draw();

  void UpdateColorDepthImages(const std::vector<unsigned char>& color, const std::vector<unsigned short>& depth);
  void DrawPointCloud();
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

  // Scene
  std::shared_ptr<Scene> scene_;

  Camera camera_;

  // Widgets
  std::vector<std::shared_ptr<Widget>> widgets_;

  // Shaders
  void LoadShaders();

  Shaders shaders_;
  Program* shader_;

  // Objects
  std::shared_ptr<PointCloudObject> point_cloud_object_;

  // Dataset
  UtKinect utkinect_;
  Wnp wnp_;

  Dataset* dataset_;

  // Kinect V2
  KinectV2 kinect_;

  // Robot
  void LoadRobotModel();
  void ViewFromRobotCamera();

  std::shared_ptr<RobotModel> robot_model_;
  std::shared_ptr<RobotState> robot_state_;

  std::unordered_map<std::string, MeshObject> mesh_objects_;

  // Textures
  std::unordered_map<std::string, TextureObject> texture_objects_;

  // Rendering lights
  void LoadShaderLightUniforms(const std::vector<Light>& lights);
};
}

#endif // OCCLUSION_ENGINE_H_

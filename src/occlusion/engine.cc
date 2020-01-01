#include "occlusion/engine.h"

#include <iostream>
#include <thread>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "occlusion/widget/image_viewer.h"
#include "occlusion/widget/scene_viewer.h"
#include "occlusion/scene/scene_edge.h"

namespace occlusion
{
namespace
{
const double PI = 3.1415926535897932384626433832795;

void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
  Engine* engine = static_cast<Engine*>(glfwGetWindowUserPointer(window));
  engine->Resize(width, height);
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  Engine* engine = static_cast<Engine*>(glfwGetWindowUserPointer(window));
  engine->Keyboard(key, scancode, action, mods);
}

void CursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
  Engine* engine = static_cast<Engine*>(glfwGetWindowUserPointer(window));
  engine->CursorPos(xpos, ypos);
}

void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
  Engine* engine = static_cast<Engine*>(glfwGetWindowUserPointer(window));

  double xpos, ypos;
  glfwGetCursorPos(window, &xpos, &ypos);

  engine->MouseButton(button, action, mods, xpos, ypos);
}
}

Engine::Engine()
  : width_(800), height_(600)
{
}

Engine::~Engine()
{
}

void Engine::UseShader(const std::string& name)
{
  shader_ = &shaders_[name];
  shader_->Use();
}

void Engine::Run()
{
  // GLFW window
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window_ = glfwCreateWindow(width_, height_, "Dataset", NULL, NULL);
  if (window_ == NULL)
  {
    std::cerr << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return;
  }

  glfwSwapInterval(1);

  glfwMakeContextCurrent(window_);

  // GLAD
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cerr << "Failed to initialize GLAD" << std::endl;
    return;
  }

  // GLFW event callbacks
  glfwSetWindowUserPointer(window_, this);
  glfwSetFramebufferSizeCallback(window_, FramebufferSizeCallback);
  glfwSetKeyCallback(window_, KeyCallback);
  glfwSetCursorPosCallback(window_, CursorPosCallback);
  glfwSetMouseButtonCallback(window_, MouseButtonCallback);

  Resize(width_, height_);

  //glfwMaximizeWindow(window_);

  // Rendering loop
  Initialize();

  const double fps = dataset_->FrameRate();

  while (!glfwWindowShouldClose(window_))
  {
    if (animation_)
    {
      double current_time = glfwGetTime();
      auto current_frame = static_cast<int>(animation_time_ * fps);
      animation_time_ += current_time - animation_absolute_time_;
      animation_absolute_time_ = current_time;
      auto next_frame = static_cast<int>(animation_time_ * fps);

      while (current_frame < next_frame)
      {
        if (!dataset_->NextFrame())
        {
          animation_ = false;
          break;
        }

        redraw_ = true;
        current_frame++;
      }
    }

    if (redraw_)
    {
      Draw();

      glfwSwapBuffers(window_);
      redraw_ = false;
    }

    glfwPollEvents();

    using namespace std::chrono_literals;
    std::this_thread::sleep_for(1ms);
  }

  // Finalize
  glfwTerminate();
}

void Engine::Resize(int width, int height)
{
  width_ = width;
  height_ = height;

  if (widgets_.size() >= 4)
  {
    widgets_[0]->Resize(0, 0, width_ / 2, height_ / 2);
    widgets_[1]->Resize(0, height_ / 2, width_ / 2, height_ / 2);
    widgets_[2]->Resize(width_ / 2, 0, (width_ + 1) / 2, height_ / 2);
    widgets_[3]->Resize(width_ / 2, height_ / 2, (width_ + 1) / 2, (height_ + 1) / 2);
  }
}

void Engine::Keyboard(int key, int scancode, int action, int mods)
{
  if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
  {
    if (dataset_->PreviousFrame())
      redraw_ = true;
  }

  else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
  {
    if (dataset_->NextFrame())
      redraw_ = true;
  }

  else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
  {
    if (dataset_->NextSequence())
    {
      redraw_ = true;
      animation_ = false;
      animation_time_ = 0.;
    }
  }

  else if (key == GLFW_KEY_UP && action == GLFW_PRESS)
  {
    if (dataset_->PreviousSequence())
    {
      redraw_ = true;
      animation_ = false;
      animation_time_ = 0.;
    }
  }

  else if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
  {
    if (!animation_)
    {
      animation_ = true;
      animation_time_ = 0.;
      animation_absolute_time_ = glfwGetTime();
    }
    else
      animation_ = false;
  }
}

void Engine::CursorPos(float xpos, float ypos)
{
  float dx = xpos - mouse_last_x_;
  float dy = ypos - mouse_last_y_;

  if (!mouse_button_status_[1])
  {
    // Zoom
    if (mouse_button_status_[0] && mouse_button_status_[2])
      camera_.Zoom(dy);

    else if (mouse_button_status_[0])
      camera_.Rotate(dx, dy);

    else if (mouse_button_status_[2])
      camera_.Translate(dx, dy);
  }

  mouse_last_x_ = xpos;
  mouse_last_y_ = ypos;

  redraw_ = true;
}

void Engine::MouseButton(int button, int action, int mods, float xpos, float ypos)
{
  bool pressed = (action == GLFW_PRESS);

  if (button == GLFW_MOUSE_BUTTON_LEFT)
    mouse_button_status_[0] = pressed;

  else if (button == GLFW_MOUSE_BUTTON_MIDDLE)
    mouse_button_status_[1] = pressed;

  else if (button == GLFW_MOUSE_BUTTON_RIGHT)
    mouse_button_status_[2] = pressed;

  mouse_last_x_ = xpos;
  mouse_last_y_ = ypos;

  redraw_ = true;
}

void Engine::Initialize()
{
  // Widgets
  widgets_.push_back(std::make_shared<SceneViewer>(this, 0, 0, width_ / 2, height_ / 2));
  widgets_.push_back(std::make_shared<SceneViewer>(this, 0, height_ / 2, width_ / 2, (height_ + 1) / 2));
  widgets_.push_back(std::make_shared<ImageViewer>(this, width_ / 2, 0, (width_ + 1) / 2, height_ / 4));
  widgets_.push_back(std::make_shared<ImageViewer>(this, width_ / 2, height_ / 4, (width_ + 1) / 2, (height_ + 3) / 4));

  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glEnable(GL_DEPTH_TEST);

  LoadShaders();
  LoadRobotModel();
  InitializeRobotState();

  // Camera
  camera_.SetPerspective();
  camera_.SetAspect(static_cast<float>(width_) / height_);
  camera_.SetFovy(60.f / 3.1415926535897932384626433832795f * 2.f);

  /*
  camera_.SetEye(-1.f, 0.f, 1.f);
  camera_.SetCenter(0.f, 0.f, 1.f);
  camera_.SetUp(0.f, 0.f, 1.f);
  */

  mouse_button_status_[0] = false;
  mouse_button_status_[1] = false;
  mouse_button_status_[2] = false;

  // Scene
  scene_ = std::make_shared<Scene>();

  auto root = scene_->GetRoot();

  auto camera_node = std::make_shared<SceneCamera>();
  SceneEdge::Create(root, camera_node);

  // Light
  Light light;
  light.type = Light::Type::Directional;
  light.position = Vector3f(0.f, 0.f, 1.f);
  light.ambient = Vector3f(0.2f, 0.2f, 0.2f);
  light.diffuse = Vector3f(0.2f, 0.2f, 0.2f);
  light.specular = Vector3f(0.2f, 0.2f, 0.2f);

  scene_->AddLight(light);

  light.position = Vector3f(-1.f, -1.f, 1.f);
  scene_->AddLight(light);

  light.position = Vector3f(-1.f, 1.f, 1.f);
  scene_->AddLight(light);

  light.position = Vector3f(1.f, -1.f, 1.f);
  scene_->AddLight(light);

  light.position = Vector3f(1.f, 1.f, 1.f);
  scene_->AddLight(light);

  // Dataset
  dataset_ = &wnp_;

  dataset_->SelectSequence(0);

  // Point cloud vertex array
  point_cloud_object_ = std::make_shared<PointCloudObject>(dataset_->DepthWidth() * dataset_->DepthHeight());
}

void Engine::UseColorImageShader()
{
  UseShader("color");
}

void Engine::UseGrayImageShader()
{
  UseShader("depth");
}

void Engine::UpdateColorDepthImages(const std::vector<unsigned char>& color, const std::vector<unsigned short>& depth)
{
  auto image_viewer = std::dynamic_pointer_cast<ImageViewer>(widgets_[2]);
  image_viewer->SetColorImage(dataset_->RgbWidth(), dataset_->RgbHeight(), color);

  auto depth_viewer = std::dynamic_pointer_cast<ImageViewer>(widgets_[3]);
  depth_viewer->SetGrayImage(dataset_->DepthWidth(), dataset_->DepthHeight(), depth, true);
}

void Engine::DrawPointCloud()
{
  UseShader("pointcloud");

  Affine3d model = robot_state_->GetLinkTransform("head_camera_rgb_optical_frame");
  model.rotate(Eigen::AngleAxisd(PI / 2., Vector3d(0., 1., 0.)));
  model.rotate(Eigen::AngleAxisd(-PI / 2., Vector3d(1., 0., 0.)));

  shader_->UniformMatrix4f("projection", camera_.ProjectionMatrix());
  shader_->UniformMatrix4f("view", camera_.ViewMatrix());
  shader_->UniformMatrix4f("model", model.cast<float>().matrix());
  shader_->Uniform4f("width_height_near_far", dataset_->DepthWidth(), dataset_->DepthHeight(), camera_.GetNear(), camera_.GetFar());

  point_cloud_object_->Draw();
}

void Engine::Draw()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  robot_state_->ForwardKinematics();

  auto rgb_image = dataset_->GetRgbImage();
  auto depth_image = dataset_->GetDepthImage();

  UpdateColorDepthImages(rgb_image, depth_image);

  widgets_[2]->Draw();
  widgets_[3]->Draw();

  // Point cloud
  glViewport(0, 0, width_ / 2, height_ / 2);

  kinect_.FeedFrame(std::move(rgb_image), std::move(depth_image));
  kinect_.GeneratePointCloud();
  point_cloud_object_->Update(kinect_.GetPointCloud());

  DrawPointCloud();

  // Robot
  DrawRobot();
}

void Engine::ViewFromRobotCamera()
{
  Affine3d model = robot_state_->GetLinkTransform("head_camera_rgb_optical_frame");

  Matrix4f transform = model.cast<float>().matrix();
  Vector3f x = transform.block(0, 0, 3, 1);
  Vector3f y = transform.block(0, 1, 3, 1);
  Vector3f eye = transform.block(0, 3, 3, 1);

  camera_.SetEye(eye + y * 0.001);
  camera_.SetCenter(eye + y * 1.001);
  camera_.SetUp(x);
}

void Engine::DrawRobot()
{
  UseShader("robot");

  LoadShaderLightUniforms(scene_->GetLights());

  Matrix4f id = Matrix4f::Identity();
  shader_->UniformMatrix4f("projection", camera_.ProjectionMatrix());
  shader_->UniformMatrix4f("view", camera_.ViewMatrix());
  shader_->UniformMatrix4f("model", id);

  shader_->Uniform3f("eye_position", camera_.GetEye());
  shader_->Uniform1i("diffuse_texture", 0);

  shader_->Uniform3f("material.ambient", Vector3f(0.2f, 0.2f, 0.2f));
  shader_->Uniform3f("material.diffuse", Vector3f(0.2f, 0.2f, 0.2f));
  shader_->Uniform3f("material.specular", Vector3f(1.0f, 1.0f, 1.0f));
  shader_->Uniform1f("material.shininess", 100.f);

  for (const auto& link_transform : robot_state_->GetLinkTransforms())
  {
    auto link = link_transform.first;
    const auto& transform = link_transform.second;

    for (const auto& visual : link->GetVisuals())
    {
      Affine3f model = (transform * visual.origin).cast<float>();
      const auto& mesh_object = mesh_objects_[visual.geometry.mesh_filename];

      if (mesh_object.HasDiffuseTexture())
      {
        shader_->Uniform1i("has_diffuse_texture", 1);
        const auto& texture_filename = mesh_object.GetDiffuseTextureFilename();
        const auto& texture_object = texture_objects_.find(texture_filename)->second;
        glBindTexture(GL_TEXTURE_2D, texture_object.GetTextureId());
      }
      else
        shader_->Uniform1i("has_diffuse_texture", 0);

      shader_->UniformMatrix4f("model", model.matrix());
      mesh_object.Draw();
      glBindTexture(GL_TEXTURE_2D, 0);
    }
  }
}

void Engine::LoadShaderLightUniforms(const std::vector<Light>& lights)
{
  for (int i = 0; i < lights.size(); i++)
  {
    const auto& light = lights[i];
    std::string light_name = "lights[" + std::to_string(i) + "]";

    shader_->Uniform1i(light_name + ".use", 1);

    switch (light.type)
    {
    case Light::Type::Directional:
      shader_->Uniform1i(light_name + ".type", 0);
      break;

    case Light::Type::Point:
      shader_->Uniform1i(light_name + ".type", 1);
      shader_->Uniform3f(light_name + ".attenuation", light.attenuation);
      break;
    }

    shader_->Uniform3f(light_name + ".position", light.position);
    shader_->Uniform3f(light_name + ".ambient", light.ambient);
    shader_->Uniform3f(light_name + ".diffuse", light.diffuse);
    shader_->Uniform3f(light_name + ".specular", light.specular);
  }

  for (int i = lights.size(); i < 8; i++)
    shader_->Uniform1i("lights[" + std::to_string(i) + "].use", 0);
}

void Engine::LoadShaders()
{
  shaders_.AddShader("color", "..\\src\\shader\\texture");
  shaders_.AddShader("depth", "..\\src\\shader\\depth");
  shaders_.AddShader("pointcloud", "..\\src\\shader\\pointcloud");
  shaders_.AddShader("robot", "..\\src\\shader\\robot");
}

void Engine::LoadRobotModel()
{
  RobotModelLoader robot_model_loader;
  robot_model_loader.SetPackageDirectory("..\\..\\fetch_ros");
  robot_model_ = robot_model_loader.Load("package://fetch_description\\robots\\fetch.urdf");

  robot_state_ = std::make_shared<RobotState>(robot_model_);

  for (const auto& link_transform : robot_state_->GetLinkTransforms())
  {
    auto link = link_transform.first;

    for (const auto& visual : link->GetVisuals())
    {
      MeshObject mesh_object(visual.geometry.mesh_filename);

      if (mesh_object.HasDiffuseTexture())
      {
        const auto& texture_filename = mesh_object.GetDiffuseTextureFilename();
        texture_objects_.insert({texture_filename, TextureObject(texture_filename)});
      }

      mesh_objects_.insert({ visual.geometry.mesh_filename, std::move(mesh_object) });
    }
  }
}

void Engine::InitializeRobotState()
{
  robot_state_->SetJointValue("torso_lift_joint", 0.193075);
  robot_state_->SetJointValue("head_tilt_joint", 0.345);
  robot_state_->SetJointValue("shoulder_pan_joint", -0.2338021);
  robot_state_->SetJointValue("shoulder_lift_joint", 0.21);
  robot_state_->SetJointValue("upperarm_roll_joint", -0.331486);
  robot_state_->SetJointValue("elbow_flex_joint", -0.455);
  robot_state_->SetJointValue("forearm_roll_joint", -0.231486);
  robot_state_->SetJointValue("wrist_flex_joint", 1.31874);
  robot_state_->SetJointValue("wrist_roll_joint", -0.429902);
  robot_state_->SetJointValue("r_gripper_finger_joint", 0.025);
  robot_state_->SetJointValue("l_gripper_finger_joint", 0.025);
  robot_state_->SetJointValue("bellows_joint", 0.2);

  robot_state_->ForwardKinematics();
  ViewFromRobotCamera();
}
}

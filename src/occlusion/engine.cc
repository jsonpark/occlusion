#include "occlusion/engine.h"

#include <iostream>
#include <thread>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace occlusion
{
namespace
{
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
}

Engine::Engine()
  : width_(1366), height_(768)
{
}

Engine::~Engine()
{
  glDeleteBuffers(1, &color_rectangle_vbo_);
  glDeleteVertexArrays(1, &color_rectangle_vao_);
  glDeleteBuffers(1, &depth_rectangle_vbo_);
  glDeleteVertexArrays(1, &depth_rectangle_vao_);
  glDeleteBuffers(1, &rectangle_ibo_);
  glDeleteTextures(1, &color_texture_);
  glDeleteTextures(1, &depth_texture_);
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

  Resize(width_, height_);

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
  glViewport(0, 0, width, height);
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

void Engine::Initialize()
{
  glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  LoadShaders();

  // Dataset
  dataset_ = &wnp_;

  dataset_->SelectSequence(0);

  glGenTextures(1, &color_texture_);
  glBindTexture(GL_TEXTURE_2D, color_texture_);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB, dataset_->RgbWidth(), dataset_->RgbHeight());

  glGenTextures(1, &depth_texture_);
  glBindTexture(GL_TEXTURE_2D, depth_texture_);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexStorage2D(GL_TEXTURE_2D, 1, GL_RED, dataset_->DepthHeight(), dataset_->DepthWidth());

  glBindTexture(GL_TEXTURE_2D, 0);

  // Rectangles
  double aspect_rgb = static_cast<double>(dataset_->RgbWidth()) / dataset_->RgbHeight();
  double aspect_depth = static_cast<double>(dataset_->DepthWidth()) / dataset_->DepthHeight();
  double aspect = aspect_rgb + aspect_depth;
  double height = 2. / aspect;

  float color_rectangle_buffer[] = {
    -aspect_rgb / 2.f, 0.f, 0.f, 0.f,
    aspect_rgb / 2.f, 0.f, 1.f, 0.f,
    aspect_rgb / 2.f, 1.f, 1.f, 1.f,
    -aspect_rgb / 2.f, 1.f, 0.f, 1.f,
  };

  float depth_rectangle_buffer[] = {
    -aspect_depth / 2.f, -1.f, 0.f, 0.f,
    aspect_depth / 2.f, -1.f, 1.f, 0.f,
    aspect_depth / 2.f, 0.f, 1.f, 1.f,
    -aspect_depth / 2.f, 0.f, 0.f, 1.f,
  };

  /*
  float color_rectangle_buffer[] = {
    -1.f, -height / 2.f, 0.f, 0.f,
    -1.f + aspect_rgb / aspect * 2.f, -height / 2.f, 1.f, 0.f,
    -1.f + aspect_rgb / aspect * 2.f, height / 2.f, 1.f, 1.f,
    -1.f, height / 2.f, 0.f, 1.f,
  };

  float depth_rectangle_buffer[] = {
    1.f - aspect_depth / aspect * 2.f, -height / 2.f, 0.f, 0.f,
    1.f, -height / 2.f, 1.f, 0.f,
    1.f, height / 2.f, 1.f, 1.f,
    1.f - aspect_depth / aspect * 2.f, height / 2.f, 0.f, 1.f,
  };
  */

  int rectangle_index_buffer[] = {
    0, 1, 2, 3
  };

  glGenVertexArrays(1, &color_rectangle_vao_);
  glBindVertexArray(color_rectangle_vao_);

  glGenBuffers(1, &color_rectangle_vbo_);
  glBindBuffer(GL_ARRAY_BUFFER, color_rectangle_vbo_);
  glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(float), color_rectangle_buffer, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(0));
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glGenBuffers(1, &rectangle_ibo_);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rectangle_ibo_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(int), rectangle_index_buffer, GL_STATIC_DRAW);

  glGenVertexArrays(1, &depth_rectangle_vao_);
  glBindVertexArray(depth_rectangle_vao_);

  glGenBuffers(1, &depth_rectangle_vbo_);
  glBindBuffer(GL_ARRAY_BUFFER, depth_rectangle_vbo_);
  glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(float), depth_rectangle_buffer, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(0));
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rectangle_ibo_);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Engine::Draw()
{
  glClear(GL_COLOR_BUFFER_BIT);

  // Texture
  auto rgb_image = dataset_->GetRgbImage();
  glBindTexture(GL_TEXTURE_2D, color_texture_);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, dataset_->RgbWidth(), dataset_->RgbHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, &rgb_image[0]);

  shader_color_.Use();
  glBindVertexArray(color_rectangle_vao_);
  glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, 0);
  glBindTexture(GL_TEXTURE_2D, 0);
  glBindVertexArray(0);

  // Depth image
  auto depth_image = dataset_->GetDepthImage();
  glBindTexture(GL_TEXTURE_2D, depth_texture_);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, dataset_->DepthHeight(), dataset_->DepthWidth(), 0, GL_RED, GL_UNSIGNED_SHORT, &depth_image[0]);

  shader_depth_.Use();
  glBindVertexArray(depth_rectangle_vao_);
  glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, 0);
  glBindTexture(GL_TEXTURE_2D, 0);
  glBindVertexArray(0);
}

void Engine::LoadShaders()
{
  shader_color_ = Program("..\\src\\shader\\texture");
  shader_depth_ = Program("..\\src\\shader\\depth");
}
}

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
  : width_(800), height_(600)
{
}

Engine::~Engine()
{
  glDeleteBuffers(1, &rectangle_vbo_);
  glDeleteBuffers(1, &rectangle_ibo_);
  glDeleteVertexArrays(1, &rectangle_vao_);
  glDeleteTextures(1, &texture_);
}

void Engine::Run()
{
  // GLFW window
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window_ = glfwCreateWindow(width_, height_, "LearnOpenGL", NULL, NULL);
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

  while (!glfwWindowShouldClose(window_))
  {
    if (animation_)
    {
      double current_time = glfwGetTime();
      auto current_frame = static_cast<int>(animation_time_ * 60.);
      animation_time_ += current_time - animation_absolute_time_;
      animation_absolute_time_ = current_time;
      auto next_frame = static_cast<int>(animation_time_ * 60.);

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

  // Rectangles
  float rectangle_buffer[] = {
    -0.5f, -0.5f, 0.f, 0.f,
    0.5f, -0.5f, 1.f, 0.f,
    0.5f, 0.5f, 1.f, 1.f,
    -0.5f, 0.5f, 0.f, 1.f,
  };

  int rectangle_index_buffer[] = {
    0, 1, 2, 3
  };

  glGenVertexArrays(1, &rectangle_vao_);
  glBindVertexArray(rectangle_vao_);

  glGenBuffers(1, &rectangle_vbo_);
  glBindBuffer(GL_ARRAY_BUFFER, rectangle_vbo_);
  glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(float), rectangle_buffer, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(0));
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glGenBuffers(1, &rectangle_ibo_);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rectangle_ibo_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(int), rectangle_index_buffer, GL_STATIC_DRAW);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  // Dataset
  dataset_ = &wnp_;
  //dataset_ = &utkinect_;

  dataset_->SelectSequence(0);

  glGenTextures(1, &texture_);
  glBindTexture(GL_TEXTURE_2D, texture_);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB, dataset_->RgbWidth(), dataset_->RgbHeight());
  glBindTexture(GL_TEXTURE_2D, 0);
}

void Engine::Draw()
{
  glClear(GL_COLOR_BUFFER_BIT);

  // Texture
  auto rgb_image = dataset_->GetRgbImage();
  glBindTexture(GL_TEXTURE_2D, texture_);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, dataset_->RgbWidth(), dataset_->RgbHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, &rgb_image[0]);

  // Depth image
  auto depth_image = dataset_->GetDepthImage();

  for (int i = 0; i < 10; i++)
  {
    for (int j = 0; j < 10; j++)
    {
      std::cout << depth_image[i + j * dataset_->DepthHeight()] << " ";
    }
    std::cout << std::endl;
  }

  shader_color_.Use();
  glBindVertexArray(rectangle_vao_);
  glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, 0);
  glBindTexture(GL_TEXTURE_2D, 0);
  glBindVertexArray(0);
}

void Engine::LoadShaders()
{
  shader_color_ = Program("..\\src\\shader\\texture");
}
}

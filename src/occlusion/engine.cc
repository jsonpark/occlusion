#include "occlusion/engine.h"

#include <iostream>

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
}

Engine::Engine()
  : width_(800), height_(600)
{
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

  glfwMakeContextCurrent(window_);

  // GLAD
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cerr << "Failed to initialize GLAD" << std::endl;
    return;
  }

  glfwSetWindowUserPointer(window_, this);
  glfwSetFramebufferSizeCallback(window_, FramebufferSizeCallback);

  Resize(width_, height_);

  // Rendering loop
  Initialize();

  while (!glfwWindowShouldClose(window_))
  {
    glClear(GL_COLOR_BUFFER_BIT);

    Draw();

    glfwSwapBuffers(window_);
    glfwPollEvents();
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

void Engine::Initialize()
{
  glClearColor(0.8f, 0.8f, 0.8f, 1.0f);

  LoadShaders();

  // Rectangles
  rectangle_buffer_[0] = -0.5f;
  rectangle_buffer_[1] = -0.5f;
  rectangle_buffer_[2] = 0.f;
  rectangle_buffer_[3] = 0.f;

  rectangle_buffer_[4] = 0.5f;
  rectangle_buffer_[5] = -0.5f;
  rectangle_buffer_[6] = 1.f;
  rectangle_buffer_[7] = 0.f;

  rectangle_buffer_[8] = 0.5f;
  rectangle_buffer_[9] = 0.5f;
  rectangle_buffer_[10] = 1.f;
  rectangle_buffer_[11] = 1.f;

  rectangle_buffer_[12] = -0.5f;
  rectangle_buffer_[13] = 0.5f;
  rectangle_buffer_[14] = 0.f;
  rectangle_buffer_[15] = 1.f;
  rectangle_buffer_.Update();

  rectangle_index_buffer_[0] = 0;
  rectangle_index_buffer_[1] = 1;
  rectangle_index_buffer_[2] = 2;
  rectangle_index_buffer_[3] = 3;
  rectangle_index_buffer_.Update();

  rectangle_.VertexAttribPointer(rectangle_buffer_, 0, 2, 4, 0);
  rectangle_.VertexAttribPointer(rectangle_buffer_, 1, 2, 4, 2);
  rectangle_.SetupDrawElements(rectangle_index_buffer_, VertexArray::DrawMode::TriangleFan, 0, 4);

  // Texture
  texture_.Load("..\\data\\utkinect\\RGB\\s01_e01\\colorImg190.jpg");
}

void Engine::Draw()
{
  shader_color_.Use();
  texture_.Bind();
  rectangle_.Draw();
  texture_.Unbind();
}

void Engine::LoadShaders()
{
  shader_color_ = Program("..\\src\\shader\\texture");
}
}

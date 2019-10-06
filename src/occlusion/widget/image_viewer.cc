#include "occlusion/widget/image_viewer.h"

#include "occlusion/engine.h"

namespace occlusion
{
ImageViewer::ImageViewer(Engine* engine)
  : Widget(engine)
{
}

ImageViewer::ImageViewer(Engine* engine, int x, int y, int width, int height)
  : Widget(engine, x, y, width, height)
{
}

ImageViewer::~ImageViewer()
{
  if (image_initialized_)
  {
    glDeleteVertexArrays(1, &vao_);
    glDeleteBuffers(1, &vbo_);
    glDeleteBuffers(1, &ibo_);
    glDeleteTextures(1, &texture_);
  }
}

void ImageViewer::GenerateImageBuffer()
{
  image_initialized_ = true;

  glGenVertexArrays(1, &vao_);
  glBindVertexArray(vao_);

  glGenBuffers(1, &vbo_);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 16, 0, GL_STATIC_DRAW);

  int rectangle_index_buffer[] = {
    0, 1, 2, 3
  };

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(0));
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glGenBuffers(1, &ibo_);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * 4, rectangle_index_buffer, GL_STATIC_DRAW);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void ImageViewer::InitializeTexture(ImageType type, int width, int height, bool flipped)
{
  image_width_ = width;
  image_height_ = height;

  float aspect = static_cast<float>(width) / height;

  float buffer1[] =
  {
    -aspect, -1.f, 0.f, 0.f,
    aspect, -1.f, 1.f, 0.f,
    aspect, 1.f, 1.f, 1.f,
    -aspect, 1.f, 0.f, 1.f,
  };

  float buffer2[] =
  {
    -1.f, -1.f / aspect, 0.f, 0.f,
    1.f, -1.f / aspect, 1.f, 0.f,
    1.f, 1.f / aspect, 1.f, 1.f,
    -1.f, 1.f / aspect, 0.f, 1.f,
  };

  glBindBuffer(GL_ARRAY_BUFFER, vbo_);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 16, buffer2);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  if (texture_ != 0)
    glDeleteTextures(1, &texture_);

  glGenTextures(1, &texture_);
  glBindTexture(GL_TEXTURE_2D, texture_);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  GLenum enum_components = type == ImageType::Color ? GL_RGB : GL_RED;

  if (!flipped)
    glTexStorage2D(GL_TEXTURE_2D, 1, enum_components, width, height);
  else
    glTexStorage2D(GL_TEXTURE_2D, 1, enum_components, height, width);

  glBindTexture(GL_TEXTURE_2D, 0);
}

void ImageViewer::SetColorImage(int width, int height, const std::vector<unsigned char>& image)
{
  image_type_ = ImageType::Color;

  if (!image_initialized_)
    GenerateImageBuffer();

  if (image_width_ != width || image_height_ != height)
    InitializeTexture(ImageType::Color, width, height);

  glBindTexture(GL_TEXTURE_2D, texture_);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, &image[0]);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void ImageViewer::SetGrayImage(int width, int height, const std::vector<unsigned short>& image, bool flipped)
{
  image_type_ = ImageType::Gray;

  if (!image_initialized_)
    GenerateImageBuffer();

  if (image_width_ != width || image_height_ != height)
    InitializeTexture(ImageType::Gray, width, height, flipped);

  glBindTexture(GL_TEXTURE_2D, texture_);

  if (!flipped)
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_SHORT, &image[0]);
  else
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, height, width, 0, GL_RED, GL_UNSIGNED_SHORT, &image[0]);

  glBindTexture(GL_TEXTURE_2D, 0);
}

void ImageViewer::Draw()
{
  if (image_initialized_)
  {
    SetViewport();

    switch (image_type_)
    {
    case ImageType::Color:
      GetEngine()->UseColorImageShader();
      break;

    case ImageType::Gray:
      GetEngine()->UseGrayImageShader();
      break;
    }

    glBindTexture(GL_TEXTURE_2D, texture_);
    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
  }
}
}

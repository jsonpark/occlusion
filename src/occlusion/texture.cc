#include "occlusion/texture.h"

#include <stdexcept>

#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

namespace occlusion
{
Texture::Texture()
{
}

Texture::~Texture()
{
}

void Texture::Load(const std::string& filename)
{
  stbi_set_flip_vertically_on_load(true);
  unsigned char* data = stbi_load(filename.c_str(), &width_, &height_, &components_, 0);
  image_ = std::vector<unsigned char>(data, data + static_cast<std::uint64_t>(width_) * height_ * components_);
  stbi_image_free(data);
}

void Texture::Bind()
{
  if (!generated_)
    Generate();

  glBindTexture(GL_TEXTURE_2D, id_);
}

void Texture::Unbind()
{
  glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Generate()
{
  if (generated_)
    return;

  generated_ = true;

  glGenTextures(1, &id_);
  glBindTexture(GL_TEXTURE_2D, id_);

  // Set the texture wrapping/filtering options (on the currently bound texture object)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // Load and generate the texture
  if (components_ == 3)
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width_, height_, 0, GL_RGB, GL_UNSIGNED_BYTE, &image_[0]);
  else if (components_ == 4)
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image_[0]);
  glGenerateMipmap(GL_TEXTURE_2D);

  glBindTexture(GL_TEXTURE_2D, 0);
}
}

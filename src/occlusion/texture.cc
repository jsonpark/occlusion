#include "occlusion/texture.h"

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

void Texture::Load(int width, int height, std::vector<unsigned char>&& image)
{
  width_ = width;
  height_ = height;
  components_ = image.size() / width / height;
  image_ = std::move(image);
}
}

#ifndef OCCLUSION_TEXTURE_OBJECT_H_
#define OCCLUSION_TEXTURE_OBJECT_H_

#include "occlusion/texture.h"

#include <glad/glad.h>

namespace occlusion
{
class TextureObject
{
public:
  TextureObject();
  TextureObject(const std::string& filename);
  ~TextureObject();

  TextureObject(const TextureObject& rhs) = delete;
  TextureObject& operator = (const TextureObject& rhs) = delete;

  TextureObject(TextureObject&& rhs) noexcept
  {
    id_ = rhs.id_;
    texture_ = std::move(rhs.texture_);

    rhs.id_ = 0;
  }

  TextureObject& operator = (TextureObject&& rhs) noexcept
  {
    id_ = rhs.id_;
    texture_ = std::move(rhs.texture_);

    rhs.id_ = 0;
  }

  void Load(const std::string& filename);

  auto GetTextureId() const
  {
    return id_;
  }

  void Clear();

private:
  // GL variables
  void PrepareGlResource();

  GLuint id_ = 0;

  Texture texture_;
};
}

#endif // OCCLUSION_TEXTURE_OBJECT_H_

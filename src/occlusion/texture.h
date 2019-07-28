#ifndef OCCLUSION_TEXTURE_H_
#define OCCLUSION_TEXTURE_H_

#include "occlusion/buffer.h"

namespace occlusion
{
class Texture
{
public:
  Texture();
  ~Texture();

  void Load(const std::string& filename);

  void Bind();
  void Unbind();

private:
  void Generate();

  bool generated_ = false;

  int width_;
  int height_;
  int components_;
  std::vector<unsigned char> image_;

  // GL variables
  GLuint id_ = 0;
};
}

#endif // OCCLUSION_TEXTURE_H_

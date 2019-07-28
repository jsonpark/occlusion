#ifndef OCCLUSION_TEXTURE_H_
#define OCCLUSION_TEXTURE_H_

#include <string>
#include <vector>

namespace occlusion
{
class Texture
{
public:
  Texture();
  ~Texture();

  void Load(const std::string& filename);
  void Load(int width, int height, std::vector<unsigned char>&& image);

private:
  int width_;
  int height_;
  int components_;
  std::vector<unsigned char> image_;
};
}

#endif // OCCLUSION_TEXTURE_H_

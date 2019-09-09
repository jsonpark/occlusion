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
  Texture(const std::string& filename);
  ~Texture();

  void Load(const std::string& filename);
  void Load(int width, int height, std::vector<unsigned char>&& image);

  void Clear();

  auto Width() const
  {
    return width_;
  }

  auto Height() const
  {
    return height_;
  }

  auto Components() const
  {
    return components_;
  }

  const auto& Image() const
  {
    return image_;
  }

private:
  int width_ = 0;
  int height_ = 0;
  int components_ = 0;
  std::vector<unsigned char> image_;
};
}

#endif // OCCLUSION_TEXTURE_H_

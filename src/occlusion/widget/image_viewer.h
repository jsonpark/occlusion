#ifndef OCCLUSION_WIDGET_IMAGE_VIEWER_H_
#define OCCLUSION_WIDGET_IMAGE_VIEWER_H_

#include "occlusion/widget/widget.h"

#include <vector>

#include <glad/glad.h>

namespace occlusion
{
class ImageViewer : public Widget
{
private:
  enum class ImageType
  {
    Color,
    Gray,
  };

public:
  ImageViewer(Engine* engine);
  ImageViewer(Engine* engine, int x, int y, int width, int height);
  ~ImageViewer();

  void SetColorImage(int width, int height, const std::vector<unsigned char>& image);
  void SetGrayImage(int width, int height, const std::vector<unsigned short>& image, bool flipped = false);

  void Draw() override;

private:
  bool image_initialized_ = false;
  ImageType image_type_ = ImageType::Color;

  int image_width_;
  int image_height_;

  void GenerateImageBuffer();
  void InitializeTexture(ImageType type, int width, int height, bool flipped = false);

  // GL resources
  GLuint vao_;
  GLuint vbo_;
  GLuint ibo_;
  GLuint texture_;
};
}

#endif // OCCLUSION_WIDGET_IMAGE_VIEWER_H_

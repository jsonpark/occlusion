#ifndef OCCLUSION_WIDGET_CAMERA_VIEWER_H_
#define OCCLUSION_WIDGET_CAMERA_VIEWER_H_

#include "occlusion/widget/widget.h"

#include "occlusion/camera.h"

namespace occlusion
{
class CameraViewer : public Widget
{
public:
  CameraViewer(Engine* engine);
  CameraViewer(Engine* engine, int x, int y, int width, int height);
  ~CameraViewer();

  void Draw() override;

private:
  Camera camera_;
};
}

#endif // OCCLUSION_WIDGET_CAMERA_VIEWER_H_

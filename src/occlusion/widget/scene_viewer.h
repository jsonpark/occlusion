#ifndef OCCLUSION_WIDGET_SCENE_VIEWER_H_
#define OCCLUSION_WIDGET_SCENE_VIEWER_H_

#include "occlusion/widget/widget.h"

#include "occlusion/scene/scene.h"
#include "occlusion/scene/scene_camera.h"

namespace occlusion
{
class SceneViewer : public Widget
{
public:
  SceneViewer(Engine* engine);
  SceneViewer(Engine* engine, int x, int y, int width, int height);
  ~SceneViewer();

  void SetCamera(int camera_index)
  {
    camera_index_ = camera_index;
  }

  void Draw() override;

private:
  std::shared_ptr<Scene> scene_;

  // Camera
  int camera_index_ = 0;
  std::vector<std::shared_ptr<SceneCamera>> camera_nodes_;
};
}

#endif // OCCLUSION_WIDGET_SCENE_VIEWER_H_

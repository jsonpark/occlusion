#ifndef OCCLUSION_SCENE_SCENE_H_
#define OCCLUSION_SCENE_SCENE_H_

#include "occlusion/scene/scene_node.h"
#include "occlusion/data/light.h"

namespace occlusion
{
class Scene
{
public:
  Scene();
  ~Scene();

  auto GetRoot() const
  {
    return root_;
  }

  void AddLight(const Light& light)
  {
    lights_.push_back(light);
  }

  const auto& GetLights() const
  {
    return lights_;
  }

private:
  std::shared_ptr<SceneNode> root_;

  std::vector<Light> lights_;
};
}

#endif // OCCLUSION_SCENE_SCENE_H_

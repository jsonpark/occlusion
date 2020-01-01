#ifndef OCCLUSION_SCENE_SCENE_EDGE_H_
#define OCCLUSION_SCENE_SCENE_EDGE_H_

#include <memory>

#include "occlusion/types.h"

namespace occlusion
{
class SceneNode;

class SceneEdge
{
public:
  static std::shared_ptr<SceneEdge> Create(std::shared_ptr<SceneNode> parent, std::shared_ptr<SceneNode> child, const Affine3d& transform = Affine3d::Identity());

public:
  SceneEdge() = delete;
  SceneEdge(std::shared_ptr<SceneNode> parent, std::shared_ptr<SceneNode> child, const Affine3d& transform = Affine3d::Identity());
  ~SceneEdge();

  const auto& GetTransform() const
  {
    return transform_;
  }

  auto Parent() const
  {
    return parent_.lock();
  }

  auto Child() const
  {
    return child_;
  }

private:
  std::weak_ptr<SceneNode> parent_;
  std::shared_ptr<SceneNode> child_;

  Affine3d transform_;
};
}

#endif // OCCLUSION_SCENE_SCENE_EDGE_H_

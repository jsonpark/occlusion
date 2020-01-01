#ifndef OCCLUSION_SCENE_SCENE_NODE_H_
#define OCCLUSION_SCENE_SCENE_NODE_H_

#include <memory>
#include <vector>

namespace occlusion
{
class SceneEdge;

class SceneNode
{
public:
  SceneNode();
  ~SceneNode();

  void SetParent(std::shared_ptr<SceneEdge> parent);
  void AddChild(std::shared_ptr<SceneEdge> child);

  auto Parent() const
  {
    return parent_;
  }

  const auto& Children() const
  {
    return children_;
  }

private:
  std::weak_ptr<SceneEdge> parent_;
  std::vector<std::shared_ptr<SceneEdge>> children_;
};
}

#endif // OCCLUSION_SCENE_SCENE_NODE_H_

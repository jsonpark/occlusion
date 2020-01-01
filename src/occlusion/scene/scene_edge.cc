#include "occlusion/scene/scene_edge.h"

#include "occlusion/scene/scene_node.h"

namespace occlusion
{
std::shared_ptr<SceneEdge> SceneEdge::Create(std::shared_ptr<SceneNode> parent, std::shared_ptr<SceneNode> child, const Affine3d& transform)
{
  auto edge = std::make_shared<SceneEdge>(parent, child, transform);
  parent->AddChild(edge);
  child->SetParent(edge);

  return edge;
}

SceneEdge::SceneEdge(std::shared_ptr<SceneNode> parent, std::shared_ptr<SceneNode> child, const Affine3d& transform)
  : parent_(parent), child_(child), transform_(transform)
{
}

SceneEdge::~SceneEdge() = default;
}

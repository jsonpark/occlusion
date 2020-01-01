#include "occlusion/scene/scene_node.h"

namespace occlusion
{
SceneNode::SceneNode() = default;

SceneNode::~SceneNode() = default;

void SceneNode::SetParent(std::shared_ptr<SceneEdge> parent)
{
  parent_ = parent;
}

void SceneNode::AddChild(std::shared_ptr<SceneEdge> child)
{
  children_.push_back(child);
}
}

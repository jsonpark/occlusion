#include "occlusion/scene/scene.h"

namespace occlusion
{
Scene::Scene()
{
  root_ = std::make_shared<SceneNode>();
}

Scene::~Scene() = default;
}

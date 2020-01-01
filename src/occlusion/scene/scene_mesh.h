#ifndef OCCLUSION_SCENE_SCENE_MESH_H_
#define OCCLUSION_SCENE_SCENE_MESH_H_

#include "occlusion/scene/scene_node.h"
#include "occlusion/object/mesh_object.h"

namespace occlusion
{
class SceneMesh : public SceneNode
{
public:
  SceneMesh();
  ~SceneMesh();

private:
  std::shared_ptr<MeshObject> mesh_;
};
}

#endif // OCCLUSION_SCENE_SCENE_MESH_H_

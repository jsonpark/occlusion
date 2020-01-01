#ifndef OCCLUSION_SCENE_SCENE_POINT_CLOUD_H_
#define OCCLUSION_SCENE_SCENE_POINT_CLOUD_H_

#include "occlusion/scene/scene_node.h"
#include "occlusion/object/point_cloud_object.h"

namespace occlusion
{
class ScenePointCloud : public SceneNode
{
public:
  ScenePointCloud();
  ~ScenePointCloud();

private:
  std::shared_ptr<PointCloudObject> point_cloud_object_;
};
}

#endif // OCCLUSION_SCENE_SCENE_POINT_CLOUD_H_

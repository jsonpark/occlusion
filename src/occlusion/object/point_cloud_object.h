#ifndef OCCLUSION_OBJECT_POINT_CLOUD_OBJECT_H_
#define OCCLUSION_OBJECT_POINT_CLOUD_OBJECT_H_

#include "occlusion/data/point_cloud.h"

#include <glad/glad.h>

namespace occlusion
{
class PointCloudObject
{
public:
  PointCloudObject() = delete;
  explicit PointCloudObject(int max_num_points);
  ~PointCloudObject();

  PointCloudObject(const PointCloudObject& rhs) = delete;
  PointCloudObject& operator = (const PointCloudObject& rhs) = delete;

  PointCloudObject(PointCloudObject&& rhs) noexcept
  {
    vao_ = rhs.vao_;
    num_points_ = rhs.num_points_;
    point_vbo_ = rhs.point_vbo_;
    color_vbo_ = rhs.color_vbo_;
    max_num_points_ = rhs.max_num_points_;

    rhs.vao_ = 0;
    rhs.num_points_ = 0;
    rhs.point_vbo_ = 0;
    rhs.color_vbo_ = 0;
  }

  PointCloudObject& operator = (PointCloudObject&& rhs) noexcept
  {
    vao_ = rhs.vao_;
    num_points_ = rhs.num_points_;
    point_vbo_ = rhs.point_vbo_;
    color_vbo_ = rhs.color_vbo_;
    max_num_points_ = rhs.max_num_points_;

    rhs.vao_ = 0;
    rhs.num_points_ = 0;
    rhs.point_vbo_ = 0;
    rhs.color_vbo_ = 0;

    return *this;
  }

  void Update(const PointCloud& point_cloud);

  void Draw();

  void Clear();

private:
  int max_num_points_ = 0;

  int num_points_ = 0;

  // GL variables
  void PrepareGlResource();

  GLuint vao_ = 0;
  GLuint point_vbo_ = 0;
  GLuint color_vbo_ = 0;
};
}

#endif // OCCLUSION_OBJECT_POINT_CLOUD_OBJECT_H_

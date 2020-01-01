#include "occlusion/object/point_cloud_object.h"

#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace occlusion
{
PointCloudObject::PointCloudObject(int max_num_points)
  : max_num_points_(max_num_points)
{
}

PointCloudObject::~PointCloudObject()
{
  Clear();
}

void PointCloudObject::Clear()
{
  if (vao_)
  {
    glDeleteVertexArrays(1, &vao_);
    glDeleteBuffers(1, &point_vbo_);
    glDeleteBuffers(1, &color_vbo_);

    vao_ = 0;
    point_vbo_ = 0;
    color_vbo_ = 0;
  }
}

void PointCloudObject::PrepareGlResource()
{
  if (vao_)
    Clear();

  glGenVertexArrays(1, &vao_);
  glBindVertexArray(vao_);

  glGenBuffers(1, &point_vbo_);
  glBindBuffer(GL_ARRAY_BUFFER, point_vbo_);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * max_num_points_ * 3, (void*)0, GL_DYNAMIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
  glEnableVertexAttribArray(0);

  glGenBuffers(1, &color_vbo_);
  glBindBuffer(GL_ARRAY_BUFFER, color_vbo_);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * max_num_points_ * 3, (void*)0, GL_DYNAMIC_DRAW);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
  glEnableVertexAttribArray(1);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void PointCloudObject::Update(const PointCloud& point_cloud)
{
  if (!vao_)
    PrepareGlResource();

  num_points_ = point_cloud.NumPoints();

  const auto& points = point_cloud.GetPointBuffer();
  const auto& colors = point_cloud.GetColorBuffer();

  glBindBuffer(GL_ARRAY_BUFFER, point_vbo_);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * points.size(), points.data());
  glBindBuffer(GL_ARRAY_BUFFER, color_vbo_);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * colors.size(), colors.data());
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void PointCloudObject::Draw()
{
  glBindVertexArray(vao_);
  glDrawArrays(GL_POINTS, 0, num_points_);
  glBindVertexArray(0);
}
}

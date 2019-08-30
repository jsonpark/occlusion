#ifndef OCCLUSION_CAMERA_H_
#define OCCLUSION_CAMERA_H_

#include "occlusion/types.h"

namespace occlusion
{
class Camera
{
private:
  enum class ProjectionType
  {
    ORTHOGONAL,
    PERSPECTIVE,
  };

public:
  Camera();
  ~Camera();

  void Load(const std::string& filename);
  void Save(const std::string& filename);

  void SetPerspective() noexcept
  {
    projection_type_ = ProjectionType::PERSPECTIVE;
  }

  void SetOrthogonal() noexcept
  {
    projection_type_ = ProjectionType::ORTHOGONAL;
  }

  void SetAspect(float aspect) noexcept
  {
    aspect_ = aspect;
  }

  void SetFovy(float fovy) noexcept
  {
    fovy_ = fovy;
  }

  void SetNear(float nearf) noexcept
  {
    near_ = nearf;
  }

  auto GetNear() const noexcept
  {
    return near_;
  }

  void SetFar(float farf) noexcept
  {
    far_ = farf;
  }

  auto GetFar() const noexcept
  {
    return far_;
  }

  void SetEye(float x, float y, float z)
  {
    eye_ = Vector3f(x, y, z);
  }

  void SetCenter(float x, float y, float z)
  {
    center_ = Vector3f(x, y, z);
  }

  void SetUp(float x, float y, float z)
  {
    up_ = Vector3f(x, y, z);
  }

  const auto& GetEye() const noexcept
  {
    return eye_;
  }

  Matrix4f ProjectionMatrix();
  Matrix4f ViewMatrix();

  void Translate(float x, float y);
  void Zoom(float z);
  void Rotate(float x, float y);

private:
  ProjectionType projection_type_;

  Vector3f eye_;
  Vector3f up_;
  Vector3f center_;

  float aspect_;
  float fovy_;
  float near_;
  float far_;

  float width_;
  float height_;

  float translate_sensitivity_ = 0.01f;
  float zoom_sensitivity_ = 0.01f;
  float rotate_sensitivity_ = 0.003f;
};
}

#endif // OCCLUSION_CAMERA_H_

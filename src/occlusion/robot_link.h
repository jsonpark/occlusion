#ifndef OCCLUSION_ROBOT_LINK_H_
#define OCCLUSION_ROBOT_LINK_H_

#include "occlusion/types.h"

namespace occlusion
{
class RobotJoint;

class RobotLink
{
public:
  struct Inertial
  {
    Affine3d origin;
    double mass;
    Matrix3d inertia;
  };

  struct Geometry
  {
    enum class Type
    {
      BOX,
      CYLINDER,
      SPHERE,
      MESH,
      UNDEFINED,
    };

    Type type = Type::UNDEFINED;

    union
    {
      struct
      {
        double x, y, z;
      };
      struct
      {
        double radius;
        double length;
      };
      double scale;
    } size;

    std::string mesh_filename;
  };

  struct Visual
  {
    Affine3d origin;
    Geometry geometry;

    bool has_color;
    Vector4d color;

    bool has_texture;
    std::string texture_filename;
  };

  struct Collision
  {
    Affine3d origin;
    Geometry geometry;
  };

public:
  RobotLink();
  ~RobotLink();

  RobotLink(const RobotLink& rhs) = default;
  RobotLink& operator = (const RobotLink& rhs) = default;

  RobotLink(RobotLink&& rhs) = default;
  RobotLink& operator = (RobotLink&& rhs) = default;

  void SetName(const std::string& name)
  {
    name_ = name;
  }

  const auto& GetName() const
  {
    return name_;
  }

  void SetInertial(const Inertial& inertial)
  {
    inertial_ = inertial;
  }

  void SetInertial(const Affine3d& origin, double mass, const Matrix3d& inertia)
  {
    inertial_.origin = origin;
    inertial_.mass = mass;
    inertial_.inertia = inertia;
  }

  void AddVisual(const Visual& visual)
  {
    visuals_.push_back(visual);
  }

  void AddCollision(const Collision& collision)
  {
    collisions_.push_back(collision);
  }

  const auto& GetVisuals() const
  {
    return visuals_;
  }

  void AddChildJoint(std::shared_ptr<RobotJoint> child_joint)
  {
    child_joints_.push_back(child_joint);
  }

  const auto& GetChildJoints()
  {
    return child_joints_;
  }

  void SetParentJoint(std::shared_ptr<RobotJoint> parent_joint)
  {
    parent_joint_ = parent_joint;
  }

private:
  std::string name_;
  Inertial inertial_;
  std::vector<Visual> visuals_;
  std::vector<Collision> collisions_;

  std::vector<std::shared_ptr<RobotJoint>> child_joints_;
  std::weak_ptr<RobotJoint> parent_joint_;
};
}

#endif // OCCLUSION_ROBOT_LINK_H_

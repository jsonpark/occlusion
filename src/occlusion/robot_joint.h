#ifndef OCCLUSION_ROBOT_JOINT_H_
#define OCCLUSION_ROBOT_JOINT_H_

#include <string>

#include "occlusion/types.h"

namespace occlusion
{
class RobotLink;

class RobotJoint
{
public:
  enum class JointType
  {
    FIXED,
    PRISMATIC,
    REVOLUTE,
  };

private:
  struct Limit
  {
    double lower = 0.;
    double upper = 0.;
    double effort = 0.;
    double velocity = 0.;
  };

public:
  RobotJoint();
  ~RobotJoint();

  RobotJoint(const RobotJoint& rhs) = default;
  RobotJoint& operator = (const RobotJoint& rhs) = default;

  RobotJoint(RobotJoint&& rhs) = default;
  RobotJoint& operator = (RobotJoint&& rhs) = default;

  bool IsFixed() const
  {
    return type_ == JointType::FIXED;
  }

  bool IsRevolute() const
  {
    return type_ == JointType::REVOLUTE;
  }

  void SetType(JointType type)
  {
    type_ = type;
  }

  void SetType(const std::string& type);

  void SetName(const std::string& name)
  {
    name_ = name;
  }

  const auto& GetName() const
  {
    return name_;
  }

  void SetParentLinkName(const std::string& link)
  {
    parent_link_name_ = link;
  }

  const auto& GetParentLinkName() const noexcept
  {
    return parent_link_name_;
  }

  void SetChildLinkName(const std::string& link)
  {
    child_link_name_ = link;
  }

  const auto& GetChildLinkName() const noexcept
  {
    return child_link_name_;
  }

  auto GetChildLink() const noexcept
  {
    return child_link_;
  }

  void SetAxis(const Vector3d& axis)
  {
    axis_ = axis;
  }

  const auto& GetAxis() const noexcept
  {
    return axis_;
  }

  void SetOrigin(const Affine3d& origin)
  {
    origin_ = origin;
  }

  const auto& GetOrigin() const noexcept
  {
    return origin_;
  }

  void SetLimit(double lower, double upper, double effort, double velocity)
  {
    limit_.lower = lower;
    limit_.upper = upper;
    limit_.effort = effort;
    limit_.velocity = velocity;
  }

  double GetLower() const noexcept
  {
    return limit_.lower;
  }

  double GetUpper() const noexcept
  {
    return limit_.upper;
  }

  double GetEffort() const noexcept
  {
    return limit_.effort;
  }

  double GetVelocityLimit() const noexcept
  {
    return limit_.velocity;
  }

  void SetParentLink(std::shared_ptr<RobotLink> parent_link)
  {
    parent_link_ = parent_link;
  }

  auto GetParentLink() const
  {
    return parent_link_;
  }

  void SetChildLink(std::shared_ptr<RobotLink> child_link)
  {
    child_link_ = child_link;
  }

  Affine3d GetTransform(double joint_value) const;

private:
  JointType type_;
  std::string name_;
  std::string parent_link_name_;
  std::string child_link_name_;

  std::weak_ptr<RobotLink> parent_link_;
  std::shared_ptr<RobotLink> child_link_;

  Affine3d origin_;
  Vector3d axis_;
  Limit limit_;
};
}

#endif // OCCLUSION_ROBOT_JOINT_H_

#include "occlusion/robot_joint.h"

namespace occlusion
{
RobotJoint::RobotJoint()
  : type_(JointType::FIXED)
{
}

RobotJoint::~RobotJoint() = default;

void RobotJoint::SetType(const std::string& type)
{
  if (type == "fixed")
    type_ = JointType::FIXED;

  else if (type == "prismatic")
    type_ = JointType::PRISMATIC;

  else if (type == "revolute" || type == "continuous")
    type_ = JointType::REVOLUTE;
}

Affine3d RobotJoint::GetTransform(double joint_value) const
{
  Affine3d transform = origin_;

  switch (type_)
  {
  case JointType::FIXED:
    break;

  case JointType::PRISMATIC:
    transform.translate(axis_ * joint_value);
    break;

  case JointType::REVOLUTE:
    transform.rotate(Eigen::AngleAxisd(joint_value, axis_));
    break;
  }

  return transform;
}
}

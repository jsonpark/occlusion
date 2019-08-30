#include "occlusion/robot_state.h"

#include <iostream>

namespace occlusion
{
RobotState::RobotState(std::shared_ptr<RobotModel> robot_model)
  : robot_model_(robot_model)
{
  Prepare(robot_model->GetBaseLink());
}

RobotState::~RobotState() = default;

void RobotState::Prepare(std::shared_ptr<RobotLink> link)
{
  int index = paired_link_transforms_.size();
  paired_link_transforms_.push_back(std::make_pair(link, Affine3d::Identity()));
  link_name_index_[link->GetName()] = index;
  link_index_[link] = index;

  for (auto child_joint : link->GetChildJoints())
  {
    auto child_link = child_joint->GetChildLink();
    Prepare(child_link);
  }
}

void RobotState::AddJointNames(const std::string& joint_name)
{
  int index = joint_names_.size();
  joint_name_index_[joint_name] = index;
  joint_names_.push_back(joint_name);
  joint_values_.push_back(0.);
}

void RobotState::SetBaseTransform(const Affine3d& transform)
{
  paired_link_transforms_[0].second = transform;
}

void RobotState::ForwardKinematics()
{
  for (int i = 0; i < paired_link_transforms_.size(); i++)
  {
    auto link = paired_link_transforms_[i].first;
    
    for (auto child_joint : link->GetChildJoints())
    {
      auto child_link = child_joint->GetChildLink();
      auto child_link_index = link_index_[child_link];

      double joint_value = 0.;
      const auto& it = joint_name_index_.find(child_joint->GetName());
      if (it != joint_name_index_.cend())
        joint_value = joint_values_[it->second];

      paired_link_transforms_[child_link_index].second = paired_link_transforms_[i].second * child_joint->GetTransform(joint_value);
    }
  }
}
}

#include "occlusion/robot_model.h"

#include <iostream>

namespace occlusion
{
RobotModel::RobotModel() = default;

RobotModel::~RobotModel() = default;

void RobotModel::CreateTreeModel()
{
  for (int i = 0; i < links_.size(); i++)
    link_name_to_index_[links_[i]->GetName()] = i;

  for (int i = 0; i < joints_.size(); i++)
    joint_name_to_index_[joints_[i]->GetName()] = i;

  const int n = links_.size();
  const int m = joints_.size();
  std::vector<char> has_parent(n, 0);

  link_index_to_joint_indices_.resize(n);
  for (int i = 0; i < n; i++)
    link_index_to_joint_indices_[i].clear();

  for (int i = 0; i < m; i++)
  {
    int child_link_id = link_name_to_index_[joints_[i]->GetChildLinkName()];
    int parent_link_id = link_name_to_index_[joints_[i]->GetParentLinkName()];

    has_parent[child_link_id] = true;

    link_index_to_joint_indices_[parent_link_id].push_back(i);
  }

  for (int i = 0; i < n; i++)
  {
    if (!has_parent[i])
    {
      base_link_index_ = i;
      break;
    }
  }

  base_node_ = links_[base_link_index_];

  CreateTreeModel(base_node_);
}

void RobotModel::CreateTreeModel(std::shared_ptr<RobotLink> link)
{
  const auto& link_name = link->GetName();
  auto link_index = link_name_to_index_.find(link_name)->second;

  const auto& joint_indices = link_index_to_joint_indices_[link_index];
  for (auto joint_index : joint_indices)
  {
    auto joint = joints_[joint_index];
    auto child_link_index = link_name_to_index_[joint->GetChildLinkName()];
    auto child_link = links_[child_link_index];

    link->AddChildJoint(joint);
    child_link->SetParentJoint(joint);
    joint->SetParentLink(link);
    joint->SetChildLink(child_link);

    CreateTreeModel(child_link);
  }
}
}

#ifndef OCCLUSION_ROBOT_MODEL_H_
#define OCCLUSION_ROBOT_MODEL_H_

#include <vector>
#include <unordered_map>

#include "occlusion/robot_link.h"
#include "occlusion/robot_joint.h"

namespace occlusion
{
class RobotModel
{
public:
  RobotModel();
  ~RobotModel();

  void CreateTreeModel();

  void SetName(const std::string& name)
  {
    name_ = name;
  }

  void AddLink(const RobotLink& link)
  {
    links_.push_back(std::make_shared<RobotLink>(link));
  }

  void AddJoint(const RobotJoint& joint)
  {
    joints_.push_back(std::make_shared<RobotJoint>(joint));
  }

  const auto& GetLinks() const noexcept
  {
    return links_;
  }

  const auto& GetJoints() const noexcept
  {
    return joints_;
  }

  auto GetLink(const std::string& name) const
  {
    auto it = link_name_to_index_.find(name);
    return links_[it->second];
  }

  auto GetJoint(const std::string& name) const
  {
    auto it = joint_name_to_index_.find(name);
    return joints_[it->second];
  }

  auto GetBaseLinkIndex() const
  {
    return base_link_index_;
  }

  const auto& GetBaseLink() const
  {
    return links_[base_link_index_];
  }

  auto GetBaseLinkNode() const
  {
    return base_node_;
  }

private:
  std::string name_;

  std::vector<std::shared_ptr<RobotLink>> links_;
  std::vector<std::shared_ptr<RobotJoint>> joints_;

  std::unordered_map<std::string, int> link_name_to_index_;
  std::unordered_map<std::string, int> joint_name_to_index_;

  int base_link_index_;

  void CreateTreeModel(std::shared_ptr<RobotLink> node);

  std::shared_ptr<RobotLink> base_node_;

  // Temporary variables for creating tree model
  std::vector<std::vector<int>> link_index_to_joint_indices_;
};
}

#endif // OCCLUSION_ROBOT_MODEL_H_

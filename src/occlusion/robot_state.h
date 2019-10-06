#ifndef OCCLUSION_ROBOT_STATE_H_
#define OCCLUSION_ROBOT_STATE_H_

#include "occlusion/robot_model.h"
#include "occlusion/types.h"

namespace occlusion
{
class RobotState
{
public:
  RobotState() = delete;

  explicit RobotState(std::shared_ptr<RobotModel> robot_model);

  ~RobotState();

  void AddJointName(const std::string& joint_name);
  void SetJointValue(const std::string& joint_name, double joint_value);

  void SetBaseTransform(const Affine3d& transform);

  void ForwardKinematics();

  auto GetLinkTransforms() const
  {
    return paired_link_transforms_;
  }

  const auto& GetLinkTransform(const std::string& link_name) const
  {
    return paired_link_transforms_[link_name_index_.find(link_name)->second].second;
  }

private:
  std::shared_ptr<RobotModel> robot_model_;

  void Prepare(std::shared_ptr<RobotLink> link);

  std::unordered_map<std::string, int> joint_name_index_;
  std::vector<std::string> joint_names_;
  std::vector<double> joint_values_;

  std::unordered_map<std::string, int> link_name_index_;
  std::unordered_map<std::shared_ptr<RobotLink>, int> link_index_;
  std::vector<std::pair<std::shared_ptr<RobotLink>, Affine3d>> paired_link_transforms_;
};
}

#endif // OCCLUSION_ROBOT_STATE_H_

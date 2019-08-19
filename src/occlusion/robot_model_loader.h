#ifndef OCCLUSION_ROBOT_MODEL_LOADER_H_
#define OCCLUSION_ROBOT_MODEL_LOADER_H_

#include <string>
#include <vector>

namespace occlusion
{
class RobotModelLoader
{
public:
  RobotModelLoader();
  ~RobotModelLoader();

  void SetPackageDirectory(const std::string& package_directory);
  void Load(const std::string& filename);

private:
  std::string package_directory_;
};
}

#endif // OCCLUSION_ROBOT_MODEL_LOADER_H_

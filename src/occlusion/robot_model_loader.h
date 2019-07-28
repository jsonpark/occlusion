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

  void Load(const std::string& filename);

private:
};
}

#endif // OCCLUSION_ROBOT_MODEL_LOADER_H_

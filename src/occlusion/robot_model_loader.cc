#include "occlusion/robot_model_loader.h"

#include <tinyxml2/tinyxml2.h>

namespace occlusion
{
RobotModelLoader::RobotModelLoader() = default;

RobotModelLoader::~RobotModelLoader() = default;

void RobotModelLoader::SetPackageDirectory(const std::string& package_directory)
{
  package_directory_ = package_directory;
}

void RobotModelLoader::Load(const std::string& filename)
{
  // TODO
}
}

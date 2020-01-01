#ifndef OCCLUSION_SHADER_SHADERS_H_
#define OCCLUSION_SHADER_SHADERS_H_

#include <vector>

#include "occlusion/shader/program.h"
#include "occlusion/types.h"

namespace occlusion
{
class Shaders
{
public:
  Shaders();
  ~Shaders();

  void AddShader(const std::string& name, const std::string& path);
  void AddShader(const std::string& name, Program&& program);

  auto& operator [] (const std::string& name)
  {
    return shaders_.find(name)->second;
  }

  const auto& operator [] (const std::string& name) const
  {
    return shaders_.find(name)->second;
  }

private:
  std::unordered_map<std::string, Program> shaders_;
};
}

#endif // OCCLUSION_SHADER_SHADERS_H_

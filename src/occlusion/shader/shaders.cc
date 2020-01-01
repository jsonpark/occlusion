#include "occlusion/shader/shaders.h"

namespace occlusion
{
Shaders::Shaders() = default;

Shaders::~Shaders() = default;

void Shaders::AddShader(const std::string& name, const std::string& path)
{
  shaders_[name] = Program(path);
}

void Shaders::AddShader(const std::string& name, Program&& program)
{
  shaders_[name] = std::move(program);
}
}

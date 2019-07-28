#ifndef OCCLUSION_PROGRAM_H_
#define OCCLUSION_PROGRAM_H_

#include <vector>

#include "occlusion/shader.h"

#include <glad/glad.h>

namespace occlusion
{
class Program
{
public:
  Program();
  Program(const std::string& shader_name);
  ~Program();

  void PrintShaderCodes();

  void Link();
  void Use();

private:
  std::vector<Shader> shaders_;

  bool linked_ = false;

  // GL variables
  GLuint id_;
};
}

#endif // OCCLUSION_PROGRAM_H_

#ifndef OCCLUSION_PROGRAM_H_
#define OCCLUSION_PROGRAM_H_

#include <vector>

#include "occlusion/shader.h"
#include "occlusion/types.h"

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

  auto Id() const
  {
    return id_;
  }

  void Uniform1i(const std::string& name, int v);
  void Uniform1f(const std::string& name, float v);
  void Uniform2f(const std::string& name, float v0, float v1);
  void Uniform3f(const std::string& name, const Vector3f& v);
  void Uniform4f(const std::string& name, float v0, float v1, float v2, float v3);
  void UniformMatrix3f(const std::string& name, const Matrix3f& m);
  void UniformMatrix4f(const std::string& name, const Matrix4f& m);

private:
  std::vector<Shader> shaders_;

  bool linked_ = false;

  // GL variables
  GLuint id_;
};
}

#endif // OCCLUSION_PROGRAM_H_

#ifndef OCCLUSION_SHADER_PROGRAM_H_
#define OCCLUSION_SHADER_PROGRAM_H_

#include <vector>

#include "occlusion/shader/shader.h"
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

  Program(const Program& rhs) = delete;
  Program& operator = (const Program& rhs) = delete;

  Program(Program&& rhs)
  {
    shaders_ = std::move(rhs.shaders_);
    linked_ = rhs.linked_;
    id_ = rhs.id_;

    rhs.shaders_.clear();
    rhs.linked_ = false;
    rhs.id_ = 0;
  }

  Program& operator = (Program&& rhs)
  {
    shaders_ = std::move(rhs.shaders_);
    linked_ = rhs.linked_;
    id_ = rhs.id_;

    rhs.shaders_.clear();
    rhs.linked_ = false;
    rhs.id_ = 0;

    return *this;
  }

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
  void Uniform3f(const std::string& name, const Vector3f& v);;
  void Uniform3f(const std::string& name, float v0, float v1, float v2);
  void Uniform4f(const std::string& name, float v0, float v1, float v2, float v3);
  void UniformMatrix3f(const std::string& name, const Matrix3f& m);
  void UniformMatrix4f(const std::string& name, const Matrix4f& m);

private:
  std::vector<Shader> shaders_;

  bool linked_ = false;

  // GL variables
  GLuint id_ = 0;
};
}

#endif // OCCLUSION_SHADER_PROGRAM_H_

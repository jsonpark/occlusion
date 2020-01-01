#include "occlusion/shader/program.h"

#include <iostream>

#include <glad/glad.h>

namespace occlusion
{
Program::Program() = default;

Program::Program(const std::string& shader_name)
{
  Shader vertex_shader(shader_name + ".vert");
  if (vertex_shader.IsLoaded())
    shaders_.push_back(vertex_shader);

  Shader fragment_shader(shader_name + ".frag");
  if (fragment_shader.IsLoaded())
    shaders_.push_back(fragment_shader);

  Shader geometry_sahder(shader_name + ".geom");
  if (geometry_sahder.IsLoaded())
    shaders_.push_back(geometry_sahder);
}

Program::~Program()
{
  if (linked_)
    glDeleteProgram(id_);
}

void Program::PrintShaderCodes()
{
  for (int i = 0; i < shaders_.size(); i++)
  {
    const auto& shader = shaders_[i];

    std::cout << "Shdaer " << i << ":" << std::endl;
    shader.PrintCode();
    std::cout << std::endl;
  }
}

void Program::Use()
{
  if (!linked_)
    Link();

  glUseProgram(id_);
}

void Program::Link()
{
  if (linked_)
    return;

  id_ = glCreateProgram();

  linked_ = true;

  std::vector<GLuint> shader_ids;
  for (const auto& shader : shaders_)
  {
    auto shader_type = GL_VERTEX_SHADER;
    switch (shader.GetType())
    {
    case Shader::Type::Vertex:
      shader_type = GL_VERTEX_SHADER;
      break;

    case Shader::Type::Fragment:
      shader_type = GL_FRAGMENT_SHADER;
      break;

    case Shader::Type::Geometry:
      shader_type = GL_GEOMETRY_SHADER;
      break;
    }

    unsigned int shader_id;
    shader_id = glCreateShader(shader_type);

    auto code = shader.GetCode().c_str();
    glShaderSource(shader_id, 1, &code, NULL);
    glCompileShader(shader_id);

    int success;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);

    if (!success)
    {
      char log[512];
      glGetShaderInfoLog(shader_id, 512, NULL, log);
      std::cout << "ERROR: Shader compilation failed\n" << log << std::endl;

      for (auto shader_id : shader_ids)
        glDeleteShader(shader_id);

      return;
    }

    shader_ids.push_back(shader_id);
  }

  for (auto shader_id : shader_ids)
    glAttachShader(id_, shader_id);

  glLinkProgram(id_);

  int success;
  glGetProgramiv(id_, GL_LINK_STATUS, &success);

  if (!success) {
    char log[512];
    glGetProgramInfoLog(id_, 512, NULL, log);
    std::cout << "ERROR: Program link failed\n" << log << std::endl;
  }

  for (auto shader_id : shader_ids)
  {
    glDetachShader(id_, shader_id);
    glDeleteShader(shader_id);
  }
}

void Program::Uniform1i(const std::string& name, int v)
{
  glUniform1i(glGetUniformLocation(id_, name.c_str()), v);
}

void Program::Uniform1f(const std::string& name, float v)
{
  glUniform1f(glGetUniformLocation(id_, name.c_str()), v);
}

void Program::Uniform2f(const std::string& name, float v0, float v1)
{
  glUniform2f(glGetUniformLocation(id_, name.c_str()), v0, v1);
}

void Program::Uniform3f(const std::string& name, const Vector3f& v)
{
  glUniform3fv(glGetUniformLocation(id_, name.c_str()), 1, v.data());
}

void Program::Uniform3f(const std::string& name, float v0, float v1, float v2)
{
  glUniform3f(glGetUniformLocation(id_, name.c_str()), v0, v1, v2);
}

void Program::Uniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
  glUniform4f(glGetUniformLocation(id_, name.c_str()), v0, v1, v2, v3);
}

void Program::UniformMatrix3f(const std::string& name, const Matrix3f& m)
{
  glUniformMatrix3fv(glGetUniformLocation(id_, name.c_str()), 1, GL_FALSE, m.data());
}

void Program::UniformMatrix4f(const std::string& name, const Matrix4f& m)
{
  glUniformMatrix4fv(glGetUniformLocation(id_, name.c_str()), 1, GL_FALSE, m.data());
}
}

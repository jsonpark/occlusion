#include "occlusion/program.h"

#include <iostream>

#include <glad/glad.h>

namespace occlusion
{
Program::Program() = default;

Program::Program(const std::string& shader_name)
{
  shaders_ = { Shader(shader_name + ".vert"), Shader(shader_name + ".frag") };
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
}

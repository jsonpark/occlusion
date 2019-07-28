#include "occlusion/shader.h"

#include <fstream>
#include <sstream>
#include <iostream>

namespace occlusion
{
Shader::Shader(const std::string& filename)
{
  // Infer the type of shader from the extension of file
  auto extension_location = filename.find_last_of('.');
  if (extension_location == std::string::npos)
    throw std::runtime_error("GlShader class cannot infer shader type (vertex, fragment, etc.) without the extension on the filename.");

  if (filename.substr(extension_location) == ".vert")
  {
    type_ = Type::Vertex;
    LoadCodeFromFile(filename);
  }
  else if (filename.substr(extension_location) == ".frag")
  {
    type_ = Type::Fragment;
    LoadCodeFromFile(filename);
  }
  else
    throw std::runtime_error("GlShader class cannot infer shader type (vertex, fragment, etc.) with an unknown extension on the filename.");
}

void Shader::LoadCodeFromFile(const std::string& filename)
{
  std::ifstream fin(filename);
  if (!fin.is_open())
    return;

  std::stringstream ss;
  ss << fin.rdbuf();
  code_ = ss.str();

  fin.close();
}

void Shader::PrintCode() const
{
  std::cout << code_ << std::endl;
}
}

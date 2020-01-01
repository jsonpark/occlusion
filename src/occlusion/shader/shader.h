#ifndef OCCLUSION_SHADER_SHADER_H_
#define OCCLUSION_SHADER_SHADER_H_

#include <string>

namespace occlusion
{
class Shader
{
public:
  enum class Type
  {
    Vertex,
    Fragment,
    Geometry,
  };

public:
  Shader() = delete;
  Shader(const std::string& filename);

  auto GetType() const
  {
    return type_;
  }

  const auto& GetCode() const
  {
    return code_;
  }

  auto IsLoaded() const
  {
    return loaded_;
  }

  void PrintCode() const;

private:
  Type type_;

  void LoadCodeFromFile(const std::string& filename);

  bool loaded_ = false;

  std::string code_;
};
}

#endif // OCCLUSION_SHADER_SHADER_H_

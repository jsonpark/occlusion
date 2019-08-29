#ifndef OCCLUSION_MESH_OBJECT_H_
#define OCCLUSION_MESH_OBJECT_H_

#include "occlusion/types.h"

#include <glad/glad.h>

namespace occlusion
{
class MeshObject
{
public:
  MeshObject();
  MeshObject(const std::string& filename);
  ~MeshObject();

  void Load(const std::string& filename);
  void Draw();

  const auto& GetDiffuseTextureFilename()
  {
    return diffuse_texture_filename_;
  }

  void Clear();

private:
  // GL variables
  void PrepareGlResource();

  GLuint vao_ = 0;
  GLuint vbo_ = 0;
  GLuint ibo_ = 0;

  std::vector<float> vertices_;
  std::vector<float> normals_;
  std::vector<float> tex_coords_;
  std::vector<unsigned int> indices_;

  std::string diffuse_texture_filename_;
};
}

#endif // OCCLUSION_MESH_OBJECT_H_

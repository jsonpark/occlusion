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

  MeshObject(const MeshObject& rhs) = delete;
  MeshObject& operator = (const MeshObject& rhs) = delete;

  MeshObject(MeshObject&& rhs) noexcept
  {
    vao_ = rhs.vao_;
    vbo_ = rhs.vbo_;
    ibo_ = rhs.ibo_;
    vertices_ = std::move(rhs.vertices_);
    normals_ = std::move(rhs.normals_);
    tex_coords_ = std::move(rhs.tex_coords_);
    indices_ = std::move(rhs.indices_);

    rhs.vao_ = 0;
    rhs.vbo_ = 0;
    rhs.ibo_ = 0;
  }

  MeshObject& operator = (MeshObject&& rhs) noexcept
  {
    vao_ = rhs.vao_;
    vbo_ = rhs.vbo_;
    ibo_ = rhs.ibo_;
    vertices_ = std::move(rhs.vertices_);
    normals_ = std::move(rhs.normals_);
    tex_coords_ = std::move(rhs.tex_coords_);
    indices_ = std::move(rhs.indices_);

    rhs.vao_ = 0;
    rhs.vbo_ = 0;
    rhs.ibo_ = 0;
  }

  void Load(const std::string& filename);
  void Draw() const;

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

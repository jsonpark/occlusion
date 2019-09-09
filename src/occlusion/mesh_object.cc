#include "occlusion/mesh_object.h"

#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace occlusion
{
MeshObject::MeshObject() = default;

MeshObject::MeshObject(const std::string& filename)
{
  Load(filename);
}

MeshObject::~MeshObject()
{
  Clear();
}

void MeshObject::Clear()
{
  if (vao_)
  {
    glDeleteVertexArrays(1, &vao_);
    glDeleteBuffers(1, &vbo_);
    glDeleteBuffers(1, &ibo_);

    vao_ = 0;
    vbo_ = 0;
    ibo_ = 0;
  }
}

void MeshObject::Load(const std::string& filename)
{
  Assimp::Importer importer;
  const aiScene* scene = importer.ReadFile(filename.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs);
  if (scene == NULL)
    return;

  aiNode* scene_node = scene->mRootNode;

  // Assume only one mesh is contained in the file
  aiMesh* mesh = scene->mMeshes[0];

  for (int i = 0; i < mesh->mNumVertices; i++)
  {
    vertices_.push_back(mesh->mVertices[i].x);
    vertices_.push_back(mesh->mVertices[i].y);
    vertices_.push_back(mesh->mVertices[i].z);
  }

  if (mesh->mNormals != NULL)
  {
    for (int i = 0; i < mesh->mNumVertices; i++)
    {
      normals_.push_back(mesh->mNormals[i].x);
      normals_.push_back(mesh->mNormals[i].y);
      normals_.push_back(mesh->mNormals[i].z);
    }
  }

  if (mesh->mTextureCoords[0] != NULL)
  {
    for (int i = 0; i < mesh->mNumVertices; i++)
    {
      tex_coords_.push_back(mesh->mTextureCoords[0][i].x);
      tex_coords_.push_back(1.f - mesh->mTextureCoords[0][i].y);
    }
  }

  for (int i = 0; i < mesh->mNumFaces; i++)
  {
    aiFace face = mesh->mFaces[i];
    for (int j = 0; j < face.mNumIndices; j++)
      indices_.push_back(face.mIndices[j]);
  }

  has_diffuse_texture_ = false;
  aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
  int num_diffuse_textures = material->GetTextureCount(aiTextureType_DIFFUSE);
  for (int i = 0; i < num_diffuse_textures; i++)
  {
    aiString str;
    material->GetTexture(aiTextureType_DIFFUSE, i, &str);
    diffuse_texture_filename_ = str.C_Str();

    // Make absolute path
    diffuse_texture_filename_ = filename.substr(0, filename.find_last_of("\\/") + 1) + diffuse_texture_filename_;
    has_diffuse_texture_ = true;
  }

  PrepareGlResource();
}

void MeshObject::PrepareGlResource()
{
  if (vao_)
    Clear();

  glGenVertexArrays(1, &vao_);
  glBindVertexArray(vao_);

  glGenBuffers(1, &vbo_);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (vertices_.size() + normals_.size() + tex_coords_.size()), (void*)0, GL_STATIC_DRAW);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * vertices_.size(), &vertices_[0]);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
  glEnableVertexAttribArray(0);
  glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * vertices_.size(), sizeof(float) * normals_.size(), &normals_[0]);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * vertices_.size()));
  glEnableVertexAttribArray(1);
  if (!tex_coords_.empty())
  {
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * (vertices_.size() + normals_.size()), sizeof(float) * tex_coords_.size(), &tex_coords_[0]);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * (vertices_.size() + normals_.size())));
    glEnableVertexAttribArray(2);
  }

  glGenBuffers(1, &ibo_);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices_.size(), &indices_[0], GL_STATIC_DRAW);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void MeshObject::Draw() const
{
  glBindVertexArray(vao_);
  glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, (void*)0);
  glBindVertexArray(0);
}
}

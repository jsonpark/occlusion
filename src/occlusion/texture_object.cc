#include "occlusion/texture_object.h"

#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace occlusion
{
TextureObject::TextureObject() = default;

TextureObject::TextureObject(const std::string& filename)
{
  Load(filename);
}

TextureObject::~TextureObject()
{
  Clear();
}

void TextureObject::Clear()
{
  if (id_)
  {
    glDeleteTextures(1, &id_);
    id_ = 0;

    texture_.Clear();
  }
}

void TextureObject::Load(const std::string& filename)
{
  texture_.Load(filename);

  PrepareGlResource();
}

void TextureObject::PrepareGlResource()
{
  if (id_)
    Clear();

  glGenTextures(1, &id_);
  glBindTexture(GL_TEXTURE_2D, id_);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  switch (texture_.Components())
  {
  case 3:
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture_.Width(), texture_.Height(), 0, GL_RGB, GL_UNSIGNED_BYTE, &texture_.Image()[0]);
    break;

  case 4:
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture_.Width(), texture_.Height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, &texture_.Image()[0]);
    break;
  }

  glBindTexture(GL_TEXTURE_2D, 0);
}
}

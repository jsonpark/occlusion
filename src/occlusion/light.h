#ifndef OCCLUSION_LIGHT_H_
#define OCCLUSION_LIGHT_H_

#include "occlusion/types.h"

namespace occlusion
{
struct Light
{
  enum class Type
  {
    Directional,
    Point,
  };

  Light()
    : type(Type::Directional),
    position(0.f, 0.f, 0.f), 
    ambient(0.f, 0.f, 0.f),
    diffuse(0.f, 0.f, 0.f),
    specular(0.f, 0.f, 0.f),
    attenuation(0.f, 0.f, 0.f)
  {
  }

  Type type;
  Vector3f position;
  Vector3f ambient;
  Vector3f diffuse;
  Vector3f specular;
  Vector3f attenuation;
};
}

#endif // OCCLUSION_LIGHT_H_

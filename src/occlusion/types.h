#ifndef OCCLUSION_TYPES_H_
#define OCCLUSION_TYPES_H_

#include <vector>

#include <Eigen/Dense>

namespace occlusion
{
using Vector3f = Eigen::Vector3f;
using Vector3d = Eigen::Vector3d;
using Vector4f = Eigen::Vector4f;
using Vector4d = Eigen::Vector4d;

using Matrix3f = Eigen::Matrix3f;
using Matrix3d = Eigen::Matrix3d;
using Matrix4f = Eigen::Matrix4f;
using Matrix4d = Eigen::Matrix4d;

using Affine3f = Eigen::Affine3f;
using Affine3d = Eigen::Affine3d;

using VectorAffine3f = std::vector<Eigen::Affine3f, Eigen::aligned_allocator<Eigen::Affine3f>>;
using VectorAffine3d = std::vector<Eigen::Affine3d, Eigen::aligned_allocator<Eigen::Affine3d>>;
}

#endif // OCCLUSION_TYPES_H_

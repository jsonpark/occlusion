#include "occlusion/kinect_v2.h"

namespace occlusion
{
const KinectV2::CameraParameters KinectV2::color_params_ =
{
  // Intrinsic
  .fx = 1050.60702,
  .fy = 1052.77088,
  .cx = 983.72076,
  .cy = 530.46047,

  // Distortion
  .k1 = 0.02468,
  .k2 = -0.02250,
  .p1 = -0.00338,
  .p2 = 0.00482,
  .k3 = 0.00000,
};

const KinectV2::CameraParameters KinectV2::depth_params_ =
{
  // Intrinsic
  .fx = 365.19599,
  .fy = 365.61839,
  .cx = 266.84507,
  .cy = 199.14226,

  // Distortion
  .k1 = 0.11926,
  .k2 = -0.53343,
  .p1 = -0.00021,
  .p2 = 0.00730,
  .k3 = 0.00000,
};

KinectV2::KinectV2() = default;

KinectV2::~KinectV2() = default;
}

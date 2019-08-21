#ifndef OCCLUSION_KINECT_V2_H_
#define OCCLUSION_KINECT_V2_H_

#include <occlusion/types.h>

namespace occlusion
{
class KinectV2
{
private:
  struct CameraParameters
  {
    // Intrinsic
    double fx;
    double fy;
    double cx;
    double cy;

    // Distortion
    double k1;
    double k2;
    double p1;
    double p2;
    double k3;
  };

  static const CameraParameters color_params_;
  static const CameraParameters depth_params_;
  static const Matrix3d rotation_;
  static const Vector3d translation_;

public:
  KinectV2();
  ~KinectV2();

private:
};
}

#endif // OCCLUSION_KINECT_V2_H_

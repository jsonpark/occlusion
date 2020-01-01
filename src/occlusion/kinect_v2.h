#ifndef OCCLUSION_KINECT_V2_H_
#define OCCLUSION_KINECT_V2_H_

#include "occlusion/types.h"
#include "occlusion/data/point_cloud.h"

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

  static const int color_width_ = 1920;
  static const int color_height_ = 1080;
  static const int depth_width_ = 512;
  static const int depth_height_ = 424;

public:
  KinectV2();
  ~KinectV2();

  void FeedFrame(std::vector<unsigned char>&& color, std::vector<unsigned short>&& depth);
  void GeneratePointCloud();

  const auto& GetPointCloud() const
  {
    return point_cloud_;
  }

private:
  Matrix3d rotation_;
  Vector3d translation_;

  std::vector<unsigned char> color_;
  std::vector<unsigned short> depth_;

  PointCloud point_cloud_;
};
}

#endif // OCCLUSION_KINECT_V2_H_

#include "occlusion/dataset.h"

namespace occlusion
{
Dataset::Dataset() = default;

Dataset::~Dataset() = default;

int Dataset::NumSequences()
{
  return 1;
}

void Dataset::SelectSequence(int idx)
{
}

void Dataset::SelectSequence(const std::string& name)
{
}

int Dataset::RgbWidth()
{
  // Default Kinect v2
  return 1920;
}

int Dataset::RgbHeight()
{
  return 1080;
}

int Dataset::DepthWidth()
{
  return 512;
}

int Dataset::DepthHeight()
{
  return 424;
}

int Dataset::NumFrames()
{
  return 10;
}

const std::vector<unsigned char>& Dataset::GetRgbImage()
{
  rgb_image_.resize(RgbWidth() * RgbHeight() * 3, 0);

  return rgb_image_;
}

const std::vector<unsigned short>& Dataset::GetDepthImage()
{
  depth_image_.resize(DepthWidth() * DepthHeight(), 0);

  return depth_image_;
}

bool Dataset::PreviousSequence()
{
  return false;
}

bool Dataset::NextSequence()
{
  return false;
}

bool Dataset::PreviousFrame()
{
  return false;
}

bool Dataset::NextFrame()
{
  return false;
}
}

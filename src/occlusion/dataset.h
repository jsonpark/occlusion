#ifndef OCCLUSION_DATASET_H_
#define OCCLUSION_DATASET_H_

#include <string>
#include <vector>

namespace occlusion
{
class Dataset
{
public:
  Dataset();
  virtual ~Dataset();

  virtual int NumSequences();
  virtual void SelectSequence(int idx);
  virtual void SelectSequence(const std::string& name);

  virtual int RgbWidth();
  virtual int RgbHeight();
  virtual int DepthWidth();
  virtual int DepthHeight();

  virtual int NumFrames();
  virtual const std::vector<unsigned char>& GetRgbImage();
  virtual const std::vector<unsigned short>& GetDepthImage();

  virtual bool PreviousSequence();
  virtual bool NextSequence();
  virtual bool PreviousFrame();
  virtual bool NextFrame();

private:
  std::vector<unsigned char> rgb_image_;
  std::vector<unsigned short> depth_image_;
};
}

#endif // OCCLUSION_DATASET_H_

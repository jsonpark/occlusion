#include "occlusion/dataset_wnp.h"

#ifdef _WIN32
#include <filesystem>
#else
#include <experimental/filesystem>
#endif

#include <iostream>
#include <algorithm>

#include <stb/stb_image.h>
#include <mat.h>

#ifdef _WIN32
namespace fs = std::filesystem;
#else
namespace fs = std::experimental::filesystem;
#endif

namespace occlusion
{
namespace
{
#ifdef _WIN32
const char directory_character = '\\';
#else
const char directory_character = '/';
#endif

const std::string directory = std::string("..") + directory_character + "data" + directory_character + "watch-n-patch" + directory_character;

// Extract file index XXXX from filename format "XXXX.jpg" and "XXXX.xml"
int ExtractFileIndex(const std::string& filename)
{
  int len = filename.length();

  // Length is 4 (index) + 4 (extension) = 8
  if (len == 8 && 
    (filename.substr(4) == ".jpg" || filename.substr(4) == ".mat"))
    return std::stoi(filename.substr(0, 4));

  return 10000;
}

std::string ZeroPrependedString(int length, int number)
{
  auto s = std::to_string(number);
  if (s.length() < 4)
    return std::string(4 - s.length(), '0') + s;
  return s;
}
}

const std::vector<std::string> Wnp::scene_names_ =
{
  "office",
  "kitchen1",
  "kitchen2",
};

Wnp::Wnp()
{
  // Get sequence names
  for (const auto& scene_name : scene_names_)
  {
    for (auto& it : fs::directory_iterator(directory + scene_name + directory_character))
    {
      const auto& path = it.path();
      sequence_names_.push_back(scene_name + directory_character + path.filename().string());
    }
  }
  std::sort(sequence_names_.begin(), sequence_names_.end());

  SelectSequence(0);
}

Wnp::~Wnp()
{
}

int Wnp::NumSequences()
{
  return sequence_names_.size();
}

void Wnp::SelectSequence(int idx)
{
  if (current_sequence_ == idx)
    return;

  current_sequence_ = idx;
  current_frame_ = 0;
  cached_rgb_image_index_ = -1;
  cached_depth_image_index_ = -1;

  // Count number of images
  auto index_compare = [](const std::string& a, const std::string& b) {
    return ExtractFileIndex(a) < ExtractFileIndex(b);
  };

  rgb_image_indices_.resize(10000);
  std::fill(rgb_image_indices_.begin(), rgb_image_indices_.end(), -1);
  for (auto& it : fs::directory_iterator(directory + sequence_names_[idx] + directory_character + "rgbjpg"))
  {
    const auto& path = it.path();
    const auto& filename = path.filename().string();
    const auto index = ExtractFileIndex(filename);
    rgb_image_indices_[index] = index;
  }

  depth_image_indices_.resize(10000, -1);
  std::fill(depth_image_indices_.begin(), depth_image_indices_.end(), -1);
  for (auto& it : fs::directory_iterator(directory + sequence_names_[idx] + directory_character + "depth"))
  {
    const auto& path = it.path();
    const auto& filename = path.filename().string();
    const auto index = ExtractFileIndex(filename);
    depth_image_indices_[index] = index;
  }

  // Verify rgb/depth filename matches
  int min_frame = 10000;
  int max_frame = 0;
  for (int i = 0; i < 10000; i++)
  {
    if (rgb_image_indices_[i] != -1 || depth_image_indices_[i] != -1)
    {
      if (min_frame > i)
        min_frame = i;
      if (max_frame < i)
        max_frame = i;
    }
    if (rgb_image_indices_[i] == -1 && i > 0)
      rgb_image_indices_[i] = rgb_image_indices_[i - 1];
    if (depth_image_indices_[i] == -1 && i > 0)
      depth_image_indices_[i] = depth_image_indices_[i - 1];
  }

  for (int i = max_frame - 1; i >= min_frame; i--)
  {
    if (rgb_image_indices_[i] == -1)
      rgb_image_indices_[i] = rgb_image_indices_[i + 1];
    if (depth_image_indices_[i] == -1)
      depth_image_indices_[i] = depth_image_indices_[i + 1];
  }

  for (int i = min_frame; i <= max_frame; i++)
  {
    rgb_image_indices_[i - min_frame] = rgb_image_indices_[i];
    depth_image_indices_[i - min_frame] = depth_image_indices_[i];
  }
  num_frames_ = max_frame - min_frame + 1;
}

void Wnp::SelectSequence(const std::string& name)
{
  for (int i = 0; i < sequence_names_.size(); i++)
  {
    if (sequence_names_[i] == name)
    {
      SelectSequence(i);
      return;
    }
  }

  std::cout << "Wnp: could not find sequence name \"" << name << "\"." << std::endl;
}

int Wnp::RgbWidth()
{
  return 1920;
}

int Wnp::RgbHeight()
{
  return 1080;
}

int Wnp::DepthWidth()
{
  return 512;
}

int Wnp::DepthHeight()
{
  return 424;
}

int Wnp::NumFrames()
{
  return num_frames_;
}

const std::vector<unsigned char>& Wnp::GetRgbImage()
{
  if (cached_rgb_image_index_ == rgb_image_indices_[current_frame_])
    return cached_rgb_image_;

  cached_rgb_image_index_ = rgb_image_indices_[current_frame_];

  auto filename = directory + sequence_names_[current_sequence_] + directory_character + "rgbjpg" + directory_character + ZeroPrependedString(4, cached_rgb_image_index_) + ".jpg";
  int width, height, components;
  stbi_set_flip_vertically_on_load(true);
  unsigned char* data = stbi_load(filename.c_str(), &width, &height, &components, 0);
  cached_rgb_image_ = std::vector<unsigned char>(data, data + static_cast<std::uint64_t>(width) * height * components);
  stbi_image_free(data);

  return cached_rgb_image_;
}

const std::vector<unsigned short>& Wnp::GetDepthImage()
{
  if (cached_depth_image_index_ == depth_image_indices_[current_frame_])
    return cached_depth_image_;

  cached_depth_image_index_ = depth_image_indices_[current_frame_];

  // TODO
  auto filename = directory + sequence_names_[current_sequence_] + directory_character + "depth" + directory_character + ZeroPrependedString(4, cached_depth_image_index_) + ".mat";

  MATFile* mat;
  mat = matOpen(filename.c_str(), "r");

  mxArray* arr = matGetVariable(mat, "depth");
  double* ptr = mxGetPr(arr);
  cached_depth_image_ = std::vector<unsigned short>(ptr, ptr + static_cast<std::uint64_t>(DepthWidth()) * DepthHeight());
  mxDestroyArray(arr);

  matClose(mat);

  return cached_depth_image_;
}

bool Wnp::PreviousSequence()
{
  if (current_sequence_ > 0)
  {
    SelectSequence(current_sequence_ - 1);
    return true;
  }

  return false;
}

bool Wnp::NextSequence()
{
  if (current_sequence_ < NumSequences() - 1)
  {
    SelectSequence(current_sequence_ + 1);
    return true;
  }

  return false;
}

bool Wnp::PreviousFrame()
{
  if (current_frame_ > 0)
  {
    current_frame_--;
    return true;
  }

  return false;
}

bool Wnp::NextFrame()
{
  if (current_frame_ < NumFrames() - 1)
  {
    current_frame_++;
    return true;
  }

  return false;
}
}

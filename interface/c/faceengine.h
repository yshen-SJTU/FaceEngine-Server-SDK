#ifndef __FACE_INTERFACE__
#define __FACE_INTERFACE__

#include <cstdlib>
#include <string>
#include <vector>

typedef struct {
  float xmin;
  float ymin;
  float xmax;
  float ymax;
  float confidence;
} FaceLocation_t;

typedef struct {
  FaceLocation_t location;
  std::vector<float> feature;
} FaceInfo_t;

void init_engine(bool is_gpu);

void set_image_resize_args(int resize_flag=-1, int width=0, int height=0, int min_size=80, float min_ratio=0.3);

std::vector<FaceLocation_t> detectFace(std::string path);
std::vector<FaceLocation_t> detectFace(char *path);
std::vector<FaceInfo_t> getFaceFeature(std::string path);
std::vector<FaceInfo_t> getFaceFeature(char *path);

#endif
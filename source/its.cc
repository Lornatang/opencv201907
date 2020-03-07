/**
 * Copyright 2019 DayHR Authors. All Rights Reserved.
 * Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * ==============================================================================
 */

#include "../include/dir.hpp"
#include "../include/dlcv_its.hpp"

using namespace cv;
using namespace std;

int main(int argc, const char *argv[]) {
  const char *base_dir    =     "/home/wwwroot/my_resume/static/images/dlcv";
  const char *raw_image   =     "/home/wwwroot/my_resume/static/images/dlcv/raw_sketch.png";
  const char *image_dir   =     "/home/wwwroot/my_resume/static/images/dlcv/static";
  const char *sketch_path =     "/home/wwwroot/my_resume/static/images/dlcv/static/sketch.png";

  printf("####Staring image sketch style conversion program!####\n\n\n");

  printf("[·]Start automatically detecting if the base directory exists.\n");
  if (__mkdir__(base_dir) != -1)
    printf("[-]Not found base directory, auto create base directory.\n\n");
  else
    printf("[!]Found base directory, not create base directory.\n\n");

  printf("[·]Start automatically detecting if the image directory exists.\n");
  if (__mkdir__(image_dir) != -1)
    printf("[-]Not found image directory, auto create image directory.\n\n");
  else
    printf("[!]Found image directory, not create image directory.\n\n");

  Mat sketch_image = image_to_sketch(raw_image);
  if (!sketch_image.empty()) {
    printf("[-]Image transfer style success!\n");
    imwrite(sketch_path, sketch_image);
  } else {
    perror("[!]Image transfer style failed!\n");
    return -1;
  }
  return 0;
}
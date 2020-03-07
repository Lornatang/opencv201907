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
#include "../include/download.hpp"
#include "../include/dlcv_vc.hpp"

using namespace std;
using namespace cv;

int main(int argc, const char *argv[]) {
  const char *base_dir   =    "/home/Program/daydao/static/images/dlcv";
  const char *video_name =    "/home/Program/daydao/static/images/dlcv/video.mp4";
  const char *video_dir  =    "/home/Program/daydao/static/images/dlcv/video";
  const char *smile_dir  =    "/home/Program/daydao/static/images/dlcv/static";
  const char *smile_path =    "/home/Program/daydao/static/images/dlcv/static/smile.png";

  printf("####Staring get video beautiful cover program!####\n\n\n");

  printf("[·]Start automatically detecting if the base directory exists.\n");
  if (__mkdir__(base_dir) != -1)
    printf("[-]Not found base directory, auto create base directory.\n\n");
  else
    printf("[!]Found base directory, not create base directory.\n\n");

  printf("[·]Start automatically detecting if the video directory exists.\n");
  if (__mkdir__(video_dir) != -1)
    printf("[-]Not found video directory, auto create video directory.\n\n");
  else
    printf("[!]Found video directory, not creat video directory.\n\n");

  printf("[·]Start automatically detecting if the smile directory exists.\n");
  if (__mkdir__(smile_dir) != -1)
    printf("[-]Not found smile directory, auto create smile directory.\n\n");
  else
    printf("[!]Found smile directory, not creat smile directory.\n\n");

  if (video_to_image(video_name, video_dir) == -1) {
    perror("[!]video transfer image error, please check video exists!\n\n");
    return -1;
  }

  // Read the image, convert it into gray image, and then equalize the histogram.
  save_smile(video_dir, smile_path);

  if (__rmdir__(video_dir) == -1) {
    perror("[!]remove video dir error, please check permissions!\n\n");
    return -1;
  }
  return 0;
}
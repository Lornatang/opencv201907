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

#include "log.hpp"
#include "detect.hpp"
#include "dir.hpp"
#include "download.hpp"
#include "process.hpp"

using namespace std;
using namespace cv;

static void help() {
  cout << "\nThis program demonstrates the smile detector.\n"
          "Contains two functions:\n"
          "1.Converting Video Files to Pictures.\n"
          "2.Extract the smiling pictures from the pictures.\n"
          "\nUsage:\n"
          "./smile [--url=<the URL of the incoming video>]\n"
          "\nExample:\n"
          "./videocover "
          "http://vodm2lzexwq.vod.126.net/vodm2lzexwq/"
          "4673d4ae-7078-42c1-affb-494b4ad0e687.mp4?resId="
          "254486990bfa2cd7aa860229db639341_1919639027_0&sign=i9VRqSZZJDUI%"
          "2B3hka7EpMg12silSgknLl2iTUoyqKT8%3D\n\n";
  cout << "\tUsing OpenCV version 4.0.1.\n\n\n";
}

int main(int argc, const char *argv[]) {
  if (argc < 2) {
    help();
    return 0;
  }
  const char *video_name = "./dlcv/cc/video_cover/video.mp4";
  const char *dir_name = "./dlcv/cc/video_cover/video/";
  string smile_name = "./dlcv/cc/video_cover/smile.png";
  if (download(argv[1], video_name) == -1) {
    lprintf(MSG_ERROR, "Download video error.\n");
    lprintf(MSG_INFO, "Return code -1.\n");
    return -1;
  }
  lprintf(MSG_INFO, "Download video successful!\n");

  if (__mkdir__(dir_name) == -1) {
    lprintf(MSG_ERROR, "Create dir error!\n");
    lprintf(MSG_INFO, "Return code -1.\n");
    return -1;
  }
  lprintf(MSG_INFO, "Create dir successful!\n");

  if (video_to_image(video_name, dir_name) == -1) {
    lprintf(MSG_ERROR, "Video file conversion error!\n");
    lprintf(MSG_INFO, "Return code -1.\n");
    return -1;
  }
  lprintf(MSG_INFO, "Video convert image successful!\n");

  // Read the image, convert it into gray image, and then equalize the
  // histogram.
  for (int i = 1; i < 9999; i++) {
    string img_name = dir_name + to_string(i) + suffix_png;

    Mat img = imread(img_name);
    if (img.empty()) break;

    Mat image_gray;
    cvtColor(img, image_gray, COLOR_BGR2GRAY);
    equalizeHist(image_gray, image_gray);

    vector<Rect> smiles = detectSmile(image_gray);

    if (!smiles.empty()) imwrite(smile_name, img);
  }
  lprintf(MSG_INFO, "Smile detector successful!\n");

  if (__rmdir__(dir_name) == -1) {
    lprintf(MSG_ERROR, "Delete temp dir error.\n");
    lprintf(MSG_INFO, "Return code -1.\n");
    return -1;
  }
  lprintf(MSG_INFO, "Delete temp dir successful!\n");

  return 0;
}
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

#include "dlcv/dlcv_vc.hpp"
#include "dlcv/dir.hpp"

using namespace cv;
using namespace std;

const string forward_salash = "/";

/**
 * Divide video file into several consecutive frames.
 * Args:
 *   video_name: Name of video file
 *   dir_name: Video Cut into Picture Preserved Address
 * Returns:
 *   success return 0, else return -1
 * @ author: Changyu Liu
 * @ last modify time: 2019.8.2
 */
int video_to_image(const char *video_name, const char *video_dir) {
  int frame_count = 1;
  int num = 0;
  Mat image;

  VideoCapture cap(video_name);
  if (!cap.isOpened()) return -1;
  string __video_name = video_name;
  string __dir_name = video_dir;
  bool flag = true;
  while (flag) {
    cap.read(image);
    if (image.empty()) flag = false;
    if (frame_count % 12 == 0) {
      num += 1;
      string image_path = video_dir + forward_salash +  to_string(num) + ".png";
      imwrite(image_path, image);
    }
    frame_count += 1;
  }
  cap.release();
  return 0;
}

vector<Rect> detect_smile(Mat &faces) {
  CascadeClassifier smile_cascade;
  vector<Rect> smiles;
  smile_cascade.load(
          "/usr/local/share/opencv4/haarcascades/haarcascade_smile.xml");
  smile_cascade.detectMultiScale(faces, smiles, 1.1, 2, 0 | CASCADE_SCALE_IMAGE,
                                 Size(30, 30));
  return smiles;
}

int save_smile(const char *video_dir, const char *smile_path) {
  string __video_dir = video_dir + forward_salash;
  for (int i = 2; i < 9999; i++) {
    string image_name = to_string(i) + ".png";
    string image_path = __video_dir + image_name;

    Mat image = imread(image_path);
    if (image.empty()) break;
    Mat gray;
    cvtColor(image, gray, COLOR_BGR2GRAY);
    equalizeHist(gray, gray);
    vector<Rect> smiles = detect_smile(gray);

    if (!smiles.empty()) {
      imwrite(smile_path, image);
      break;
    }
    if (__access__(smile_path) == -1)
      cerr << "undetector smile from this video!" << endl;
  }
  return 0;
}
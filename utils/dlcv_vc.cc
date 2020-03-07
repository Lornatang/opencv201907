/**
 * Copyright 2019 DayHR Authors. All Rights Reserved.
 * Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * ==============================================================================
 */

#include "../include/dlcv_vc.hpp"

using namespace cv;
using namespace std;

string forward_salash = "/";

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
  int num = 1;
  Mat image;

  VideoCapture cap(video_name);
  if (!cap.isOpened()) return -1;
  string __video_dir = video_dir + forward_salash;
  string __video_name = video_name;
  bool flag = true;
  while (flag) {
    cap.read(image);
    if (image.empty()) flag = false;
    if (frame_count % 20 == 0) {
      string image_name = to_string(num) + ".png";
      string image_path = __video_dir + image_name;
      imwrite(image_path, image);
      num++;
    }
    frame_count++;
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
  double min_smile_degree, max_smile_degree = 0.;
  Mat image, gray;
  vector<Rect> smiles;
  string __video_dir = video_dir + forward_salash;

  for (int i = 1; i < 60; i++) {
    string image_name = to_string(i) + ".png";
    string image_path = __video_dir + image_name;

    image = imread(image_path);
    if (image.empty()) return -1;

    cvtColor(image, gray, COLOR_BGR2GRAY);
    equalizeHist(gray, gray);
    smiles = detect_smile(gray);

    if (!smiles.empty())
      for (const auto& smile : smiles) {
        min_smile_degree = smile.height + smile.width;
        if (min_smile_degree > max_smile_degree) {
          imwrite(smile_path, image);
          max_smile_degree = min_smile_degree;
        }
        else
          break;
      }
  }
  return 0;
}
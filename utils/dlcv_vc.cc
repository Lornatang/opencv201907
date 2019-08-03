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

using namespace cv;
using namespace std;

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
int video_to_image(const char *video_name, const char *dir_name) {
  int frame_cnt = 0;
  int num = 0;
  Mat img;

  VideoCapture cap(video_name);
  if (!cap.isOpened()) {
    cerr << "open video error! please check video exists!" << endl;
    return -1;
  }

  bool flag = true;
  while (flag) {
    cap.read(img);
    if (img.empty()) flag = false;

    if (frame_cnt % 20 == 0) {
      ++num;
      String img_path = dir_name + to_string(num) + ".png";
      imwrite(img_path, img);
    }
    ++frame_cnt;
  }
  cap.release();
  return 0;
}

vector<Rect> detectSmile(Mat &faces) {
  CascadeClassifier smileCascade;
  vector<Rect> smiles;
  smileCascade.load(
          "/usr/local/share/opencv4/haarcascades/haarcascade_smile.xml");
  smileCascade.detectMultiScale(faces, smiles, 1.1, 2, 0 | CASCADE_SCALE_IMAGE,
                                Size(30, 30));
  return smiles;
}

int saveSmile(const String &videoDir, const String &smilePath) {
  for (int i = 1; i < 9999; i++) {
    String imageName = "/" + to_string(i) + ".png";
    String imagePath = videoDir + imageName;

    Mat image = imread(imagePath);
    if (image.empty()) break;

    Mat imageGray;
    cvtColor(image, imageGray, COLOR_BGR2GRAY);
    equalizeHist(imageGray, imageGray);

    vector<Rect> smiles = detectSmile(imageGray);

    if (!smiles.empty()) imwrite(smilePath, image);
  }
  return 0;
}
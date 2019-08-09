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


#include "dlcv/dlcv_its.hpp"

using namespace cv;
using namespace std;

/**
 * Convert color map to simple pencil style.
 * Args:
 *   fileName: A file stream of input strings.
 *   saveName: Save file path.
 * Returns:
 *   success return 0, else return -1.
 * @author: Changyu Liu.
 * @last modify time: 2019.8.2
 */
Mat imageToSketch(const char *file_name) {
  Mat image = imread(file_name);
  int image_width=image.cols;
  int image_height=image.rows;
  Mat gray,blur;

  cvtColor(image, gray, COLOR_BGR2GRAY);

  addWeighted(gray, -1, NULL, 0, 255, blur);

  GaussianBlur(blur, blur, Size(11, 11), 0);

  Mat img(blur.size(), CV_8UC1);
  for (int y=0; y < image_height; y++)
  {

    auto* P0  = gray.ptr<uchar>(y);
    auto* P1  = blur.ptr<uchar>(y);
    auto* P  = img.ptr<uchar>(y);
    for (int x=0; x < image_width; x++)
    {
      int tmp0=P0[x];
      int tmp1=P1[x];
      P[x] =(uchar) min((tmp0+(tmp0*tmp1)/(256-tmp1)),255);
    }

  }
  return img;
}
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

#ifndef INCLUDE_PROCESS_HPP
#define INCLUDE_PROCESS_HPP

#include "core.hpp"
#include "imgproc.hpp"
#include "highgui.hpp"

#include "download.hpp"

#include <iostream>

using namespace cv;
using namespace std;

/**
 * Convert color map to simple pencil style.
 * Args:
 *   filename: A file stream of input strings.
 * Returns:
 *   success return 0, else return -1.
 * @author: Changyu Liu.
 * @last modify time: 2019.7.30
 * */
int imageToSketch(const String &fileName, const String &saveName) {
  Mat image = imread(fileName, IMREAD_COLOR);
  if (!image.data || image.empty()) {
    lprintf(MSG_ERROR, "Error: image open error!\n");
    return -1;
  }

  Mat gray_image;
  cvtColor(image, gray_image, COLOR_BGR2GRAY);

  Mat newImage;
  // Scales, calculates absolute values, and converts the result to 8-bit.
  addWeighted(gray_image, -1, NULL, 0, 255, newImage);

  // Applies the bilateral filter to an image.
  GaussianBlur(newImage, newImage, Size(11, 11), 0);

  // Blend: color dodge
  Mat dstImage(newImage.size(), CV_8UC1);

  for (int y = 0; y < gray_image.rows; y++) {
    auto *P0 = newImage.ptr<uchar>(y);
    auto *P1 = newImage.ptr<uchar>(y);
    auto *P = dstImage.ptr<uchar>(y);
    for (int x = 0; x < gray_image.cols; x++) {
      int tmp0 = P0[x];
      int tmp1 = P1[x];
      P[x] = (uchar) min((tmp0 + (tmp0 * tmp1) / (256 - tmp1)), 255);
    }
  }
  lprintf(MSG_INFO, "Information: Image convert sketch successful!\n");
  imwrite(saveName, newImage);
  return 0;
}
#endif // INCLUDE_PROCESS_HPP

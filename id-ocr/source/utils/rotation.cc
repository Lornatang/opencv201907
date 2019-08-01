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

#include "../../include/pretreatment.hpp"

using namespace cv;
using namespace std;

/**
 * Convert and intercept an image that has found an Angle of tilt.
 * Args:
 *   filename: Mat image.
 * Returns:
 *   double angle.
 * @example:
 *   image = imread("./lena.png");
 *   cvtColor(image, gray, COLOR_BGR2GRAY);
 *   double angle = compute_skew(gray);
 *   deskew(image, angle);
 */
Mat deskew(const char *filename, double angle) {
  Mat image = imread(filename, 0);
  bitwise_not(image, image);

  vector<cv::Point> points;
  Mat_<uchar>::iterator it = image.begin<uchar>();
  Mat_<uchar>::iterator end = image.end<uchar>();
  for (; it != end; ++it)
    if (*it)
      points.push_back(it.pos());

  RotatedRect box = minAreaRect(Mat(points));

  // rotation
  Mat rot_mat = getRotationMatrix2D(box.center, angle, 1);

  Mat rotated;
  warpAffine(image, rotated, rot_mat, image.size(), INTER_CUBIC);

  // Cropping
  Size box_size = box.size;
  if (box.angle < -45.)
    std::swap(box_size.width, box_size.height);
  Mat cropped;
  getRectSubPix(rotated, box_size, box.center, cropped);
  return cropped;
}
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
 * Calculate image tilt Angle.
 * Args:
 *   filename: Mat image.
 * Returns:
 *   double angle.
 * Example:
 *   image = imread("./lena.png");
 *   cvtColor(image, gray, COLOR_BGR2GRAY);
 *   double angle = compute_skew(gray);
 *   cout << "angle: " << angle << endl;
 */
double compute_skew(const char *filename) {
  Mat image = imread(filename, 0);
  // Binarize
  threshold(image, image, 225, 255, THRESH_BINARY);

  // Invert colors
  bitwise_not(image, image);

  // Now perform our erosion
  Mat element = cv::getStructuringElement(MORPH_RECT, Size(5, 3));
  erode(image, image, element);

  vector<Point> points;
  Mat_<uchar>::iterator it = image.begin<uchar>();
  Mat_<uchar>::iterator end = image.end<uchar>();
  for (; it != end; ++it)
    if (*it)
      points.push_back(it.pos());

  RotatedRect box = minAreaRect(Mat(points));
  double angle = box.angle;
  if (angle < -45.)
    angle += 90.;

  return angle;
}
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

#ifndef OCR_PRETREATMENT_HPP
#define OCR_PRETREATMENT_HPP

#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <vector>

using namespace cv;

/**
 * Calculate image tilt Angle.
 * Args:
 *   filename: Mat image.
 * Returns:
 *   double angle.
 * Example:
 *   image = imread("./lena.png");
 *   double angle = compute_skew(image);
 *   cout << "angle: " << angle << endl;
 */
double compute_skew(const char *filename);

/**
 * Convert and intercept an image that has found an Angle of tilt.
 * Args:
 *   filename: Mat image.
 *   angle: Magnitude of tilt Angle.
 * Returns:
 *   After rotation image.
 * @example:
 *   image = imread("./lena.png");
 *   double angle = compute_skew(image);
 *   deskew(image, angle);
 */
Mat deskew(const char *filename, double angle);


#endif //OCR_PRETREATMENT_HPP

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

#ifndef DLCV_DLCV_OCR_HPP
#define DLCV_DLCV_OCR_HPP

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include "tesseract/baseapi.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <fstream>
#include <cstring>
#include <cstdlib>

using namespace cv;
using namespace std;

/**
 * Convert color images to binary images for processing.
 * Args:
 *   filename: input file name.
 * Returns:
 *   Mat.
 * @author: Changyu Liu.
 * @last modify time: 2019.8.6.
 */
Mat img_to_binary(const String &);

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
double compute_skew(const char *);

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
Mat deskew(const char *, double);

/**
 * detector card face and save it.
 * Args:
 *   filename: input card image.
 *   save_path: face rol save path.
 * Returns:
 *   vector<Rect>.
 * @author: Changyu Liu
 * @last modify time: 2019.8.7
 */
int save_face(const char *, const char *);

/**
 * Sort the four corners
 * @author: Changyu Liu
 * @last modify time: 2019.8.15
 */
bool sort_corners(std::vector<cv::Point2f> &);

/**
 * Calculate the size of the intersection of the edges of the graph.
 * @author: Changyu Liu
 * @last modify time: 2019.8.15
 */
cv::Point2f compute_intersect(cv::Vec4i, cv::Vec4i);

/**
 * Detect if edges are connected
 * @author: Changyu Liu
 * @last modify time: 2019.8.15
 */
bool is_bad_line(int, int);

/**
 * Sort the detected frameworks
 * @author: Changyu Liu
 * @last modify time: 2019.8.15
 */
bool x_sort(const cv::Point2f &, const cv::Point2f &);

/**
 * Sort the detected angles
 * @author: Changyu Liu
 * @last modify time: 2019.8.15
 */
void sort_corners(std::vector<cv::Point2f> &, cv::Point2f);

/**
 * Calculate the size of the correction drawing
 * @author: Changyu Liu
 * @last modify time: 2019.8.15
 */
void cal_dst_size(const std::vector<cv::Point2f> &);

/**
 * correct image ops
 * @author: Changyu Liu
 * @last modify time: 2019.8.15
 */
void adjust(const char *, const char *);

/**
 * ocr.
 * Args:
 *   filename: input file name.
 * Returns:
 *   string.
 * @author: Changyu Liu
 * @last modify time: 2019.8.6
 */
char *image_to_string(const char *);

/**
 * save string data to text.
 * Args:
 *   text: ocr extract text.
 *   filename: save data file path.
 * Returns:
 *   success return 0, else return -1.
 * @author: Changyu Liu
 * @last modify time: 2019.8.6
 */
int save_data(const char *, const char *);

#endif // DLCV_DLCV_OCR_HPP

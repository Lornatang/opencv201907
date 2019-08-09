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

#ifndef DLCV_DLCV_VC_HPP
#define DLCV_DLCV_VC_HPP

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/opencv.hpp"

#include <iostream>
#include <vector>

/**
 * Divide video file into several consecutive frames.
 * Args:
 *   video_name: Name of video file
 *   dir_name: Video Cut into Picture Preserved Address
 * Returns:
 *   success return 0, else return -1
 * @ author: Changyu Liu
 * @ last modify time: 2019.7.30
 */
int video_to_image(const char *, const char *);

/**
 * Check the smiley faces in the picture.
 * Args:
 *   image: Matrix type data.
 * Returns:
 *   success return 0, else return -1
 * @ author: Changyu Liu
 * @ last modify time: 2019.8.9
 */
std::vector<cv::Rect> detect_smile(cv::Mat &);

/**
 * Save the recognized smiley face to the specified directory.
 * Args:
 *   video_dir: Get a smiley face under a bunch of pictures.
 *   image: Matrix type data.
 * Returns:
 *   success return 0, else return -1
 * @ author: Changyu Liu
 * @ last modify time: 2019.8.9
 */
int save_smile(const char *, const char *);

#endif // DLCV_DLCV_VC_HPP

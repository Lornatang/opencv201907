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


#ifndef DLCV_DLCV_ITS_HPP
#define DLCV_DLCV_ITS_HPP

#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"

#include <iostream>

using namespace cv;

/**
 * Convert color map to simple pencil style.
 * Args:
 *   fileName: A file stream of input strings.
 *   saveName: Save file path.
 * Returns:
 *   success return 0, else return -1.
 * @author: Changyu Liu.
 * @last modify time: 2019.8.1
 */
Mat imageToSketch(const char *filename);

#endif // DLCV_DLCV_ITS_HPP

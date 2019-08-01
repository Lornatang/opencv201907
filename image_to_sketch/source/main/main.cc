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

//#include "./utils/dir.cc"
//#include "./utils/download.cc"
//#include "./utils/process.cc"
//#include "./time.hpp"

#include "dir.hpp"
#include "download.hpp"
#include "log.hpp"
#include "process.hpp"

#include <ctime>

using namespace cv;
using namespace std;

const char *BASE_DIR = "./dlcv/cc/image_to_sketch/temp/";
const char *SAVE_DIR = "./dlcv/cc/image_to_sketch/static/";
String rawSuffix = "raw.png";
String newSuffix = "sketch.png";

int main(int argc, const char *argv[]) {
  time_t t;

  if ((__mkdir__(BASE_DIR)) && (__mkdir__(SAVE_DIR)) == -1)
    lprintf(MSG_WARNING, "Warning: folder exists!\n");

  String rawFileName = BASE_DIR + rawSuffix;
  if (download(argv[1], rawFileName.c_str()) == -1) {
    lprintf(MSG_ERROR, "Error: image download error!\n");
  }

  String newFileName = SAVE_DIR + newSuffix;
  if (imageToSketch(rawFileName, newFileName) == -1) {
    lprintf(MSG_ERROR, "Error: image convert sketch error!\n");
    return -1;
  }

  if (__rmdir__(BASE_DIR) == -1) {
    lprintf(MSG_ERROR, "Error: delete folder error!\n");
    return -1;
  }
  return 0;
}
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

#include "dlcv/dir.hpp"
#include "dlcv/download.hpp"
#include "dlcv/dlcv_its.hpp"

using namespace cv;
using namespace std;

static void help() {
  cout << "\nThe program is image transfer sketch style.\n\n"
          "Usage:\n"
          "  its [url]\n"
          "Example:\n"
          "  its http://pic33.nipic.com/20131007/13639685_123501617185_2.jpg\n\n";
}

int main(int argc, const char *argv[]) {
  const char *base_dir =    "/home/wwwroot/my_resume/static/images/dlcv";
  const char *raw_image =   "/home/wwwroot/my_resume/static/images/dlcv/raw_sketch.png";
  const char *image_dir =   "/home/wwwroot/my_resume/static/images/dlcv/static";
  const char *sketch =      "/home/wwwroot/my_resume/static/images/dlcv/static/sketch.png";

  if (__mkdir__(base_dir) != -1)
    cout << "detector base dir not exists, auto create it." << endl;
  else
    cout << "detector base dir exists, not create." << endl;

//  if (download_file(const_cast<char *>(argv[1]), const_cast<char *>(raw_image)) == 2) {
//    cerr << "download file error!" << endl;
//    return -1;
//  }

  if (__mkdir__(image_dir) != -1)
    cout << "detector image dir not exists, auto create it." << endl;
  else
    cout << "detector image dir exists, not create." << endl;

  Mat sketch_image = imageToSketch(raw_image);
  if (!sketch_image.empty()) {
    cout << "image transfer style success!" << endl;
    imwrite(sketch, sketch_image);
  } else {
    cerr << "image transfer style failed!" << endl;
    return -1;
  }
  return 0;
}
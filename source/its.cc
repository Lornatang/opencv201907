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

#include "dlcv/download.hpp"
#include "dlcv/dlcv_its.hpp"

using namespace cv;
using namespace std;

static void help() {
  cout << "\nThe program is image transfer sketch style.\n\n"
          "Usage:\n"
          "  its [url] [save_path] [sketch_path]\n"
          "Example:\n"
          "  its http://pic33.nipic.com/20131007/13639685_123501617185_2.jpg 1.jpg 2.jpg\n\n";
}

int main(int argc, const char *argv[]) {
  if (argc < 4) {
    help();
    return 0;
  }
  char *url = (char *)argv[1];
  char *rawImage = (char *)argv[2];
  char *newImage = (char *)argv[3];
  download_image(url, rawImage);

  Mat image = imread(rawImage, 0);
  Mat sketch;
  imageToSketch(image, sketch);
  imwrite(newImage, sketch);
  return 0;
}
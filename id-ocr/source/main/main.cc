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

#include "../../include/log.hpp"
#include "../../include/format.hpp"
#include "../../include/pretreatment.hpp"

using namespace cv;
using namespace std;

static void help() {
  newLine(1);
  cout << "The program of this program is to rotate and correct the photos "
          "and then extract the information through OCR";
  newLine(1);
  cout << "Example:";
  newLine(1);
  cout << "idocr ../images/rotation1.png";
  newLine(2);
  cout << "OpenCV Version: " << CV_VERSION;
  newLine(1);
}

int main(int argc, const char *argv[]) {
  if (argc < 2) {
    help();
    return -1;
  }

  double angle = compute_skew(argv[1]);
  Mat newImage = deskew(argv[1], angle);
  return 0;
}
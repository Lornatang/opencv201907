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

#include "../../include/dir.hpp"
#include "../../include/download.hpp"
#include "../../include/process.hpp"

using namespace cv;
using namespace std;

static void help() {
  cout << "The program is image transfer sketch style.";
  cout << endl;
  cout << "Example:\n"
          "\tsketch www.xxxx.com raw.png sketch.png";
  cout << endl;
}

int main(int argc, const char *argv[]) {
  if (argc < 4) {
    help();
    return -1;
  }

  download((char *)argv[1], (char *)argv[2]);
  Mat image = imread(argv[2], 0);
  Mat sketch = imageToSketch(argv[2]);
  imwrite(argv[3], sketch);
  return 0;
}
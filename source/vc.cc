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

#include "dlcv/log.hpp"
#include "dlcv/dir.hpp"
#include "dlcv/download.hpp"
#include "dlcv/dlcv_vc.hpp"

using namespace std;
using namespace cv;

static void help() {
  cout << "\nThe program is extract video beautiful cover.\n\n"
          "Usage:\n"
          "  utils [url] [video_name] [video_dir] [smile_path]\n"
          "Example:\n"
          "  utils http://pic33.nipic.com/20131007/13639685_123501617185_2.jpg 1.jpg 2.jpg\n\n";
}

int main(int argc, const char *argv[]) {
  if (argc < 4) {
    help();
    return 0;
  }

  char *url = (char *)argv[1];
  char *videoName = (char *)argv[2];
  char *videoDir = (char *)argv[3];
  char *smilePath = (char *)argv[4];

  if(download(url, videoName) == -1) {
    cerr << "download video file error, please check url validation!" << endl;
    return -1;
  }
  if (__mkdir__(videoDir) == -1) {
    cerr << "create video dir error, please check permissions!" << endl;
    return -2;
  }

  if (video_to_image(videoName, videoDir) == -1) {
    cerr << "video transfer image error, please check video exists!" << endl;
    return -3;
  }

  // Read the image, convert it into gray image, and then equalize the
  // histogram.
  saveSmile(videoDir, smilePath);

  if (__rmdir__(videoDir) == -1) {
    cerr << "remove video dir error, please check permissions!" << endl;
    return -4;
  }
  return 0;
}
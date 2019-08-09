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
          "  vc [url]\n"
          "Example:\n"
          "  vc www.google.com \n\n";
}

int main(int argc, const char *argv[]) {
  const char *base_dir =    "/home/wwwroot/my_resume/static/images/dlcv";
  const char *video_name =  "/home/wwwroot/my_resume/static/images/dlcv/video.mp4";
  const char *video_dir =   "/home/wwwroot/my_resume/static/images/dlcv/video";
  const char *smile_dir =   "/home/wwwroot/my_resume/static/images/dlcv/static";
  const char *smile =       "/home/wwwroot/my_resume/static/images/dlcv/static/smile.png";
  if (__mkdir__(base_dir) != -1)
    cout << "detector base dir not exists, auto create it." << endl;
  else
    cout << "detector base dir exists, not create." << endl;

  if (__mkdir__(video_dir) != -1)
    cout << "detector video dir not exists, auto create it." << endl;
  else
    cout << "detector video dir exists, not create." << endl;
  if (__mkdir__(smile_dir) != -1)
    cout << "detector smile dir not exists, auto create it." << endl;
  else
    cout << "detector smile dir exists, not create." << endl;

  if (video_to_image(video_name, video_dir) == -1) {
    cerr << "video transfer image error, please check video exists!" << endl;
    return -3;
  }

  // Read the image, convert it into gray image, and then equalize the
  // histogram.
  save_smile(video_dir, smile);

  if (__rmdir__(video_dir) == -1) {
    cerr << "remove video dir error, please check permissions!" << endl;
    return -5;
  }
  return 0;
}
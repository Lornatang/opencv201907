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
          "  vc [url] [video_name] [video_dir] [smile_path]\n"
          "Example:\n"
          "  vc https://apd-8a4572ab0afe85c93b0e4877c5df81cc.v.smtcdns.com/"
          "om.tc.qq.com/AHKbD-BmXB-ra-Aps7tRHbbfeRt8BQsuLEkYIt6Vs9h0/uwMROf"
          "z2r5zEIaQXGdGnC2dfJ7xlHUZLN7WLSNBHzUj-9W46/k0660momfpn.p712.1.mp4"
          "?sdtfrom=v1010&guid=51b813b2e8d8ad0a480e1e104d15fccf&vkey=9E5CF93"
          "8961E74F9A9D2EB05E4E07C0DF27EA6A09DBE7832CCA375172F90638455DD5667"
          "EA2A0A7FD28F14DEB322FE0D8D1A2DF77910EE8E274D5894ECA4AA60767359F47"
          "845F81BB8820B9AD5512A34CFA37D75D1B5F956F3B4A9F15BBDC1E72546FD18CD"
          "1C5F8B6905181D6775D3E64332E8658E964A288792F669F66E33F9 video.mp4 "
          "video smile.png\n\n";
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
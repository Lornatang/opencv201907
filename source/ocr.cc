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

#include "dlcv/dlcv_ocr.hpp"

using namespace cv;
using namespace std;

int main(int argc, const char *argv[]) {
  const char *base_dir =      "/home/wwwroot/my_resume/static/images/dlcv";
  const char *raw_path =      "/home/wwwroot/my_resume/static/images/dlcv/raw_ocr.png";
  const char *correct_path =  "/home/wwwroot/my_resume/static/images/dlcv/correct.png";
  const char *face =          "/home/wwwroot/my_resume/static/images/dlcv/face.png";
  const char *binary_path =   "/home/wwwroot/my_resume/static/images/dlcv/binary.png";
  const char *text_path =     "/home/wwwroot/my_resume/static/images/dlcv/text.txt";

  if (__mkdir__(base_dir) != -1)
    cout << "detector base dir not exists, auto create it." << endl;
  else
    cout << "detector base dir exists, not create." << endl;

//  if (download_file(const_cast<char *>(url), const_cast<char *>(raw_path)) == CURLE_FAILED_INIT) {
//    cerr << "download file error!" << endl;
//    return -1;
//  }


  double angle = compute_skew(raw_path);
  Mat correct_image = deskew(raw_path, angle);
  imwrite(correct_path, correct_image);

  if (save_face(raw_path, face) == -1)
    cerr << "no face!" << endl;

  Mat newImage = img_to_binary(raw_path);
  imwrite(binary_path, newImage);

  const char *text = image_to_string(raw_path);

  if (save_data(text, text_path) == -1) {
    cerr << "ocr not detector any data." << endl;
    return -1;
  }
  return 0;
}

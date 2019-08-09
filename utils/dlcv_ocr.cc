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

/**
 * Convert color images to binary images for processing.
 * Args:
 *   filename: input file name.
 * Returns:
 *   Mat.
 * @author: Changyu Liu.
 * @last modify time: 2019.8.6.
 */
Mat img_to_binary(const String &filename) {
  Mat img = imread(filename);
  Mat gray;
  cvtColor(img, gray, COLOR_BGR2GRAY);
  threshold(gray, gray, 128, 255, THRESH_BINARY | THRESH_OTSU);
  return gray;
}

/**
 * Calculate image tilt Angle.
 * Args:
 *   filename: Mat image.
 * Returns:
 *   double angle.
 * Example:
 *   image = imread("./lena.png");
 *   cvtColor(image, gray, COLOR_BGR2GRAY);
 *   double angle = compute_skew(gray);
 *   cout << "angle: " << angle << endl;
 * @author: Changyu Liu
 * @last modify time: 2019.8.5
 */
double compute_skew(const char *filename) {
  Mat image = imread(filename, 0);
  // Binarize
  threshold(image, image, 225, 255, THRESH_BINARY);

  // Invert colors
  bitwise_not(image, image);

  // Now perform our erosion
  Mat element = getStructuringElement(MORPH_RECT, Size(5, 3));
  erode(image, image, element);

  vector<Point> points;
  Mat_<uchar>::iterator it = image.begin<uchar>();
  Mat_<uchar>::iterator end = image.end<uchar>();
  for (; it != end; ++it)
    if (*it)
      points.push_back(it.pos());

  RotatedRect box = minAreaRect(Mat(points));
  double angle = box.angle;
  if (angle < -45.)
    angle += 90.;

  return angle;
}

/**
 * Convert and intercept an image that has found an Angle of tilt.
 * Args:
 *   filename: Mat image.
 * Returns:
 *   double angle.
 * @example:
 *   image = imread("./lena.png");
 *   cvtColor(image, gray, COLOR_BGR2GRAY);
 *   double angle = compute_skew(gray);
 *   deskew(image, angle);
 * @author: Changyu Liu
 * @last modify time: 2019.8.5
 */
Mat deskew(const char *file_name, double angle) {
  Mat image = imread(file_name);
  Mat gray;
  cvtColor(image, gray, COLOR_BGR2GRAY);
  bitwise_not(gray, gray);

  vector<Point> points;
  Mat_<uchar>::iterator it = gray.begin<uchar>();
  Mat_<uchar>::iterator end = gray.end<uchar>();
  for (; it != end; ++it)
    if (*it)
      points.push_back(it.pos());

  RotatedRect box = minAreaRect(Mat(points));

  // rotation
  Mat rot_mat = getRotationMatrix2D(box.center, angle, 1);

  Mat rotated;
  warpAffine(image, rotated, rot_mat, image.size(), INTER_CUBIC);

  // Cropping
  Size box_size = box.size;
  if (box.angle < -45.)
    swap(box_size.width, box_size.height);
  Mat cropped;
  getRectSubPix(rotated, box_size, box.center, cropped);
  return cropped;
}

/**
 * ocr.
 * Args:
 *   filename: input file name.
 * Returns:
 *   string.
 * @author: Changyu Liu
 * @last modify time: 2019.8.6
 */
char *image_to_string(const char *filename) {
  // Create Tesseract object
  auto *ocr = new tesseract::TessBaseAPI();

  // Initialize tesseract to use English (eng) and the LSTM OCR engine.
  ocr->Init(nullptr, "chi_sim", tesseract::OEM_LSTM_ONLY);

  // Set Page segmentation mode to PSM_AUTO (3)
  ocr->SetPageSegMode(tesseract::PSM_AUTO);

  // Open input image using OpenCV
  Mat im = cv::imread(filename, IMREAD_COLOR);

  // Set image data
  ocr->SetImage(im.data, im.cols, im.rows, 3, im.step);

  // Run Tesseract OCR on image
  char *text = ocr->GetUTF8Text();

  return text;
}

/**
 * save string data to text.
 * Args:
 *   text: ocr extract text.
 *   filename: save data file path.
 * Returns:
 *   success return 0, else return -1.
 * @author: Changyu Liu
 * @last modify time: 2019.8.6
 */
int save_data(const char *text, const char *filename) {
  if (text == nullptr)
    return -1;
  remove((const char *) filename);
  ofstream out(filename, ios::app);
  out << text << endl;
  out.close();
  return 0;
}

/**
 * detector card face and save it.
 * Args:
 *   filename: input card image.
 *   save_path: face rol save path.
 * Returns:
 *   vector<Rect>.
 * @author: Changyu Liu
 * @last modify time: 2019.8.7
 */
int save_face(const char *filename, const char *save_path) {
  Mat image = imread(filename);
  Mat gray;
  cvtColor(image, gray, COLOR_BGR2GRAY);
  CascadeClassifier facesCascade;
  vector<Rect> faces;
  facesCascade.load(
          "/usr/local/share/opencv4/haarcascades/"
          "haarcascade_frontalface_default.xml");
  facesCascade.detectMultiScale(gray, faces, 1.1, 3, CASCADE_SCALE_IMAGE,
                                Size(50, 50));

  if (faces.empty())
    return -1;

  for (auto &face : faces) {
    Point top_right, bottom_left;

    top_right.x = face.x - face.width * 0.25;
    top_right.y = face.y - face.height * 0.4;

    bottom_left.x = face.x + face.width * 1.35;
    bottom_left.y = face.y + face.height * 1.4;

    // rectangle(image.clone(), top_right, bottom_left, Scalar(0, 0, 255), 2);

    Rect rect(top_right.x, top_right.y, face.width * 1.5, face.height * 1.8);
    Mat ROI = image(rect);
    imwrite(save_path, ROI);
  }
  return 0;
}


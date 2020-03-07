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

#include "../include/dlcv_ocr.hpp"

using namespace cv;
using namespace std;

Point2f center(0, 0);

// finally image height and width
int g_dst_height;
int g_dst_width;

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

bool sort_corners(std::vector<cv::Point2f> &corners) {
  std::vector<cv::Point2f> top, bot;
  cv::Point2f tmp_pt;
  std::vector<cv::Point2f> olddata = corners;

  if (corners.size() != 4) {
    return false;
  }

  for (size_t i = 0; i < corners.size(); i++) {
    for (size_t j = i + 1; j < corners.size(); j++) {
      if (corners[i].y < corners[j].y) {
        tmp_pt = corners[i];
        corners[i] = corners[j];
        corners[j] = tmp_pt;
      }
    }
  }
  top.push_back(corners[0]);
  top.push_back(corners[1]);
  bot.push_back(corners[2]);
  bot.push_back(corners[3]);
  if (top.size() == 2 && bot.size() == 2) {
    corners.clear();
    cv::Point2f tl = top[0].x > top[1].x ? top[1] : top[0];
    cv::Point2f tr = top[0].x > top[1].x ? top[0] : top[1];
    cv::Point2f bl = bot[0].x > bot[1].x ? bot[1] : bot[0];
    cv::Point2f br = bot[0].x > bot[1].x ? bot[0] : bot[1];
    corners.push_back(tl);
    corners.push_back(tr);
    corners.push_back(br);
    corners.push_back(bl);
    return true;
  } else {
    corners = olddata;
    return false;
  }
}

cv::Point2f compute_intersect(cv::Vec4i a, cv::Vec4i b) {
  int x1 = a[0], y1 = a[1], x2 = a[2], y2 = a[3];
  int x3 = b[0], y3 = b[1], x4 = b[2], y4 = b[3];

  if (float d = ((float) (x1 - x2) * (y3 - y4)) - ((y1 - y2) * (x3 - x4))) {
    cv::Point2f pt;
    pt.x = ((x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - y3 * x4)) / d;
    pt.y = ((x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4)) / d;
    return pt;
  } else
    return cv::Point2f(-1, -1);
}

bool IsBadLine(int a, int b) {
  return a * a + b * b < 100;
}

bool x_sort(const Point2f &m1, const Point2f &m2) {
  return m1.x < m2.x;
}

// Determine the center line of the four points
void sort_corners(std::vector<cv::Point2f> &corners,
                  cv::Point2f center) {
  std::vector<cv::Point2f> top, bot;
  vector<Point2f> backup = corners;
  sort(corners.begin(), corners.end(), x_sort);

  for (int i = 0; i < corners.size(); i++) {
    // this is less than 2 to avoid the case where all three vertices are at top
    if (corners[i].y < center.y && top.size() < 2)
      top.push_back(corners[i]);
    else
      bot.push_back(corners[i]);
  }
  corners.clear();

  if (top.size() == 2 && bot.size() == 2) {
    cout << "log" << endl;
    cv::Point2f tl = top[0].x > top[1].x ? top[1] : top[0];
    cv::Point2f tr = top[0].x > top[1].x ? top[0] : top[1];
    cv::Point2f bl = bot[0].x > bot[1].x ? bot[1] : bot[0];
    cv::Point2f br = bot[0].x > bot[1].x ? bot[0] : bot[1];

    corners.push_back(tl);
    corners.push_back(tr);
    corners.push_back(br);
    corners.push_back(bl);
  } else {
    corners = backup;
  }
}

void cal_dst_size(const vector<cv::Point2f> &corners) {
  int h1 = sqrt((corners[0].x - corners[3].x) * (corners[0].x - corners[3].x) +
                (corners[0].y - corners[3].y) * (corners[0].y - corners[3].y));
  int h2 = sqrt((corners[1].x - corners[2].x) * (corners[1].x - corners[2].x) +
                (corners[1].y - corners[2].y) * (corners[1].y - corners[2].y));
  g_dst_height = MAX(h1, h2);

  int w1 = sqrt((corners[0].x - corners[1].x) * (corners[0].x - corners[1].x) +
                (corners[0].y - corners[1].y) * (corners[0].y - corners[1].y));
  int w2 = sqrt((corners[2].x - corners[3].x) * (corners[2].x - corners[3].x) +
                (corners[2].y - corners[3].y) * (corners[2].y - corners[3].y));
  g_dst_width = MAX(w1, w2);
}

void adjust(const char *raw_path, const char *new_path) {
  Mat src = imread(raw_path);
  Mat source = src.clone();
  Mat temp = src.clone();
  Mat img = src.clone();
  cvtColor(img, img, COLOR_RGB2GRAY);
  GaussianBlur(img, img, Size(5, 5), 0, 0);

  // Get the custom kernel
  // The first parameter MORPH_RECT represents the convolution kernel of the rectangle.
  // Of course, elliptic and intersected ones can also be selected
  Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
  // dilate ops. very good!
  dilate(img, img, element);
  // edge extraction
  Canny(img, img, 30, 120, 3);

  vector<vector<Point> > contours;
  vector<vector<Point> > f_contours;
  std::vector<cv::Point> approx2;
  // Note that the fifth parameter, CV_RETR_EXTERNAL, retrieves only the outer box
  findContours(img, f_contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);

  // Find the outline with the largest area
  int max_area = 0;
  int index = 0;
  for (int i = 0; i < f_contours.size(); i++) {
    double tmparea = fabs(contourArea(f_contours[i]));
    if (tmparea > max_area) {
      index = i;
      max_area = tmparea;
    }

  }
  contours.push_back(f_contours[index]);

  vector<Point> tmp = contours[0];

  for (int line_type = 1; line_type <= 3; line_type++) {
    Mat black = img.clone();
    black.setTo(0);
    // Pay attention to the thickness of the line, do not choose too thin
    drawContours(black, contours, 0, Scalar(255), line_type);

    std::vector<Vec4i> lines;
    std::vector<cv::Point2f> corners;
    std::vector<cv::Point2f> approx;

    int para = 10;
    int flag = 0;
    int round = 0;
    for (; para < 300; para++) {
      lines.clear();
      corners.clear();
      approx.clear();
      center = Point2f(0, 0);

      cv::HoughLinesP(black, lines, 1, CV_PI / 180, para, 30, 10);

      // Filter lines that are too close
      std::set<int> ErasePt;
      for (int i = 0; i < lines.size(); i++) {
        for (int j = i + 1; j < lines.size(); j++) {
          if (IsBadLine(abs(lines[i][0] - lines[j][0]), abs(lines[i][1] - lines[j][1])) &&
              (IsBadLine(abs(lines[i][2] - lines[j][2]), abs(lines[i][3] - lines[j][3])))) {
            // add the bad line to the collection
            ErasePt.insert(j);
          }
        }
      }

      int Num = lines.size();
      while (Num != 0) {
        std::set<int>::iterator j = ErasePt.find(Num);
        if (j != ErasePt.end()) {
          lines.erase(lines.begin() + Num - 1);
        }
        Num--;
      }
      if (lines.size() != 4) {
        continue;
      }

      // Calculate the intersection of the lines and save the portion within the image range
      for (int i = 0; i < lines.size(); i++) {
        for (int j = i + 1; j < lines.size(); j++) {
          cv::Point2f pt = compute_intersect(lines[i], lines[j]);
          // Make sure the intersection is within the range of the image
          if (pt.x >= 0 && pt.y >= 0 && pt.x <= src.cols && pt.y <= src.rows)
            corners.push_back(pt);
        }
      }
      if (corners.size() != 4) {
        continue;
      }
#if 1
      bool IsGoodPoints = true;

      // Make sure the distance between points is large enough to eliminate the error points
      for (int i = 0; i < corners.size(); i++) {
        for (int j = i + 1; j < corners.size(); j++) {
          int distance = sqrt((corners[i].x - corners[j].x) * (corners[i].x - corners[j].x) +
                              (corners[i].y - corners[j].y) * (corners[i].y - corners[j].y));
          if (distance < 5) {
            IsGoodPoints = false;
          }
        }
      }

      if (!IsGoodPoints) continue;
#endif
      cv::approxPolyDP(cv::Mat(corners), approx, cv::arcLength(cv::Mat(corners), true) * 0.02, true);

      if (lines.size() == 4 && corners.size() == 4 && approx.size() == 4) {
        flag = 1;
        break;
      }
    }

    // Get mass center
    for (int i = 0; i < corners.size(); i++)
      center += corners[i];
    center *= (1. / corners.size());

    if (flag) {
      cv::circle(temp, corners[0], 3, CV_RGB(255, 0, 0), -1);
      cv::circle(temp, corners[1], 3, CV_RGB(0, 255, 0), -1);
      cv::circle(temp, corners[2], 3, CV_RGB(0, 0, 255), -1);
      cv::circle(temp, corners[3], 3, CV_RGB(255, 255, 255), -1);
      cv::circle(temp, center, 3, CV_RGB(255, 0, 255), -1);

      sort_corners(corners, center);
//      cout << "corners size" << corners.size() << endl;
//      cout << "tl:" << corners[0] << endl;
//      cout << "tr:" << corners[1] << endl;
//      cout << "br:" << corners[2] << endl;
//      cout << "bl:" << corners[3] << endl;

      cal_dst_size(corners);

      cv::Mat quad = cv::Mat::zeros(g_dst_height, g_dst_width, CV_8UC3);
      std::vector<cv::Point2f> quad_pts;
      quad_pts.push_back(cv::Point2f(0, 0));
      quad_pts.push_back(cv::Point2f(quad.cols, 0));
      quad_pts.push_back(cv::Point2f(quad.cols, quad.rows));

      quad_pts.push_back(cv::Point2f(0, quad.rows));

      cv::Mat transmtx = cv::getPerspectiveTransform(corners, quad_pts);
      cv::warpPerspective(source, quad, transmtx, quad.size());

      /*process for id card*/
      Mat local, gray;
      cvtColor(quad, gray, COLOR_RGB2GRAY);
      int blockSize = 25;
      int constValue = 10;
      adaptiveThreshold(gray, local, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, blockSize, constValue);
      imwrite(new_path, local);
    }
  }
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


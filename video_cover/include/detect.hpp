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

#ifndef INCLUDE_DETECT_HPP
#define INCLUDE_DETECT_HPP

#include <iostream>
#include <vector>
#include "core.hpp"
#include "highgui.hpp"
#include "imgproc.hpp"
#include "imgcodecs.hpp"
#include "opencv.hpp"

using namespace std;
using namespace cv;

vector<Rect> detectFaces(Mat gray) {
  CascadeClassifier facesCascade;
  vector<Rect> faces;
  facesCascade.load(
          "/usr/local/share/opencv4/haarcascades/"
          "haarcascade_frontalface_default.xml");
  facesCascade.detectMultiScale(gray, faces, 1.1, 2, 0 | CASCADE_SCALE_IMAGE,
                                Size(30, 30));
  return faces;
}

vector<Rect> detectSmile(Mat faces) {
  CascadeClassifier smileCascade;
  vector<Rect> smiles;
  smileCascade.load(
          "/usr/local/share/opencv4/haarcascades/haarcascade_smile.xml");
  smileCascade.detectMultiScale(faces, smiles, 1.1, 2, 0 | CASCADE_SCALE_IMAGE,
                                Size(30, 30));
  return smiles;
}

void detectDrawEyes(Mat img, Mat img_gray) {
  vector<Rect> faces = detectFaces(img_gray);
  for (size_t i = 0; i < faces.size(); i++) {
    Mat faceROI = img_gray(faces[i]);
    CascadeClassifier eyes_cascade;
    eyes_cascade.load(
            "/usr/local/share/opencv4/haarcascades/haarcascade_eye.xml");
    vector<Rect> eyes;
    eyes_cascade.detectMultiScale(faceROI, eyes, 1.1, 2,
                                  0 | CASCADE_SCALE_IMAGE, Size(30, 30));
    for (size_t j = 0; j < eyes.size(); j++) {
      Point eyes_center(faces[i].x + eyes[j].x + eyes[j].width / 2,
                        faces[i].y + eyes[j].y + eyes[j].height / 2);
      int r = cvRound((eyes[j].width + eyes[j].height) * 0.25);
      circle(img, eyes_center, r, Scalar(255, 0, 0), 1, 8, 0);
    }
  }
  namedWindow("detect and draw eyes");
  imshow("detect and draw eyes", img);
}

void saveFaces(Mat img, Mat img_gray) {
  vector<Rect> faces = detectFaces(img_gray);
  for (size_t i = 0; i < faces.size(); i++) {
    stringstream buffer;
    buffer << i;
    string saveName = "smile" + buffer.str() + ".png";
    Rect roi = faces[i];
    imwrite(saveName, img(roi));
  }
}

void drawFaces(Mat img, vector<Rect> faces) {
  namedWindow("draw faces");
  for (size_t i = 0; i < faces.size(); i++) {
    Point center(faces[i].x + faces[i].width / 2,
                 faces[i].y + faces[i].height / 2);
    ellipse(img, center, Size(faces[i].width / 2, faces[i].height / 1.5), 0, 0,
            360, Scalar(0, 255, 0), 2, 8, 0);
  }
  imshow("draw faces", img);
}

#endif  // INCLUDE_DETECT_HPP

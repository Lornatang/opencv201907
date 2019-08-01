// test.cc
#include <opencv4/opencv2/opencv.hpp>
using namespace cv;

int main(int argc, char **argv) {
  Mat img = cv::imread(argv[1], -1);
  if (img.empty()) return -1;
  cv::namedWindow("Test", cv::WINDOW_AUTOSIZE);
  cv::imshow("Test", img);
  cv::waitKey(0);
  cv::destroyWindow("Test");
  return 0;
}

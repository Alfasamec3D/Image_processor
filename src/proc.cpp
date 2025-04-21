#include "proc.hpp"

#include <thread>

void grayscale_section(cv::Mat& image, const int& start, const int& end) {
  for (int i = start; i < end; ++i) {
    for (int j = 0; j < image.cols; ++j) {
      cv::Vec3b& pixel = image.at<cv::Vec3b>(i, j);
      uchar gray = static_cast<uchar>(0.11 * pixel[0] + 0.59 * pixel[1] +
                                      0.3 * pixel[2]);
      pixel = cv::Vec3b(gray, gray, gray);
    }
  }
}

void grayscale(cv::Mat& image, int numthreads) {
  std::vector<std::thread> threads;
  for (int i = 0; i < numthreads; ++i) {
    int start = image.rows / numthreads * i;
    int end =
        (i == numthreads - 1) ? image.rows : image.rows / numthreads * (i + 1);
    threads.emplace_back(grayscale_section, std::ref(image), start, end);
  }
  for (auto& th : threads) th.join();
}

// Gaussian blur of the section of the image
void imageblur_section(cv::Mat& input, cv::Mat& output, int start, int end,
                       int depth) {
  cv::Mat section = input.rowRange(start, end);
  cv::Mat output_section = output.rowRange(start, end);
  cv::GaussianBlur(section, output_section, cv::Size(depth, depth), 0);
}

// Gaussian blur if the whole image
void imageblur(cv::Mat& input, cv::Mat& output, int depth, int numthreads) {
  std::vector<std::thread> threads;
  cv::Mat region;
  cv::Mat outputregion;
  for (int i = 0; i < numthreads; ++i) {
    int start = input.rows / numthreads * i;
    int end =
        (i == numthreads - 1) ? input.rows : input.rows / numthreads * (i + 1);

    threads.emplace_back(imageblur_section, std::ref(input), std::ref(output),
                         start, end, depth);
  }
  for (auto& th : threads) th.join();
}
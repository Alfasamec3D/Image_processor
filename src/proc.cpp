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

void grayscale(cv::Mat& image) {
  int numthreads = std::thread::hardware_concurrency();
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
void imageblur_section(const cv::Mat& input, cv::Mat& output, int start,
                       int end, int depth) {
  cv::Mat section = input.rowRange(start, end);
  cv::Mat output_section = output.rowRange(start, end);
  cv::GaussianBlur(section, output_section, cv::Size(depth, depth), 0);
}

// Gaussian blur if the whole image
void imageblur(const cv::Mat& input, cv::Mat& output, int depth) {
  int numthreads = std::thread::hardware_concurrency();
  if (input.empty()) {
    throw std::runtime_error("imageblur(): input image is empty!");
  }
  std::vector<std::thread> threads;
  for (int i = 0; i < numthreads; ++i) {
    int start = input.rows / numthreads * i;
    int end =
        (i == numthreads - 1) ? input.rows : input.rows / numthreads * (i + 1);
    threads.emplace_back(imageblur_section, std::ref(input), std::ref(output),
                         start, end, depth);
  }

  for (auto& th : threads) th.join();
}

void brightness_contrast_section(const cv::Mat& input, cv::Mat& output,
                                 const int start, const int end, const int a,
                                 const int b) {
  cv::Mat section = input.rowRange(start, end);
  cv::Mat output_section = output.rowRange(start, end);
  for (int y = 0; y < section.rows; y++) {
    for (int x = 0; x < section.cols; x++) {
      for (int c = 0; c < section.channels(); c++) {
        output_section.at<cv::Vec3b>(y, x)[c] =
            cv::saturate_cast<uchar>(a * section.at<cv::Vec3b>(y, x)[c] + b);
      }
    }
  }
}

void brightness_contrast(const cv::Mat& input, cv::Mat& output, const int a,
                         const int b) {
  const int numthreads = std::thread::hardware_concurrency();
  std::vector<std::thread> threads;
  cv::Mat region;
  cv::Mat outputregion;
  for (int i = 0; i < numthreads; ++i) {
    int start = input.rows / numthreads * i;
    int end =
        (i == numthreads - 1) ? input.rows : input.rows / numthreads * (i + 1);

    threads.emplace_back(brightness_contrast_section, std::ref(input),
                         std::ref(output), start, end, a, b);
  }
  for (auto& th : threads) th.join();
}
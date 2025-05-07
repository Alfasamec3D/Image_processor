#include "proc.hpp"

#include <thread>

void grayscale_section(const cv::Mat& input, cv::Mat& output, const int& start,
                       const int& end) {
  for (int i = start; i < end; ++i) {
    for (int j = 0; j < input.cols; ++j) {
      cv::Vec3b& pixel = output.at<cv::Vec3b>(i, j);
      const cv::Vec3b& pixel_in = input.at<cv::Vec3b>(i, j);
      uchar gray = static_cast<uchar>(0.11 * pixel_in[0] + 0.59 * pixel_in[1] +
                                      0.3 * pixel_in[2]);
      pixel = cv::Vec3b(gray, gray, gray);
    }
  }
}

//literally graysacaling an image
void grayscale(const cv::Mat& input, cv::Mat& output) {
  int numthreads = std::thread::hardware_concurrency();
  std::vector<std::thread> threads;
  for (int i = 0; i < numthreads; ++i) {
    int start = output.rows / numthreads * i;
    int end = (i == numthreads - 1) ? output.rows
                                    : output.rows / numthreads * (i + 1);
    threads.emplace_back(grayscale_section, std::ref(input), std::ref(output),
                         start, end);
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

//Changing brightness and contrast of section of image
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

//Changing brightness and contrast of whole image
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

//Simply speaking applying all of the filters
void updateImage(const cv::Mat& input, cv::Mat& output, const int a,
                 const int b, bool graystate, const int depth, const int rotatestate,
                 const int flipstate) {
  output = input.clone();
  brightness_contrast(input, output, a, b);
  cv::Mat result = output.clone();

  if (graystate) {
    grayscale(result, output);
    result = output.clone();
  }

  imageblur(result, output, depth * 2 + 1);
  result = output.clone();

  if (flipstate) {
    flip(result, output, 1);
    result = output.clone();
  }

  output =cv::Mat(result.size(),result.type() );
  switch (rotatestate) {
    case 0:
      output = result.clone();
      break;
    case 1:
      rotate(result, output, cv::ROTATE_90_CLOCKWISE);
      break;
    case 2:
      rotate(result, output, cv::ROTATE_180);
      break;
    case 3:
      rotate(result, output, cv::ROTATE_90_COUNTERCLOCKWISE);
      break;
  }
  return;
}
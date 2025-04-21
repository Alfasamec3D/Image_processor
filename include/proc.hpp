#ifndef PROC_HPP
#define PROC_HPP

#include <opencv2/opencv.hpp>

cv::Mat load_image(const cv::String& filename);

void grayscale_section(cv::Mat& image, const int& start, const int& end);

void grayscale(cv::Mat& image, int numthreads);

void imageblur_section(cv::Mat& input, cv::Mat& output, int start, int end,
                       int depth);
void imageblur(cv::Mat& input, cv::Mat& output, int numthreads, int depth);

#endif
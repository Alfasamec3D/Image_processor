#ifndef PROC_HPP
#define PROC_HPP

#include <opencv2/opencv.hpp>

cv::Mat load_image(const cv::String& filename);

void grayscale_section(const cv::Mat& input, cv::Mat& output, const int& start, const int& end);

void grayscale(const cv::Mat& input, cv::Mat& output);

void imageblur_section(const cv::Mat& input, cv::Mat& output,const int start,const int end,
                       const int depth);
void imageblur(const cv::Mat& input, cv::Mat& output, const int depth);

void brightness_contrast_section(const cv::Mat& input, cv::Mat& output, const int start,
                                 const int end,const  int a, const int b);

void brightness_contrast(const cv::Mat& input, cv::Mat& output, 
                         const int a, const int b);

void updateImage(const cv::Mat& input, cv::Mat& output, const int a,
                 const int b, bool graystate, int depth, int rotatestate,
                 int flipstate);

#endif
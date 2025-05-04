#include "proc.hpp"
#include <gtest/gtest.h>
#include <thread> 

TEST(ImageProcessingTest, GrayscaleConversion) { 
  int numthreads =std::thread::hardware_concurrency();
  cv::String image_path1{"../tests/temple.jpg"};
  cv::String image_path2{"../tests/grayscaleTest.png"};
  cv::Mat input = cv::imread(image_path1);
  cv::Mat expected = cv::imread(image_path2);

  ASSERT_FALSE(input.empty())<<"Failed to load input image.";
  ASSERT_FALSE(expected.empty()) << "Failed to load expected grayscale image.";

  grayscale(input);

  cv::Mat diff;
  cv::absdiff  (input, expected, diff);
  int numDiffPixels = cv :: countNonZero(diff.reshape(1));

  EXPECT_EQ(numDiffPixels, 0)
      << "The grayscale coversion result differs from the expected output.";
}
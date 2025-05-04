#include "proc.hpp"
#include <gtest/gtest.h>
#include <thread>

TEST(ImageProcessingTest, GrayscaleSectionConversion) { 
    const int numthreads =4;
  cv::String image_path1{"../tests/temple.jpg"};
  cv::String image_path2{"../tests/grayscalesectionTest.png"};
  cv::Mat input = cv::imread(image_path1);
  cv::Mat expected = cv::imread(image_path2);
  int i = 2;
  int start = input.rows / numthreads * i;
  int end =
      (i == numthreads - 1) ? input.rows : input.rows / numthreads * (i + 1);
  
  ASSERT_FALSE(i >= numthreads);
  ASSERT_FALSE(input.empty())<<"Failed to load input image.";
  ASSERT_FALSE(expected.empty()) << "Failed to load expected grayscale image.";

  grayscale_section(input, start, end);

  cv::Mat diff;
  cv::absdiff  (input, expected, diff);
  int numDiffPixels = cv :: countNonZero(diff.reshape(1));

  EXPECT_EQ(numDiffPixels, 0)
      << "The grayscale section coversion result differs from the expected output.";
}
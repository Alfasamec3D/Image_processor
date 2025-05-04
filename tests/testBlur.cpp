#include "proc.hpp"
#include <gtest/gtest.h>

TEST(ImageProcessingTest, BlurConversion) { 
  const cv::String image_path1{"../tests/temple.jpg"};
  const cv::String image_path2{"../tests/blurTest.png"};
  const cv::Mat input = cv::imread(image_path1);
  const cv::Mat expected = cv::imread(image_path2);
  cv::Mat output(input.size(),input.type() );
  const int i = 2;
  const int depth = 9;
  ASSERT_FALSE(input.empty())<<"Failed to load input image.";
  ASSERT_FALSE(expected.empty()) << "Failed to load expected grayscale image.";

  imageblur(std::ref(input), std::ref(output), depth);

  cv::Mat diff;
  cv::absdiff  (output, expected, diff);
  int numDiffPixels = cv :: countNonZero(diff.reshape(1));

  EXPECT_EQ(numDiffPixels, 0)
      << "The blur coversion result differs from the expected output.";
}
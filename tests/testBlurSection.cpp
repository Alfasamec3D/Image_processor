#include "proc.hpp"
#include <gtest/gtest.h>

TEST(ImageProcessingTest, BlurSectionConversion) {
  const int numthreads = 4;
  const cv::String image_path1{"../tests/temple.jpg"};
  const cv::String image_path2{"../tests/blursectionTest.png"};
  const cv::Mat input = cv::imread(image_path1);
  const cv::Mat expected = cv::imread(image_path2);
  cv::Mat output(input.size(),input.type() );
  const int i = 2;
  const int start = input.rows / numthreads * i;
  const int end =
      (i == numthreads - 1) ? input.rows : input.rows / numthreads * (i + 1);
  const int depth = 9;
  ASSERT_FALSE(input.empty())<<"Failed to load input image.";
  ASSERT_FALSE(expected.empty()) << "Failed to load expected grayscale image.";

  imageblur_section(std::ref(input), std::ref(output), start, end, depth);

  cv::Mat diff;
  cv::absdiff  (output, expected, diff);
  int numDiffPixels = cv :: countNonZero(diff.reshape(1));

  EXPECT_EQ(numDiffPixels, 0)
      << "The blur section coversion result differs from the expected output.";
}
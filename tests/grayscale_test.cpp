#include "proc.hpp"
int main()
{
  cv::String image_path{"/mnt/9C468886468862BE/media/programs/image_processor/media/temple.jpg"};
  cv::Mat image =load_image(image_path);
  grayscale(image, 10);
}
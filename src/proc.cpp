#include "proc.hpp"
#include <thread>
QImage cvMatToQImage(const cv::Mat &mat) {
  cv::Mat rgb;
  cv::cvtColor(mat, rgb, cv::COLOR_BGR2RGB);
  return QImage(rgb.data, rgb.cols, rgb.rows, rgb.step, QImage::Format_RGB888)
      .copy();
}
cv::Mat load_image(const cv::String& filename) {
  cv::Mat image = cv::imread(filename);
  if (image.empty()) {
    std::cerr << "Error: Could not load image";
  }
  return image;
}

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
    int end = (i == numthreads - 1) ? image.rows
                                   : image.rows / numthreads * (i + 1);
    threads.emplace_back(grayscale_section, std::ref(image), start, end);
  }
  for (auto& th: threads) th.join();
}

MainAppWindow::MainAppWindow() { 
  imageLable->setAlignment(Qt::AlignCenter);
  setCentralWidget(imageLabel);
  QToolBar* toolbar = addToolBar("Toolbar");
  QAction* loadAction = new QAction("Load File", this);

  toolbar->addAction(loadAction);
  connect(loadAction, &QAction::triggered, this, &ImageWindow::loadImage);
}
MainAppWindow::loadImage()
{
  QString fileName = QFileDialog::getOpenFileName(
      this, "Open Image", "", "Image (*.png *.jpg *.jpeg *.bmp)");
      if (!fileName.isEmpty())
      {
        QPixmap pix(fileName);
        imageLabel->setPixmap(pix.scaled(
            imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
      }
}
MainAppWindow::resizeEvent(QResizeEvent *) override{
  if (!imageLabel->)
}
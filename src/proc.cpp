#include "proc.hpp"
#include <QAction>
#include <QFileDialog>
#include <QToolBar>
#include <thread>

QImage cvMatToQImage(const cv::Mat& mat) {
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
    int end =
        (i == numthreads - 1) ? image.rows : image.rows / numthreads * (i + 1);
    threads.emplace_back(grayscale_section, std::ref(image), start, end);
  }
  for (auto& th : threads) th.join();
}

MainAppWindow::MainAppWindow() {
  imageLabel = new QLabel(this);
  imageLabel->setAlignment(Qt::AlignCenter);
  setCentralWidget(imageLabel);
  QToolBar* toolbar = addToolBar("Toolbar");
  QAction* loadAction = new QAction("Load File", this);
  QAction* grayscaleAction = new QAction("Grayscale", this);
  toolbar->addAction(loadAction);
  toolbar->addAction(grayscaleAction);
  connect(loadAction, &QAction::triggered, this, &MainAppWindow::loadImage);
  connect(grayscaleAction, &QAction::triggered, this,
          &MainAppWindow::applyGrayscale);
}

void MainAppWindow::loadImage() {
  QString fileName = QFileDialog::getOpenFileName(
      this, "Open Image", "", "Image (*.png *.jpg *.jpeg *.bmp)");
  if (!fileName.isEmpty()) {
    QPixmap pix(fileName);
    imageLabel->setPixmap(pix.scaled(imageLabel->size(), Qt::KeepAspectRatio,
                                     Qt::SmoothTransformation));
  }
}

void MainAppWindow::applyGrayscale() {   QPixmap currentPixmap = imageLabel->pixmap(Qt::ReturnByValue);
  if (currentPixmap.isNull()) return;

  // Convert QPixmap to QImage to cv::Mat
  QImage qImage = currentPixmap.toImage().convertToFormat(QImage::Format_RGB888);
  cv::Mat cvImage(qImage.height(), qImage.width(), CV_8UC3, 
                 const_cast<uchar*>(qImage.bits()), qImage.bytesPerLine());

  // Convert RGB to BGR (OpenCV expects BGR)
  cv::Mat bgrImage;
  cv::cvtColor(cvImage, bgrImage, cv::COLOR_RGB2BGR);

  // Apply grayscale
  grayscale(bgrImage, 4);  // Use your existing function

  // Convert back to RGB for Qt
  cv::Mat rgbResult;
  cv::cvtColor(bgrImage, rgbResult, cv::COLOR_BGR2RGB);

  // Convert to QImage
  QImage resultImage(rgbResult.data, rgbResult.cols, rgbResult.rows, 
                    rgbResult.step, QImage::Format_RGB888);
  
  // Ensure deep copy
  imageLabel->setPixmap(QPixmap::fromImage(resultImage.copy()));
}

void MainAppWindow::resizeEvent(QResizeEvent*) {
  QPixmap pix = imageLabel->pixmap(Qt::ReturnByValue);
  if (pix.isNull()) return;
  imageLabel->setPixmap(pix.scaled(imageLabel->size(), Qt::KeepAspectRatio,
                                   Qt::SmoothTransformation));
}
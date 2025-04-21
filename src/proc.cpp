#include "proc.hpp"

#include <QAction>
#include <QFileDialog>
#include <QMenu>
#include <QMessageBox>
#include <QToolBar>
#include <thread>

QPixmap cvMatToQPixmap(const cv::Mat& mat) {
  cv::Mat rgb;
  cv::cvtColor(mat, rgb, cv::COLOR_BGR2RGB);
  return QPixmap::fromImage(QImage(rgb.data, rgb.cols, rgb.rows, rgb.step,
                                   QImage::Format_RGB888))
      .copy();
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

// Gaussian blur of the section of the image
void imageblur_section(cv::Mat& input, cv::Mat& output, int start, int end,
                       int depth) {
  cv::Mat section = input.rowRange(start, end);
  cv::Mat output_section = output.rowRange(start, end);
  cv::GaussianBlur(section, output_section, cv::Size(depth, depth), 0);
}

// Gaussian blur if the whole image
void imageblur(cv::Mat& input, cv::Mat& output, int depth, int numthreads) {
  std::vector<std::thread> threads;
  cv::Mat region;
  cv::Mat outputregion;
  for (int i = 0; i < numthreads; ++i) {
    int start = input.rows / numthreads * i;
    int end =
        (i == numthreads - 1) ? input.rows : input.rows / numthreads * (i + 1);

    threads.emplace_back(imageblur_section, std::ref(input), std::ref(output),
                         start, end, depth);
  }
  for (auto& th : threads) th.join();
}

MainAppWindow::MainAppWindow() {
  imageLabel = new QLabel(this);
  imageLabel->setAlignment(Qt::AlignCenter);
  setCentralWidget(imageLabel);
  QToolBar* toolbar = addToolBar("Toolbar");

  // Creating actions
  QAction* loadAction = new QAction("Load File", this);
  QAction* grayscaleAction = new QAction("Grayscale", this);
  QAction* saveAction = new QAction("Save File", this);
  QAction* blurAction = new QAction("Gaussian Blur", this);
  QAction* fileAction = new QAction("File", this);

  QMenu* fileMenu = new QMenu(this);
  fileMenu->addAction(loadAction);
  fileMenu->addAction(saveAction);

  // Connect menu to main button
  fileAction->setMenu(fileMenu);

  // Adding action buttons to the toolbar
  toolbar->addAction(fileAction);
  toolbar->addAction(grayscaleAction);
  toolbar->addAction(blurAction);

  // Connecting buttons and actions
  connect(loadAction, &QAction::triggered, this, &MainAppWindow::loadImage);
  connect(grayscaleAction, &QAction::triggered, this,
          &MainAppWindow::applyGrayscale);
  connect(saveAction, &QAction::triggered, this, &MainAppWindow::saveImage);
  connect(blurAction, &QAction::triggered, this, &MainAppWindow::applyBlur);
}

void MainAppWindow::loadImage() {
  QString fileName = QFileDialog::getOpenFileName(
      this, "Open Image", "", "Image (*.png *.jpg *.jpeg *.bmp)");

  if (!fileName.isEmpty()) {
    currentProcessedImage = cv::imread(fileName.toStdString());
    imageLabel->setPixmap(cvMatToQPixmap(currentProcessedImage)
                              .scaled(imageLabel->size(), Qt::KeepAspectRatio,
                                      Qt::SmoothTransformation));
  }
}

void MainAppWindow::saveImage() {
  if (currentProcessedImage.empty()) {
    QMessageBox::warning(this, "Error", "No processed image to save!");
    return;
  }
  QString fileName = QFileDialog::getSaveFileName(
      this, "Save Image", "", "Images (*.pn *.jpg *.jpeg *.bmp)");
  if (!fileName.isEmpty()) {
    cv::imwrite(fileName.toStdString(), currentProcessedImage);
  }
}

void MainAppWindow::applyGrayscale() {
  QPixmap currentPixmap = imageLabel->pixmap(Qt::ReturnByValue);
  if (currentPixmap.isNull()) return;

  // Apply grayscale

  grayscale(currentProcessedImage, 4);

  // Ensure deep copy
  imageLabel->setPixmap(cvMatToQPixmap(currentProcessedImage)
                            .scaled(imageLabel->size(), Qt::KeepAspectRatio,
                                    Qt::SmoothTransformation));
}

void MainAppWindow::resizeEvent(QResizeEvent*) {
  QPixmap pix = imageLabel->pixmap(Qt::ReturnByValue);
  if (pix.isNull()) return;
  imageLabel->setPixmap(pix.scaled(imageLabel->size(), Qt::KeepAspectRatio,
                                   Qt::SmoothTransformation));
}

void MainAppWindow::applyBlur() {
  QPixmap currentPixmap = imageLabel->pixmap(Qt::ReturnByValue);
  if (currentPixmap.isNull()) return;

  // Apply grayscale
  int depth = 5;
  cv::Mat input = currentProcessedImage.clone();

  imageblur(input, currentProcessedImage, depth, 6);

  // Ensure deep copy
  imageLabel->setPixmap(cvMatToQPixmap(currentProcessedImage)
                            .scaled(imageLabel->size(), Qt::KeepAspectRatio,
                                    Qt::SmoothTransformation));
}
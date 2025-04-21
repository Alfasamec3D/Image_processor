#include "gui.hpp"
#include <QAction>
#include <QFileDialog>
#include <QMenu>
#include <QMessageBox>
#include <QSlider>
#include <QToolBar>
#include <QVBoxLayout>

QPixmap cvMatToQPixmap(const cv::Mat& mat) {
  cv::Mat rgb;
  cv::cvtColor(mat, rgb, cv::COLOR_BGR2RGB);
  return QPixmap::fromImage(QImage(rgb.data, rgb.cols, rgb.rows, rgb.step,
                                   QImage::Format_RGB888))
      .copy();
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
  connect(blurAction, &QAction::triggered, this,
          &MainAppWindow::showBlurSlider);
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

void MainAppWindow::applyBlur(int depth) {
  QPixmap currentPixmap = imageLabel->pixmap(Qt::ReturnByValue);
  if (currentPixmap.isNull()) return;

  // Apply grayscale
  cv::Mat input = currentProcessedImage.clone();

  imageblur(input, currentProcessedImage, depth, 6);

  // Ensure deep copy
  imageLabel->setPixmap(cvMatToQPixmap(currentProcessedImage)
                            .scaled(imageLabel->size(), Qt::KeepAspectRatio,
                                    Qt::SmoothTransformation));
}

void MainAppWindow::showBlurSlider() {
  cv::Mat baseImage = currentProcessedImage.clone();
  QDialog* dialog = new QDialog(this);
  dialog->setWindowTitle("Adjust Blur");

  QVBoxLayout* layout = new QVBoxLayout(dialog);

  QLabel* label = new QLabel("Blur Strength", dialog);
  QSlider* slider = new QSlider(Qt::Horizontal, dialog);
  slider->setRange(1, 21);
  slider->setSingleStep(2);
  slider->setValue(5);

  layout->addWidget(label);
  layout->addWidget(slider);
  dialog->setLayout(layout);

  // Connect the slider to applyBlur
  connect(slider, &QSlider::valueChanged, this, [this, &baseImage](int value) {
    if (value % 2 == 0) value += 1;
    if (!baseImage.empty()) {
      cv::Mat output;
      imageblur(baseImage, output, value, 6);
      QPixmap pixmap = cvMatToQPixmap(output).scaled(
          imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
      imageLabel->setPixmap(pixmap);
    }
  });

  dialog->exec();  // Shows the dialog modally
}

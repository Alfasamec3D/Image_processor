#include "gui.hpp"
#include <QAction>
#include <QApplication>
#include <QDebug>
#include <QFileDialog>
#include <QMenu>
#include <QMessageBox>
#include <QToolBar>
#include <QToolButton>
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
  imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
  setCentralWidget(imageLabel);
  QToolBar* toolbar = addToolBar("Toolbar");
  toolbar->setMovable(false);
  toolbar->setFloatable(false);

  // Creating actions
  QAction* loadAction = new QAction("Load File", this);
  QAction* grayscaleAction = new QAction("Grayscale", this);
  QAction* saveAction = new QAction("Save File", this);
  QAction* blurAction = new QAction("Gaussian Blur", this);
  QAction* rotate_clockwiseAction = new QAction("Rotate  90° clockwise", this);
  QAction* rotate_counterclockwiseAction =
      new QAction("Rotate  90° counterclockwise", this);
  QAction* rotate_180Action = new QAction("Rotate 180°", this);
  QAction* flip_horizontalAction = new QAction("Flip Horizontally", this);
  QAction* flip_verticalAction = new QAction("Flip Vertical", this);

  QMenu* positionMenu = new QMenu(this);
  QMenu* fileMenu = new QMenu(this);

  fileMenu->addAction(loadAction);
  fileMenu->addAction(saveAction);
  positionMenu->addAction(rotate_clockwiseAction);
  positionMenu->addAction(rotate_counterclockwiseAction);
  positionMenu->addAction(rotate_180Action);
  positionMenu->addAction(flip_horizontalAction);
  positionMenu->addAction(flip_verticalAction);

  QToolButton* fileButton = new QToolButton();
  QToolButton* positionButton = new QToolButton();

  positionButton->setText("Position");
  positionButton->setPopupMode(QToolButton::InstantPopup);
  positionButton->setMenu(positionMenu);
  fileButton->setText("File");
  fileButton->setPopupMode(QToolButton::InstantPopup);
  fileButton->setMenu(fileMenu);

  // Adding action buttons to the toolbar
  toolbar->addWidget(fileButton);
  toolbar->addWidget(positionButton);
  toolbar->addAction(grayscaleAction);
  toolbar->addAction(blurAction);

  // Connecting buttons and actions
  connect(loadAction, &QAction::triggered, this, &MainAppWindow::loadImage);
  connect(grayscaleAction, &QAction::triggered, this,
          &MainAppWindow::applyGrayscale);
  connect(saveAction, &QAction::triggered, this, &MainAppWindow::saveImage);
  connect(blurAction, &QAction::triggered, this,
          &MainAppWindow::showBlurSlider);

  connect(rotate_clockwiseAction, &QAction::triggered, this,
          [this]() { rotateImage(cv::ROTATE_90_CLOCKWISE); });
  connect(rotate_counterclockwiseAction, &QAction::triggered, this,
          [this]() { rotateImage(cv::ROTATE_90_COUNTERCLOCKWISE); });
  connect(rotate_180Action, &QAction::triggered, this,
          [this]() { rotateImage(cv::ROTATE_180); });

  connect(flip_verticalAction, &QAction::triggered, this,
          [this]() { flipImage(1); });
  connect(flip_horizontalAction, &QAction::triggered, this,
          [this]() { flipImage(0); });
}
void MainAppWindow::loadImage() {
  QString fileName = QFileDialog::getOpenFileName(
      this, "Open Image", "", "Image (*.png *.jpg *.jpeg *.bmp)");

  if (!fileName.isEmpty()) {
    baseImage = cv::imread(fileName.toStdString());
    if (baseImage.empty()) {
      QMessageBox::warning(this, "Error", "Could not load image!");
      return;
    }
    currentProcessedImage = baseImage.clone();
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

  grayscale(currentProcessedImage);

  // Ensure deep copy
  imageLabel->setPixmap(cvMatToQPixmap(currentProcessedImage)
                            .scaled(imageLabel->size(), Qt::KeepAspectRatio,
                                    Qt::SmoothTransformation));
}

void MainAppWindow::resizeEvent(QResizeEvent*) {
  if (currentProcessedImage.empty()) return;
  imageLabel->setPixmap(cvMatToQPixmap(currentProcessedImage)
                            .scaled(imageLabel->size(), Qt::KeepAspectRatio,
                                    Qt::SmoothTransformation));
}

void MainAppWindow::rotateImage(int rotateCode) {
  cv::Mat input = currentProcessedImage.clone();
  rotate(input, currentProcessedImage, rotateCode);
  imageLabel->setPixmap(cvMatToQPixmap(currentProcessedImage)
                            .scaled(imageLabel->size(), Qt::KeepAspectRatio,
                                    Qt::SmoothTransformation));
}

void MainAppWindow::flipImage(int flipCode) {
  cv::Mat input = currentProcessedImage.clone();
  flip(input, currentProcessedImage, flipCode);
  imageLabel->setPixmap(cvMatToQPixmap(currentProcessedImage)
                            .scaled(imageLabel->size(), Qt::KeepAspectRatio,
                                    Qt::SmoothTransformation));
}

void MainAppWindow::brightness_contrast_Image(int a, int b) {
  QPixmap currentPixmap = imageLabel->pixmap(Qt::ReturnByValue);
  if (currentPixmap.isNull()) return;

  // Apply grayscale
  cv::Mat input = currentProcessedImage.clone();

  brightness_contrast(input, currentProcessedImage, a, b);

  // Ensure deep copy
  imageLabel->setPixmap(cvMatToQPixmap(currentProcessedImage)
                            .scaled(imageLabel->size(), Qt::KeepAspectRatio,
                                    Qt::SmoothTransformation));
}


void MainAppWindow::showBlurSlider() {
  if (!blurSlider) {
    blurSlider = new QSlider(Qt::Horizontal, this);
    blurSlider->setRange(1, 49);
    blurSlider->setValue(5);
    blurSlider->setGeometry(10, 40, 200, 30);
    blurSlider->show();
    connect(blurSlider, &QSlider::valueChanged, this, [this](int value) {
      if (baseImage.empty()) {
        qDebug() << "Base image is empty! Cannot apply blur.";
      }
      if (value % 2 == 0) ++value;

      cv::Mat output(baseImage.size(), baseImage.type());
      imageblur(baseImage, output, value);
      currentProcessedImage = output;
      imageLabel->setPixmap(cvMatToQPixmap(currentProcessedImage)
                                .scaled(imageLabel->size(), Qt::KeepAspectRatio,
                                        Qt::SmoothTransformation));
    });
  }
}

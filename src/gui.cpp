#include "gui.hpp"

#include <QAction>
#include <QApplication>
#include <QDebug>
#include <QFileDialog>
#include <QIntValidator>
#include <QLineEdit>
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
  QAction* loadAction = new QAction("Open...", this);
  QAction* grayscaleAction = new QAction("Grayscale", this);
  QAction* saveAction = new QAction("Save As...", this);
  QAction* blurAction = new QAction("Gaussian Blur", this);
  QAction* brightcontrastAction = new QAction("Brightness/Contrast");
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
  toolbar->addAction(brightcontrastAction);

  // Connecting buttons and actions
  connect(loadAction, &QAction::triggered, this, &MainAppWindow::loadImage);
  connect(grayscaleAction, &QAction::triggered, this,
          &MainAppWindow::applyGrayscale);
  connect(saveAction, &QAction::triggered, this, &MainAppWindow::saveImage);
  connect(blurAction, &QAction::triggered, this,
          &MainAppWindow::showBlurSlider);
  connect(brightcontrastAction, &QAction::triggered, this,
          &MainAppWindow::showBrightContrastSlider);
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

void MainAppWindow::showBrightContrastSlider() {
  if (!brightcontrastControlWidget) {
    brightcontrastControlWidget = new QFrame(this, Qt::Popup);
    QVBoxLayout* layout = new QVBoxLayout(brightcontrastControlWidget);
    QLabel* labelBr = new QLabel("Brightness:", brightcontrastControlWidget);
    QLabel* labelCr = new QLabel("Contrast:", brightcontrastControlWidget);
    brightSlider = new QSlider(Qt::Horizontal, brightcontrastControlWidget);
    contrastSlider=new QSlider(Qt::Horizontal, brightcontrastControlWidget);
    QLineEdit* brightLineEdit = new QLineEdit("5", brightcontrastControlWidget);
    QLineEdit* contrastLineEdit = new QLineEdit("5", brightcontrastControlWidget);

    brightLineEdit->setFixedWidth(40);
    brightLineEdit->setValidator(new QIntValidator(-49, 49, brightLineEdit));
    contrastLineEdit->setFixedWidth(40);
    contrastLineEdit->setValidator(new QIntValidator(1, 49, brightLineEdit));

    brightSlider->setRange(-49, 49);
    brightSlider->setValue(5);
    contrastSlider->setRange(1, 49);
    contrastSlider->setValue(5);

    QHBoxLayout* BrsliderLayout = new QHBoxLayout();
    QHBoxLayout* CrsliderLayout = new QHBoxLayout();
    BrsliderLayout->addWidget(brightSlider);
    BrsliderLayout->addWidget(brightLineEdit);
    CrsliderLayout->addWidget(contrastSlider);
    CrsliderLayout->addWidget(contrastLineEdit);

    layout->addWidget(labelBr);
    layout->addLayout(BrsliderLayout);
    layout->addWidget(labelCr);
    layout->addLayout(CrsliderLayout);

    connect(brightSlider, &QSlider::valueChanged, this,
            [this, brightLineEdit](int value) {
              brightLineEdit->setText(QString::number(value));
              if (baseImage.empty()) {
                qDebug() << "Base image is empty! Cannot apply brightness.";
                return;
              }

              cv::Mat output(baseImage.size(), baseImage.type());
              brightness_contrast(baseImage, output, 1, value);
              currentProcessedImage = output;
              imageLabel->setPixmap(cvMatToQPixmap(currentProcessedImage)
                                        .scaled(imageLabel->size(),
                                                Qt::KeepAspectRatio,
                                                Qt::SmoothTransformation));
            });

            connect(contrastSlider, &QSlider::valueChanged, this,
              [this, contrastLineEdit](int value) {
                contrastLineEdit->setText(QString::number(value));
                if (baseImage.empty()) {
                  qDebug() << "Base image is empty! Cannot apply contrast.";
                  return;
                }
  
                cv::Mat output(baseImage.size(), baseImage.type());
                brightness_contrast(baseImage, output, value, 0);
                currentProcessedImage = output;
                imageLabel->setPixmap(cvMatToQPixmap(currentProcessedImage)
                                          .scaled(imageLabel->size(),
                                                  Qt::KeepAspectRatio,
                                                  Qt::SmoothTransformation));
              });

    connect(brightLineEdit, &QLineEdit::editingFinished, this,
            [this, brightLineEdit]() {
              bool ok;
              int value = brightLineEdit->text().toInt(&ok);
              if (!ok) return;

              value = std::clamp(value, -49, 49);
              brightSlider->setValue(value);
            });
            connect(contrastLineEdit, &QLineEdit::editingFinished, this,
              [this, contrastLineEdit]() {
                bool ok;
                int value = contrastLineEdit->text().toInt(&ok);
                if (!ok) return;
  
                value = std::clamp(value, -49, 49);
                contrastSlider->setValue(value);
              });

            connect(brightcontrastControlWidget, &QWidget::destroyed, this, [this]() {
              brightcontrastControlWidget = nullptr;
              brightSlider = nullptr;
            });
  }
  brightcontrastControlWidget->setGeometry(10, 40, 250, 80);
  brightcontrastControlWidget->show();
}

void MainAppWindow::showBlurSlider() {
  if (!blurControlWidget) {
    blurControlWidget = new QFrame(this, Qt::Popup);
    QVBoxLayout* mainLayout = new QVBoxLayout(blurControlWidget);
    QLabel* label = new QLabel("Blur Depth:", blurControlWidget);
    blurSlider = new QSlider(Qt::Horizontal, blurControlWidget);
    QLineEdit* blurLineEdit = new QLineEdit("5", blurControlWidget);

    blurLineEdit->setFixedWidth(40);
    blurLineEdit->setValidator(new QIntValidator(0, 49, blurLineEdit));

    blurSlider->setRange(0, 49);
    blurSlider->setValue(5);

    QHBoxLayout* sliderLayout = new QHBoxLayout();
    sliderLayout->addWidget(blurSlider);
    sliderLayout->addWidget(blurLineEdit);

    mainLayout->addWidget(label);
    mainLayout->addLayout(sliderLayout);

    connect(blurSlider, &QSlider::valueChanged, this,
            [this, blurLineEdit](int value) {
              blurLineEdit->setText(QString::number(value));
              if (baseImage.empty()) {
                qDebug() << "Base image is empty! Cannot apply blur.";
                return;
              }

              cv::Mat output(baseImage.size(), baseImage.type());
              imageblur(baseImage, output, value * 2 + 1);
              currentProcessedImage = output;
              imageLabel->setPixmap(cvMatToQPixmap(currentProcessedImage)
                                        .scaled(imageLabel->size(),
                                                Qt::KeepAspectRatio,
                                                Qt::SmoothTransformation));
            });

    connect(blurLineEdit, &QLineEdit::editingFinished, this,
            [this, blurLineEdit]() {
              bool ok;
              int value = blurLineEdit->text().toInt(&ok);
              if (!ok) return;

              value = std::clamp(value, 0, 49);
              blurSlider->setValue(value);
            });
    connect(blurControlWidget, &QWidget::destroyed, this, [this]() {
      blurControlWidget = nullptr;
      blurSlider = nullptr;
    });
  }
  blurControlWidget->setGeometry(10, 40, 250, 80);
  blurControlWidget->show();
}
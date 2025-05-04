#ifndef GUI_HPP
#define GUI_HPP

#include <QLabel>
#include <QMainWindow>
#include <QPixmap>
#include <QSlider>

#include "proc.hpp"

QImage cvMatToQImage(const cv::Mat& mat);

class MainAppWindow : public QMainWindow {
  Q_OBJECT
 public:
  MainAppWindow();
  virtual ~MainAppWindow() = default;

 private slots:
  void loadImage();
  void applyGrayscale();
  void saveImage();
  void showBlurSlider();
  void rotateImage(int rotateCode);
  void showBrightContrastSlider();
  void flipImage(int flipCode);

 protected:
  void resizeEvent(QResizeEvent*);

 private:
  QLabel* imageLabel = nullptr;
  cv::Mat currentProcessedImage;
  QSlider* brightSlider = nullptr;
  QSlider* contrastSlider = nullptr;
  QSlider* blurSlider = nullptr;
  cv::Mat baseImage;
  QWidget* blurControlWidget = nullptr;
  QWidget* brightcontrastControlWidget = nullptr;
};

#endif
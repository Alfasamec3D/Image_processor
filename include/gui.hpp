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
  void brightness_contrast_Image(int a, int b);
  void flipImage(int flipCode);

 protected:
  void resizeEvent(QResizeEvent*);

 private:
  QLabel* imageLabel=nullptr;
  cv::Mat currentProcessedImage;
  QSlider* blurSlider=nullptr;
  cv::Mat baseImage;
};

#endif
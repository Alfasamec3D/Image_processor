#ifndef GUI_HPP
#define GUI_HPP

#include"proc.hpp"

#include <QLabel>
#include <QMainWindow>
#include <QPixmap>

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
  void applyBlur(int depth);
  void showBlurSlider();

 protected:
  void resizeEvent(QResizeEvent*);

 private:
  QLabel* imageLabel;
  cv::Mat currentProcessedImage;
};

#endif
#ifndef PROC_HPP
#define PROC_HPP
#include <QLabel>
#include <QMainWindow>
#include <QPixmap>
#include <opencv2/opencv.hpp>
QImage cvMatToQImage(const cv::Mat& mat);

cv::Mat load_image(const cv::String& filename);

void grayscale_section(cv::Mat& image, const int& start, const int& end);

void grayscale(cv::Mat& image, int numthreads);

void imageblur_section(cv::Mat& input, cv::Mat& output, int start, int end,
                       int depth);
void imageblur(cv::Mat& input, cv::Mat& output, int numthreads, int depth);

class MainAppWindow : public QMainWindow {
  Q_OBJECT
 public:
  MainAppWindow();
  virtual ~MainAppWindow() = default;
  
 private slots:
  void loadImage();
  void applyGrayscale();
  void saveImage();
  void applyBlur();

 protected:
  void resizeEvent(QResizeEvent*);

 private:
  QLabel* imageLabel;
  cv::Mat currentProcessedImage;
};

#endif
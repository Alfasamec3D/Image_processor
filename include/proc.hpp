#ifndef GEOM_H
#define GEOM_H
#include <opencv2/opencv.hpp>
#include <QPixmap>

QImage cvMatToQImage(const cv::Mat& mat);

cv::Mat load_image(const cv::String& filename);

void grayscale_section(cv::Mat& image, const int& start, const int& end);

void grayscale(cv::Mat& image, int numthreads);

class MainAppWindow: public QMainWIndow{
 Q_Object 
 public : 
 MainAppWindow();
 private slots:
  void loadImage();
  protected:
   void resizeEvent(QResizeEvent*);
   private:
    QLabel* imageLabel;
}
#endif
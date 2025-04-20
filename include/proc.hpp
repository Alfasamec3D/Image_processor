#ifndef PROC_HPP
#define PROC_HPP
#include <opencv2/opencv.hpp>
#include <QPixmap>
#include <QMainWindow>
#include <QLabel>
QImage cvMatToQImage(const cv::Mat& mat);

cv::Mat load_image(const cv::String& filename);

void grayscale_section(cv::Mat& image, const int& start, const int& end);

void grayscale(cv::Mat& image, int numthreads);
class MainAppWindow: public QMainWindow{
    Q_OBJECT
    public :
     MainAppWindow();
     virtual ~MainAppWindow()=default;
    private slots:
     void loadImage();
     void applyGrayscale();

    protected:
     void resizeEvent(QResizeEvent*);

    private:
     QLabel* imageLabel;
   };

#endif
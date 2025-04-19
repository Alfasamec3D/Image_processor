#ifndef GEOM_H
#define GEOM_H
#include <opencv2/opencv.hpp>
#include <QPixmap>
#include <QMainWindow>
#include <QLabel>
#include <QFileDialog>
#include<QAction>
#include <QToolBar>

QImage cvMatToQImage(const cv::Mat& mat);

cv::Mat load_image(const cv::String& filename);

void grayscale_section(cv::Mat& image, const int& start, const int& end);

void grayscale(cv::Mat& image, int numthreads);

class MainAppWindow: public QMainWindow{
 Q_OBJECT
 public : 
 MainAppWindow() {
    imageLabel = new QLabel(this);
    imageLabel->setAlignment(Qt::AlignCenter);
    setCentralWidget(imageLabel);
    QToolBar* toolbar = addToolBar("Toolbar");
    QAction* loadAction = new QAction("Load File", this);
  
    toolbar->addAction(loadAction);
    connect(loadAction, &QAction::triggered, this, &MainAppWindow::loadImage);
  }
 private slots:
  void loadImage()
  {
    QString fileName = QFileDialog::getOpenFileName(
        this, "Open Image", "", "Image (*.png *.jpg *.jpeg *.bmp)");
        if (!fileName.isEmpty())
        {
          QPixmap pix(fileName);
          imageLabel->setPixmap(pix.scaled(
              imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        }
  }
  protected:
  void resizeEvent(QResizeEvent *) override{
    if (!imageLabel->pixmap()) return;
    imageLabel->setPixmap(imageLabel->pixmap()->scaled(
        imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
  }
   private:
    QLabel* imageLabel;
};
#endif
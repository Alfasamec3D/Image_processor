#include"proc.hpp"
#include <QApplication>
#include <QFileDialog>
#include <QPushButton>
#include <QVBoxLayout>
#include <opencv2/core/types.hpp>
#include <opencv2/imgproc.hpp>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  QWidget window;
  QVBoxLayout layout;
  QLabel imageLabel;
  QPushButton button("Load & Grayscale");

  layout.addWidget(&imageLabel);
  layout.addWidget(&button);
  window.setLayout(&layout);

  QObject::connect(&button, &QPushButton::clicked, [&]() {
    QString path = QFileDialog::getOpenFileName();
    if (path.isEmpty()) return;
    cv::Mat image = cv::imread(path.toStdString());
    cv::Mat blur;
    cv::blur(image, blur, cv::Size{10, 10});
    //cv::blur(gray, image, cv::COLOR_GRAY2BGR);  // Convert back to 3 channels
    imageLabel.setPixmap(QPixmap::fromImage(cvMatToQImage(blur)));
  });

  window.show();
  return app.exec();
}
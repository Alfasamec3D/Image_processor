#include <QApplication>
#include "proc.hpp"

int main(int argc, char *argv[]) { 
  QApplication app(argc, argv);
  MainAppWindow window;
  window.resize(800, 600);
  window.show();
  return app.exec();
}
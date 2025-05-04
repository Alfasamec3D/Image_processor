#include <QApplication>
#include "gui.hpp"
#include <thread>

int main(int argc, char *argv[]) {
  QApplication::setStyle("Fusion");

  QPalette darkPalette;
  darkPalette.setColor(QPalette::Window, QColor(53, 53, 53));
  darkPalette.setColor(QPalette::WindowText, Qt::white);
  darkPalette.setColor(QPalette::Base, QColor(25, 25, 25));
  darkPalette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
  darkPalette.setColor(QPalette::Text, Qt::white);
  darkPalette.setColor(QPalette::Button, QColor(53,53,53));
  darkPalette.setColor(QPalette::ButtonText, Qt::white);             
  darkPalette.setColor(QPalette::BrightText, Qt::red);
  darkPalette.setColor(QPalette::Highlight, QColor(33, 150, 243));
  darkPalette.setColor(QPalette::HighlightedText, Qt::black);
  qApp->setPalette(darkPalette);

  QApplication app(argc, argv);
  MainAppWindow window;
  window.resize(800, 600);
  window.show();
  return app.exec();
}
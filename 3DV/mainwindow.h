#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QErrorMessage>
#include <QFileDialog>
#include <QMainWindow>
#include <QSettings>

#include "glview.h"
#include "qgifimage.h"

extern "C" {
#include "../s21_3DV.h"
}

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private slots:

  void on_openButton_clicked();
  void printpaint(QString filepath);
  void move();
  void saveSettings();
  void loadSettings();
  void createGif();

  void on_SliderSize_valueChanged(int value);

  void on_spinBox_sizeDot_valueChanged(int arg1);

  void on_spinBox_sizeLine_valueChanged(int arg1);

  void on_horizontalScrollBar_RotZ_valueChanged(int value);

  void on_doubleSpinBox_xMove_valueChanged(double arg1);

  void on_doubleSpinBox_yMove_valueChanged(double arg1);

  void on_doubleSpinBox_zMove_valueChanged(double arg1);

  void on_linecolor_clicked();

  void on_dotcolor_clicked();

  void on_bckgcolor_clicked();

  void on_saveJPEG_clicked();

  void on_saveBMP_clicked();

  void on_saveGIF_clicked();

  void on_comboBox_PointType_currentIndexChanged(int index);

  void on_comboBox_LineType_currentIndexChanged(int index);

  void on_comboBox_PlaneType_currentIndexChanged(int index);

private:
  double moveX = 0, moveY = 0, moveZ = 0;
  double moveXOld = 0, moveYOld = 0, moveZOld = 0;
  Ui::MainWindow *ui;
  QSettings *settings;
  QGifImage *gif_image = nullptr;
  QTimer *gif_timer = nullptr;
};
#endif // MAINWINDOW_H

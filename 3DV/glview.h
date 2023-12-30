#ifndef GLVIEW_H
#define GLVIEW_H

extern "C" {
#include "../s21_3DV.h"
}
#include <QColorDialog>
#include <QDoubleSpinBox>
#include <QDragEnterEvent>
#include <QMainWindow>
#include <QOpenGLWidget>
#include <QSlider>
#include <QTimer>
#include <QtOpenGL>

class glView : public QOpenGLWidget {
  Q_OBJECT
public:
    t_data obj =  {0, 0, NULL, NULL};
  QDoubleSpinBox *spinPointer[3];
  glView(QWidget *parent = nullptr);
  void getColrline(float *r, float *g, float *b, float *a);
  void setColrline(float r, float g, float b, float a);
  void getColrdot(float *r, float *g, float *b, float *a);
  void setColrdot(float r, float g, float b, float a);
  void getbckgc(float *r, float *g, float *b, float *a);
  void setbckgc(float r, float g, float b, float a);
  double ZoomSize = 0.01;
  int DotSize = 1;
  int LineSize = 1;
  int boxVertexInd = 2, boxFacetsInd = 0, boxProjectionInd = 0;
  float rcl = 0, gcl = 0, bcl = 1, acl = 1;   //линии
  float rcd = 0, gcd = 0, bcd = 1, acd = 1;   //точки
  float rbc = 1.1, gbc = 0, bbc = 1, abc = 1; //фон
  float zRot = 0, xRot = 0, yRot = 0;
  float xRotOld = 0.0, yRotOld = 0.0;
  int PointType = 0;
  int LineType = 0;
  int PlaneType = 0;

private:
  QPoint mPos;

  void paintGL() override;
  void initializeGL() override;
  void mousePressEvent(QMouseEvent *) override;
  void mouseMoveEvent(QMouseEvent *) override;
};

#endif // GLVIEW_H

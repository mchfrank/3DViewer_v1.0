#include "glview.h"

glView::glView(QWidget *parent) : QOpenGLWidget(parent) {
  setAcceptDrops(true);
  this->obj.count_of_facets = 0;
  this->obj.count_of_vertexes = 0;
}

void glView::initializeGL() {
  glClearColor(rbc, gbc, bbc, abc);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
}

void glView::paintGL() {
  glClearColor(rbc, gbc, bbc, abc); // цвет фона
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  if (PlaneType == 1) { // вроде рабочая проекция (хз какая  именно) и хз
                        // правильно ли все в целом
    glFrustum(-1, 1, -1, 1, 3, 10);
    glTranslatef(0.0, 0.0, -5.0);
  }

  glScalef(ZoomSize, ZoomSize, ZoomSize); // размер отрисовки

  /*повороты*/
  glRotatef(xRot, 1, 0, 0);
  glRotatef(yRot, 0, 1, 0);
  glRotatef((12 / M_PI * zRot), 0, 0, 1);

  glVertexPointer(3, GL_DOUBLE, 0, this->obj.vertex);
  glEnableClientState(GL_VERTEX_ARRAY);

  glPointSize(DotSize); // размер точек

  if (PointType == 1)
    glEnable(GL_POINT_SMOOTH); // круглые точки
  if (PointType == 0)
    glDisable(GL_POINT_SMOOTH); // квадратные точки

  if (this->boxVertexInd != 0) {
    glColor4f(rcd, gcd, bcd, acd); // цвет точек
  }

  if (PointType != 2) { // без точек (если 2)
    glDrawArrays(GL_POINTS, 1, this->obj.count_of_vertexes);
  }

  glLineWidth(LineSize); // размер линий
  if (LineType == 1) {   // пунктирные линии
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(2, 0x00FF00FF);
  }

  if (LineType == 0) { // сплошные линии
    glDisable(GL_LINE_SMOOTH);
    glDisable(GL_LINE_STIPPLE);
  }

  glColor4f(rcl, gcl, bcl, acl); // цвет линий

  if (LineType != 2) { // без линий (если 2)
    for (unsigned int i = 1; i < this->obj.count_of_facets + 1; i++) {
      glDrawElements(GL_LINE_LOOP,
                     this->obj.polygons[i].numbers_of_vertexes_in_facets,
                     GL_UNSIGNED_INT, this->obj.polygons[i].vertexes);
    }
  }
  glDisableClientState(GL_VERTEX_ARRAY);
}

void glView::mousePressEvent(QMouseEvent *mo) {
  mPos = mo->pos();
  xRotOld = xRot;
  yRotOld = yRot;
}

void glView::mouseMoveEvent(QMouseEvent *mo) {
  xRot = (1 / M_PI * (mo->pos().y() - mPos.y()));
  yRot = (1 / M_PI * (mo->pos().x() - mPos.x()));

  xRot += xRotOld;
  yRot += yRotOld;
  update();
}

void glView::getColrline(float *r, float *g, float *b, float *a) {
  *r = this->rcl;
  *g = this->gcl;
  *b = this->bcl;
  *a = this->acl;
}

void glView::setColrline(float r, float g, float b, float a) {
  this->rcl = r;
  this->gcl = g;
  this->bcl = b;
  this->acl = a;
  update();
}

void glView::getColrdot(float *r, float *g, float *b, float *a) {
  *r = this->rcd;
  *g = this->gcd;
  *b = this->bcd;
  *a = this->acd;
}

void glView::setColrdot(float r, float g, float b, float a) {
  this->rcd = r;
  this->gcd = g;
  this->bcd = b;
  this->acd = a;
  update();
}

void glView::setbckgc(float r, float g, float b, float a) {
  this->rbc = r;
  this->gbc = g;
  this->bbc = b;
  this->abc = a;
  update();
}

void glView::getbckgc(float *r, float *g, float *b, float *a) {
  *r = this->rbc;
  *g = this->gbc;
  *b = this->bbc;
  *a = this->abc;
}

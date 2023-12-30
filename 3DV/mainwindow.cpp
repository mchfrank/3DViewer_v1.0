#include "mainwindow.h"

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), gif_timer(new QTimer) {
  ui->setupUi(this);

  connect(gif_timer, SIGNAL(timeout()), this, SLOT(createGif()));
  connect(ui->saveGIF, &QPushButton::clicked, this,
          &MainWindow::on_saveGIF_clicked);

  connect(ui->saveGIF, &QPushButton::clicked, this,
          &MainWindow::on_saveGIF_clicked);

  settings = new QSettings(QApplication::applicationDirPath() + "/settings.ini", QSettings::IniFormat, this);
  loadSettings();
}

MainWindow::~MainWindow() {
  saveSettings();
  delete settings;
  if (ui->widget->obj.vertex != NULL) remove_struct(&ui->widget->obj);
  if (gif_image) {
      delete gif_image;
      delete gif_timer;
  }
  delete ui;
}

void MainWindow::saveSettings() {
  float colorSet[4];
  ui->widget->getColrdot(&colorSet[0], &colorSet[1], &colorSet[2],
                         &colorSet[3]);
  for (int i = 0; i < 4; i++) {
    QString tmp = "Point Color " + QString::number(i);
    settings->setValue(tmp, colorSet[i]);
  }

  ui->widget->getbckgc(&colorSet[0], &colorSet[1], &colorSet[2], &colorSet[3]);
  for (int i = 0; i < 4; i++) {
    QString tmp = "Bg Color " + QString::number(i);
    settings->setValue(tmp, colorSet[i]);
  }

  ui->widget->getColrline(&colorSet[0], &colorSet[1], &colorSet[2],
                          &colorSet[3]);
  for (int i = 0; i < 4; i++) {
    QString tmp = "Line Color " + QString::number(i);
    settings->setValue(tmp, colorSet[i]);
  }

  settings->setValue("title", windowTitle());
  settings->setValue("geometry", geometry());
  settings->setValue("size", ui->SliderSize->sliderPosition());
  settings->setValue("sizeLine", ui->spinBox_sizeLine->value());
  settings->setValue("sizeDot", ui->spinBox_sizeDot->value());
  settings->setValue("PositionRotZ",
                     ui->horizontalScrollBar_RotZ->sliderPosition());
  settings->setValue("fileName", ui->FileName->text());
  settings->setValue("xRotOld", ui->widget->xRotOld);
  settings->setValue("yRotOld", ui->widget->yRotOld);
  settings->setValue("xRot", ui->widget->xRot);
  settings->setValue("yRot", ui->widget->yRot);
  settings->setValue("moveX", ui->doubleSpinBox_xMove->value());
  settings->setValue("moveY", ui->doubleSpinBox_yMove->value());
  settings->setValue("moveZ", ui->doubleSpinBox_zMove->value());
  settings->setValue("typePoint", ui->comboBox_PointType->currentIndex());
  settings->setValue("typeLine", ui->comboBox_LineType->currentIndex());
  settings->setValue("typePlane", ui->comboBox_PlaneType->currentIndex());
}

void MainWindow::loadSettings() {
  setWindowTitle(settings->value("titele", "3D_Viewer").toString());
  setGeometry(settings->value("geometry", QRect(200, 200, 1740, 900)).toRect());
  ui->SliderSize->setSliderPosition(settings->value("size", 1).toInt());
  ui->spinBox_sizeLine->setValue(settings->value("sizeLine", 1).toInt());
  ui->spinBox_sizeDot->setValue(settings->value("sizeDot", 1).toInt());
  ui->horizontalScrollBar_RotZ->setSliderPosition(
      settings->value("PositionRotZ", 1).toInt());
  ui->FileName->setText(settings->value("fileName", "").toString());
  ui->widget->xRotOld = settings->value("xRotOld", 0).toFloat();
  ui->widget->yRotOld = settings->value("yRotOld", 0).toFloat();
  ui->widget->xRot = settings->value("xRot", 0).toFloat();
  ui->widget->yRot = settings->value("yRot", 0).toFloat();
  ui->doubleSpinBox_xMove->setValue(settings->value("moveX", 0).toDouble());
  ui->doubleSpinBox_yMove->setValue(settings->value("moveY", 0).toDouble());
  ui->doubleSpinBox_zMove->setValue(settings->value("moveZ", 0).toDouble());
  ui->comboBox_PointType->setCurrentIndex(
      settings->value("typePoint", 0).toInt());
  ui->comboBox_LineType->setCurrentIndex(
      settings->value("typeLine", 0).toInt());
  ui->comboBox_PlaneType->setCurrentIndex(
      settings->value("typePlane", 0).toInt());
  ui->widget->PointType = settings->value("typePoint", 0).toInt();
  ui->widget->LineType = settings->value("typeLine", 0).toInt();
  ui->widget->PlaneType = settings->value("typePlane", 0).toInt();
  moveYOld = 0;
  moveXOld = 0;
  moveZOld = 0;
  if (ui->FileName->text() != "\0") {
    printpaint(ui->FileName->text());
    move();
  }
  float colorSet[4];
  for (int i = 0; i < 4; i++) {
    QString tmp = "Point Color " + QString::number(i);
    colorSet[i] = settings->value(tmp, 1).toFloat();
  }

  ui->widget->setColrdot(colorSet[0], colorSet[1], colorSet[2], colorSet[3]);
  for (int i = 0; i < 4; i++) {
    QString tmp = "Bg Color " + QString::number(i);
    colorSet[i] = settings->value(tmp, 0.262882).toFloat();
  }
  ui->widget->setbckgc(colorSet[0], colorSet[1], colorSet[2], colorSet[3]);
  for (int i = 0; i < 4; i++) {
    QString tmp = "Line Color " + QString::number(i);
    colorSet[i] = settings->value(tmp, 1).toFloat();
  }
  ui->widget->setColrline(colorSet[0], colorSet[1], colorSet[2], colorSet[3]);
}

void MainWindow::on_openButton_clicked() {
  QString str, oldStr;
  if (ui->widget->obj.vertex != NULL) remove_struct(&ui->widget->obj);
  oldStr = ui->FileName->text();
  str = QFileDialog::getOpenFileName(this, "", "", tr("obj (*.obj)"));
  ui->FileName->setText(str);
  moveXOld = 0;
  moveYOld = 0;
  moveZOld = 0;
  if (ui->FileName->text() == "\0") {
    (new QErrorMessage(this))->showMessage("Файл не выбран");
    ui->FileName->setText(oldStr);
  } else {
    printpaint(str);
    move();
  }
}

void MainWindow::printpaint(QString filepath) {
  ui->widget->obj.count_of_vertexes = 0;
  ui->widget->obj.count_of_facets = 0;
  QByteArray a = filepath.toLocal8Bit();
  char *aa = a.data();
  int error = pars(aa, &ui->widget->obj);
  if (error == 1)
    (new QErrorMessage(this))->showMessage("Ошибка выделения памяти");
  if (error == 2)
    (new QErrorMessage(this))
        ->showMessage(
            "Присутвуют отрицательные индексы, невозможно построить полигоны. "
            "Будут построены только вершины");
  if (error == 3)
    (new QErrorMessage(this))->showMessage("Не корректный/пустой файл");
  ui->countVertex->setText(QString::number(ui->widget->obj.count_of_vertexes));
  ui->countFacets->setText(QString::number(ui->widget->obj.count_of_facets));
  ui->widget->update();
}

void MainWindow::move() {
  move_matrix(moveX - moveXOld, moveY - moveYOld, moveZ - moveZOld,
              &ui->widget->obj);
  moveXOld = moveX;
  moveYOld = moveY;
  moveZOld = moveZ;
  ui->widget->update();
}

void MainWindow::on_SliderSize_valueChanged(int value) {
  ui->widget->ZoomSize = (double)value / 1000;
  ui->widget->update();
}

void MainWindow::on_spinBox_sizeDot_valueChanged(int arg1) {
  ui->widget->DotSize = arg1;
  ui->widget->update();
}

void MainWindow::on_spinBox_sizeLine_valueChanged(int arg1) {
  ui->widget->LineSize = arg1;
  ui->widget->update();
}

void MainWindow::on_horizontalScrollBar_RotZ_valueChanged(int value) {
  ui->widget->zRot = (float)value;
  ui->widget->update();
}

void MainWindow::on_doubleSpinBox_xMove_valueChanged(double arg1) {
  moveX = arg1;
  move();
}

void MainWindow::on_doubleSpinBox_yMove_valueChanged(double arg1) {
  moveY = arg1;
  move();
}

void MainWindow::on_doubleSpinBox_zMove_valueChanged(double arg1) {
  moveZ = arg1;
  move();
}

void MainWindow::on_linecolor_clicked() {
  float r, g, b, a;
  ui->widget->getColrline(&r, &g, &b, &a);
  QColor tmp;
  tmp.setRgbF(r, g, b, a);
  QColor color = QColorDialog::getColor(tmp);
  color.getRgbF(&r, &g, &b, &a);
  ui->widget->setColrline(r, g, b, a);
}

void MainWindow::on_dotcolor_clicked() {
  float r, g, b, a;
  ui->widget->getColrdot(&r, &g, &b, &a);
  QColor tmp;
  tmp.setRgbF(r, g, b, a);
  QColor color = QColorDialog::getColor(tmp);
  color.getRgbF(&r, &g, &b, &a);
  ui->widget->setColrdot(r, g, b, a);
}

void MainWindow::on_bckgcolor_clicked() {
  float r, g, b, a;
  ui->widget->getbckgc(&r, &g, &b, &a);
  QColor tmp;
  tmp.setRgbF(r, g, b, a);
  QColor color = QColorDialog::getColor(tmp);
  color.getRgbF(&r, &g, &b, &a);
  ui->widget->setbckgc(r, g, b, a);
}

void MainWindow::on_saveJPEG_clicked() {
  QImage img = ui->widget->grabFramebuffer();
  QString dirname =
      QFileDialog::getSaveFileName(this, "Выбeрите папку", "/", ".jpeg");
  img.save(dirname + ".jpeg", "JPEG");
}

void MainWindow::on_saveBMP_clicked() {
  QImage img = ui->widget->grabFramebuffer();
  QString dirname =
      QFileDialog::getSaveFileName(this, "Выбeрите папку", "/", ".bmp");
  img.save(dirname + ".bmp", "BMP");
}

void MainWindow::on_saveGIF_clicked() {
  if (ui->saveGIF->text() == "Сохранить gif") {
    ui->saveGIF->setText("Запись...");
    gif_image = new QGifImage;
    gif_image->setDefaultDelay(100);
    gif_timer->start(100);
  }
}

void MainWindow::createGif() {
  if (gif_image->frameCount() < 50) {
    switch (gif_image->frameCount()) {
    case 10:
      ui->saveGIF->setText("5 sec...");
      break;
    case 20:
      ui->saveGIF->setText("4 sec...");
      break;
    case 30:
      ui->saveGIF->setText("3 sec...");
      break;
    case 40:
      ui->saveGIF->setText("2 sec...");
      break;
    case 49:
      ui->saveGIF->setText("1 sec...");
      break;
    }
    QImage frame = ui->widget->grabFramebuffer();
    gif_image->addFrame(
        frame.scaled(640, 480, Qt::IgnoreAspectRatio, Qt::SmoothTransformation),
        0);
    return;
  }
  gif_timer->stop();
  ui->saveGIF->setText("Сохранить gif");
  QString dirname =
      QFileDialog::getSaveFileName(this, "Выбeрите папку", "/", ".gif");
  gif_image->save(dirname + ".gif");
}

void MainWindow::on_comboBox_PointType_currentIndexChanged(int index) {
  ui->widget->PointType = index;
  ui->widget->update();
}

void MainWindow::on_comboBox_LineType_currentIndexChanged(int index) {
  ui->widget->LineType = index;
  ui->widget->update();
}

void MainWindow::on_comboBox_PlaneType_currentIndexChanged(int index) {
  ui->widget->PlaneType = index;
  ui->widget->update();
}

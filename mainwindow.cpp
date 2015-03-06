#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <string>
#include "paintarea.h"


MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  pPaintArea = new PaintArea(this);
  //this->setCentralWidget(pPaintArea);
  pPaintArea->setGeometry(0, 35, 410, 410);
}

MainWindow::~MainWindow()
{
  delete pPaintArea;
  delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    pPaintArea->show();
}

void MainWindow::on_pushButton_clicked()
{
  // test : draw one kml rect
  std::string testfilepath1 = "E:/GitHub/test_data/test3.kml";
  std::string testfilepath2 = "E:/GitHub/test_data/test4.kml";
  pPaintArea->registerKmlFile(testfilepath1, drawObject::area);
  pPaintArea->registerKmlFile(testfilepath2, drawObject::area);
  pPaintArea->update();

}

void MainWindow::on_pushButton_2_clicked()
{
  QRectF* pRect = new QRectF(42.0, 128.0, 10, 10);
  pPaintArea->registerData((void*)(pRect), (drawObject::ShapeType)2);
  pPaintArea->update();
}

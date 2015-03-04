#include "myframe.h"
#include <QFrame>
#include "drawobject.h"
#include <QPainter>
#include <ogrsf_frmts.h>
#include "kmlfilereader.h"

MyFrame::MyFrame(QWidget *parent, Qt::WindowFlags f) :
  QFrame(parent, f)
{
  setPalette(QPalette(Qt::white));
  setAutoFillBackground(true);
}

void MyFrame::paintEvent(QPaintEvent *event)
{
  QPainter painter(this);
  drawAirPlane airplane;
  airplane.draw(painter, 100, 100);

//  KmlFileReader reader;
//  std::string strFile = "E:/work_code/qt_code/test_data/test3.kml";
//  OGRPolygon polygon;
//  reader.getPolygonCoordinate(strFile, polygon);
//  OGREnvelope pEnvelope;
//  polygon.getEnvelope(&pEnvelope);
//  int minX = int(pEnvelope.MinX - 1);
//  int maxY = int(pEnvelope.MaxY + 1);
//  painter.setWindow(minX, maxY,
//                    painter.viewport().width(),
//                    painter.viewport().height());

}

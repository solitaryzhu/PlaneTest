#include "paintarea.h"
#include <QPainter>
#include "kmlfilereader.h"
#include <ogr_api.h>

PaintArea::PaintArea(QWidget *parent) :
  QWidget(parent)
{
  setPalette(QPalette(Qt::white));
  setAutoFillBackground(true);
  setFixedSize(400, 400);

  // initialize the drawobject
  drawObject* p;
  p = new drawGeographicalPoint();
  //listDrawObject.push_back(p);
  mapDrawObject.insert(std::make_pair(drawObject::point, p));
  p = new drawGeographicalLine();
  //listDrawObject.push_back(p);
  mapDrawObject.insert(std::make_pair(drawObject::line, p));
  p = new drawGeographicalPolygon();
  //listDrawObject.push_back(p);
  mapDrawObject.insert(std::make_pair(drawObject::area, p));
}

PaintArea::~PaintArea()
{
  std::map<drawObject::ShapeType, drawObject*>::iterator it;
  for(it = mapDrawObject.begin(); it != mapDrawObject.end(); ++it)
  {
      delete (it->second);
  }
  drawObject::releaseShape();
}

void PaintArea::paintEvent(QPaintEvent *event)
{
  QPainter painter(this);
//  drawAirPlane airplane;
//  airplane.draw(painter, 100, 100);
//  drawGeographicalPolygon polygon;
//  QRect rect;
//  rect.setBottomLeft(QPoint(30, 50));
//  rect.setTopRight(QPoint(40, 60));
//  polygon.draw(painter, rect);
  drawObject::setGeoCoordSys(painter);
  std::map<drawObject::ShapeType, drawObject*>::iterator it;
  for(it = mapDrawObject.begin(); it != mapDrawObject.end(); ++it)
  {
	  drawObject* pDraw = it->second;
	  pDraw->draw(painter);
  }
}

void PaintArea::registerData(void *pVoid, drawObject::ShapeType type)
{
    drawObject::registerShape(pVoid, type);
}

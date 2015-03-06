#include "paintarea.h"
#include <QPainter>
#include <ogr_api.h>
#include "cogrgeometryfilereader.h"

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

void PaintArea::registerKmlFile(std::string filePath, drawObject::ShapeType type)
{
  if(filePath.empty())
    return;

  void* pData = 0;

  if(type == drawObject::area)
  {
      std::auto_ptr<OGRGeometry> region =
          COGRGeometryFileReader::GetFirstOGRPolygonFromFile(filePath);
      OGRPolygon* pRegion = dynamic_cast<OGRPolygon*>(region.get());

      /* convert the region to polygon */
      OGRLinearRing* pRing = pRegion->getExteriorRing();
      QVector<QPointF> vtr;
      for(int i=0; i<pRing->getNumPoints(); ++i)
      {
          OGRPoint point;
          pRing->getPoint(i, &point);
          vtr.push_back(QPointF(point.getX(), point.getY()));
      }

      pData = new QPolygonF(vtr);
  }
  else // only support the area file for now
    return;

  registerData(pData, type);
}

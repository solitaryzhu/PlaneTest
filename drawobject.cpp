#include "drawobject.h"
#include <qdebug.h>
#include <QPainter>
#include "myrectf.h"
#include "mypolygon.h"
#include <ogrsf_frmts.h>
#include "kmlfilereader.h"


QRectF drawObject::geoRect;
double drawObject::scale = .0;
std::list<shapeData*> drawObject::listShapeData;
std::list<GuidancePoint*> drawObject::listGuidancePoints;

void drawObject::registerPoint(QRectF &rect,
                               QPointF point)
{
  qreal x1, y1, x2, y2;
  rect.getCoords(&x1, &y1, &x2, &y2);
  if(y1 < point.y())
  {
    rect.setCoords(x1, point.y(), x2, y2);
  }

  rect.getCoords(&x1, &y1, &x2, &y2);
  if(x2 < point.x())
  {
      rect.setCoords(x1, y1, point.x(), y2);
  }
}

void drawObject::releaseShape()
{
	std::list<shapeData*>::iterator it = listShapeData.begin();
	std::list<shapeData*>::iterator itEnd = listShapeData.end();
	for (; it != itEnd; it++)
	{
		shapeData* p = *it;
		delete p;
		p = 0;
	}
}

void drawObject::registerShape(void *pVoid, ShapeType t, int nNums)
{
  // recalculate the geoRect
  if(ShapeType::point == t)
  {
    QPointF* pPoint = (QPointF*)pVoid;
    if(!geoRect.contains(*pPoint))
    {
      registerPoint(geoRect, *pPoint);
    }
  }
  else if(ShapeType::line == t)
  {
    QLineF* pLine = (QLineF*)pVoid;
    QPointF p1 = pLine->p1();
    QPointF p2 = pLine->p2();
    if(!geoRect.contains(p1)){ registerPoint(geoRect, p1);}
    if(!geoRect.contains(p2)){ registerPoint(geoRect, p2);}
  }
  else if(area == t)
  {
    QPolygonF* pPolygon = (QPolygonF*)pVoid;
    QRectF rect = pPolygon->boundingRect();
    if(!geoRect.contains(rect)){ geoRect = geoRect | rect;}
  }
  else if(polyline == t)
  {
      QPointF* pPoints = (QPointF*)pVoid;
      for(int i=0; i<nNums; ++i){ registerPoint(geoRect, pPoints[i]);}
  }
  // recalculate the scale
  double xScale = 400/geoRect.width();
  double yScale = 400/geoRect.height();
  scale = ((xScale<yScale)?xScale:yScale);
  // save the data
  shapeData* pData = new shapeData(pVoid, t, nNums);
  listShapeData.push_back(pData);
}

void drawObject::setGeoCoordSys(QPainter &painter)
{
  // view coordinate system and geographical coordinate system
	painter.setWindow(0, 400, 400, -400);
}

void drawAirPlane::draw(QPainter &painter,
                        int x,
                        int y,
                        double scale)
{
  Q_ASSERT(scale < 1);

  int _x = -x;
  int _y = -y;
  double _scale = painter.viewport().width()*scale/40;

  painter.setWindow(_x, _y,
                    painter.viewport().width(),
                    painter.viewport().height());

  // draw an airplane
  QBrush PlaneBrush(QColor(0, 100, 0));
  painter.setBrush(PlaneBrush);
  painter.setPen(Qt::NoPen);
  // 1st draw one horizontal rectangle
  MyRectF rect1(QPointF(-20.0, -5.0), QPointF(20.0, 5.0));
  rect1.scaleVle(_scale);
  painter.drawRoundRect(rect1.getCore(), 50, 50);
  // 2nd draw one vertical rectangle
  MyRectF rect2(QPointF(-5.0, -15.0), QPointF(5, 15.0));
  rect2.scaleVle(_scale);
  painter.drawRoundRect(rect2.getCore(), 50, 50);
  // 3nd draw another horizontal rectangle
  MyRectF rect3(QPointF(-10.0, 11.0), QPointF(10.0, 21));
  rect3.scaleVle(_scale);
  painter.drawRoundRect(rect3.getCore(), 50, 50);

  painter.setWindow(0, 0,
                    painter.viewport().width(),
                    painter.viewport().height());
}

void drawGeographicalPoint::draw(QPainter &painter)
{
  // the key of drawing geographical coordinate
  // is the translation of coordinate system
}

void drawGeographicalLine::draw(QPainter &painter)
{

}

void drawGeographicalPolyline::draw(QPainter &painter)
{
    std::list<shapeData*>::iterator it;
    for(it = listShapeData.begin(); it != listShapeData.end(); ++it)
    {
        shapeData* pData = *it;
        if(0 != pData->pVoid && pData->type == polyline)
        {
            QPointF* pPoints = (QPointF*)(pData->pVoid);
            mypolylineptr ptr;
            ptr.setCore(pPoints, pData->nNumofShape);
            ptr.optimizeCoord(geoRect.topLeft(), scale);
            painter.drawPolyline(ptr.getCore(), pData->nNumofShape);
        }
    }
}

void drawGeographicalPolygon::draw(QPainter &painter)
{
  QBrush PlaneBrush(QColor(0, 100, 0));
  painter.setBrush(PlaneBrush);
  painter.setPen(Qt::NoPen);

  std::list<shapeData*>::iterator it;
  for (it = listShapeData.begin(); it != listShapeData.end(); ++it)
  {
      shapeData* pData = *it;
      if(0 != pData->pVoid && pData->type == area)
        {
          QPolygonF* pPolygon = (QPolygonF*)(pData->pVoid);
          mypolygon polygon;
          polygon.setCore(*pPolygon);
          polygon.optimizeCoord(geoRect.topLeft(), scale);
          painter.drawPolygon(polygon.getCore());
        }
  }
}

void drawObject::registerGuidancePoint(GuidancePoint *pPoint)
{
    listGuidancePoints.push_back(pPoint);
}

void drawObject::releaseGuidancePoint()
{
    for(std::list<GuidancePoint*>::iterator it = listGuidancePoints.begin();
        it != listGuidancePoints.end(); ++it)
    {
        delete (*it);
    }
}

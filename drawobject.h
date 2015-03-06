#ifndef DRAWOBJECT_H
#define DRAWOBJECT_H

#include <QString>
#include <QRect>
#include <QMatrix>
#include <list>

class QPainter;
class shapeData;

class drawObject
{
public:
  virtual void draw(QPainter& painter) = 0;
  /*
   * point means
  */
  enum ShapeType{point, line, area, guidance};
  static void setGeoCoordSys(QPainter& painter);
  // recalculate the scale, the geoRect and save the data
  static void registerShape(void* pVoid, ShapeType t);
  static void releaseShape();
protected:
  static void registerPoint(QRectF& rect, QPointF point);
  static QRectF geoRect;
  static double scale;
  static std::list<shapeData*> listShapeData;
};

class shapeData
{ 
public:
	shapeData(void* _data, drawObject::ShapeType t){ pVoid = _data; type = t; }
	drawObject::ShapeType type; void* pVoid; 
};

class drawGeographicalPoint : public drawObject
{
public:
  void draw(QPainter& painter);
};

class drawGeographicalLine : public drawObject
{
public:
  void draw(QPainter &painter);
};

class drawGeographicalPolygon : public drawObject
{
public:
  void draw(QPainter& painter);
};

class drawAirPlane
{
public:
  drawAirPlane(){}

  // (x,y) is the point of view coordinate
  // scale = width of plane / width of window
  void draw(QPainter& painter,
            int x,
            int y,
            double scale = 0.1);
};

#endif // DRAWOBJECT_H

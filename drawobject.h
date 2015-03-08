#ifndef DRAWOBJECT_H
#define DRAWOBJECT_H

#include <QString>
#include <QRect>
#include <QMatrix>
#include <list>

class QPainter;
class shapeData;

typedef struct GuidancePoint
{
    enum GuidancePointType
    {
        AirPort,
        A1Type,
        A2Type,
        B1Type,
        B2Type,
        Normal
    };
    GuidancePoint(GuidancePointType t, QPointF p, int lVle=0, int pVle=0):
        type(t),point(p), nLineNum(lVle), nPointNum(pVle){};
    int nLineNum; // belong to which line
    int nPointNum; // the sequence number
    GuidancePointType type; // airport or A1 or A2 or B1 or B2
    QPointF point; // the coordinate
} GP;

class drawObject
{
public:
  virtual void draw(QPainter& painter) = 0;
  /*
   * point represents by (x, y, z)
   * line represents by two points
   * area represents by serious points
   * guidance represents by serious points who have direction
  */
  enum ShapeType{point, line, area, guidance, polyline};

  static void setGeoCoordSys(QPainter& painter);
  // recalculate the scale, the geoRect and save the data
  static void registerShape(void* pVoid, ShapeType t, int nNums = 1);
  static void releaseShape();

  static void registerGuidancePoint(GuidancePoint* pPoint);
  static void releaseGuidancePoint();
protected:
  static void registerPoint(QRectF& rect, QPointF point);
  static QRectF geoRect;
  static double scale;
  static std::list<shapeData*> listShapeData;
  static std::list<GuidancePoint*>  listGuidancePoints;
};

class shapeData
{ 
public:
    shapeData(void* _data, drawObject::ShapeType t, int nNums = 1)
    { pVoid = _data; type = t; nNumofShape = nNums; }

    drawObject::ShapeType type;
    void* pVoid;
    int nNumofShape;
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

class drawGeographicalPolyline : public drawObject
{
public:
    void draw(QPainter &painter);
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

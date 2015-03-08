#ifndef MYPOLYGON_H
#define MYPOLYGON_H

#include <QPolygonF>
#include <QPointF>

class mypolygon
{
public:
  mypolygon();
  const QPolygonF getCore(){ return _core; }
  void setCore(const QPolygonF& core){ _core = core; }
  void optimizeCoord(QPointF _trans, double _scale);

private:
  QPolygonF _core;
};

class mypolylineptr
{
public:
    mypolylineptr();
    const QPointF* getCore(int& num)
    { num = nNumofPoints; return _core; }
    const QPointF* getCore(){ return _core; }
    void setCore(QPointF* ptr, int num)
    { _core = ptr; nNumofPoints = num; }
    void optimizeCoord(QPointF _trans, double _scale);

private:
    QPointF* _core;
    int nNumofPoints;
};

#endif // MYPOLYGON_H

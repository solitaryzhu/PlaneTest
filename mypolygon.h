#ifndef MYPOLYGON_H
#define MYPOLYGON_H

#include <QPolygonF>

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

#endif // MYPOLYGON_H

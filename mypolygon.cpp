#include "mypolygon.h"

mypolygon::mypolygon()
{
  _core = QPolygonF();
}

void mypolygon::optimizeCoord(QPointF _trans, double _scale)
{
  for(QPolygonF::iterator it = _core.begin();
      it != _core.end(); ++it)
  {
      it->operator -=(_trans);
      it->operator *=(_scale);
  }
}

mypolylineptr::mypolylineptr()
{ _core = 0; }

void mypolylineptr::optimizeCoord(QPointF _trans, double _scale)
{
    for(int i=0; i<nNumofPoints; ++i)
    {
        _core[i] -= _trans;
        _core[i] *= _scale;
    }
}

#ifndef PAINTAREA_H
#define PAINTAREA_H

#include <QWidget>
//#include <list>
#include <map>
#include "drawobject.h"


class PaintArea : public QWidget
{
  Q_OBJECT
public:
  explicit PaintArea(QWidget *parent = 0);
  ~PaintArea();
  void registerData(void* pVoid, drawObject::ShapeType type);

protected:
  void paintEvent(QPaintEvent* event);

signals:

public slots:

private:
  //std::list<drawObject*> listDrawObject;
  std::map<drawObject::ShapeType, drawObject*> mapDrawObject;

};

#endif // PAINTAREA_H

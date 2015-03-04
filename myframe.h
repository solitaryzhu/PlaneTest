#ifndef MYFRAME_H
#define MYFRAME_H

#include <QFrame>

class MyFrame : public QFrame
{
  Q_OBJECT
public:
  MyFrame(QWidget* parent=0, Qt::WindowFlags f=0);
  void paintEvent(QPaintEvent* event);

signals:

public slots:

};

#endif // MYFRAME_H

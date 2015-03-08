#include "paintarea.h"
#include <QPainter>
#include <fstream>
#include <sstream>
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

void PaintArea::registerGhtFile(std::string filePath)
{
    if(filePath.empty())
        return;

    QList<QPointF> lsPoints;
    std::ifstream infile(filePath);
    std::istringstream iss;
    char* cline;
    std::string head;
    double dCoordX, dCoordY, dCoordZ;
    int lineNum, PointNum, type;
    GuidancePoint* pGP = 0;

    while(infile.getline(cline, 255))
    {
        //line.replace(head.find("-"), 1, " ");
        std::string line(cline);
        iss.clear();
        iss.str(line);
        if(std::string::npos != line.find("AIRPORT"))
        {
            iss >> head >> dCoordX >> dCoordY >> type;
            pGP = new GuidancePoint(GuidancePoint::AirPort, QPointF(dCoordX, dCoordY));

        }
        else if(std::string::npos != line.find("-"))
        {
            iss >> lineNum >> PointNum >> dCoordX >> dCoordY >> dCoordZ >> type;
            if(std::string::npos != line.find("A1"))
            {
                pGP = new GuidancePoint(GuidancePoint::A1Type, QPointF(dCoordX, dCoordY), lineNum);

            }
            else if(std::string::npos != line.find("A2"))
            {
                pGP = new GuidancePoint(GuidancePoint::A2Type,
                                        QPointF(dCoordX, dCoordY), lineNum);

            }
            else if(std::string::npos != line.find("B1"))
            {
                pGP = new GuidancePoint(GuidancePoint::B1Type,
                                        QPointF(dCoordX, dCoordY), lineNum);
            }
            else if(std::string::npos != line.find("B2"))
            {
                pGP = new GuidancePoint(GuidancePoint::B2Type,
                                        QPointF(dCoordX, dCoordY), lineNum);
            }
            else
            {

                pGP = new GuidancePoint(GuidancePoint::Normal,
                                        QPointF(dCoordX, dCoordY), lineNum, PointNum);
            }
        }
        else
            continue;

        lsPoints.push_back(QPointF(dCoordX, dCoordY));
        drawObject::registerGuidancePoint(pGP);
    }
    infile.close();

}

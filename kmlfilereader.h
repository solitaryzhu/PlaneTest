#ifndef KMLFILEREADER_H
#define KMLFILEREADER_H

#include <string>
#include <ogrsf_frmts.h>

class KmlFileReader
{
public:
  KmlFileReader();
  void getPointCoordinate(std::string strFile,
                          OGRPoint& point);
  void getPolygonCoordinate(std::string strFile,
                            OGRPolygon& polygon);
protected:
  OGRGeometry* getFirstGeometry(std::string strFile);
};

#endif // KMLFILEREADER_H

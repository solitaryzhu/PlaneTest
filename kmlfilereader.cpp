#include "kmlfilereader.h"
#include <qdebug.h>

KmlFileReader::KmlFileReader()
{
}

OGRGeometry* KmlFileReader::getFirstGeometry(std::string strFile)
{
  OGRGeometry* pGeom = 0;
  OGRRegisterAll();
  OGRDataSource* pDS = OGRSFDriverRegistrar::Open(strFile.c_str());
  if (0 == pDS){ Q_ASSERT(0); return 0; }

  OGRLayer* pLayer = pDS->GetLayer(0);
  if (0 == pLayer){ Q_ASSERT(0); return 0; }
  pLayer->ResetReading();

  OGRFeature* pFeature = 0;
  while (0 != (pFeature = pLayer->GetNextFeature()))
  {
      pGeom = pFeature->GetGeometryRef()->clone();
      OGRFeature::DestroyFeature(pFeature);
  }

  OGRDataSource::DestroyDataSource(pDS);

  return pGeom;
}

void KmlFileReader::getPointCoordinate(std::string strFile,
                                       OGRPoint& point)
{
  OGRGeometry* pGeom = getFirstGeometry(strFile);

  if (0 != pGeom && wkbPoint == wkbFlatten(pGeom->getGeometryType()))
  {
      OGRPoint* pPoint = (OGRPoint*)pGeom;
      point = *pPoint;
      delete pGeom;
  }
}

void KmlFileReader::getPolygonCoordinate(std::string strFile,
                                         OGRPolygon &polygon)
{
  OGRGeometry* pGeom = getFirstGeometry(strFile);
  if (0 != pGeom && wkbPolygon == wkbFlatten(pGeom->getGeometryType()))
  {
      OGRPolygon* pPolygon = (OGRPolygon*)pGeom;
      polygon = *pPolygon;
      delete pGeom;
  }
}

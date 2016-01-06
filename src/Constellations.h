//
//  Constellations.h
//  ElectronicGirls-Pleiadi
//
//  Created by XXX on 06/01/16.
//
//

#ifndef __ElectronicGirls_Pleiadi__Constellations__
#define __ElectronicGirls_Pleiadi__Constellations__

#include "BillboardLayer.h"

class Constellations : public BillboardLayer
{
public:
                            Constellations();
  void                      addConstellation(int totPoints, ofVec3f points[]);
protected:
  void                      drawBillboard();
private:
  vector<ofVboMesh>         constellations;
  int                       totConstellations;
};

#endif /* defined(__ElectronicGirls_Pleiadi__Constellations__) */

//
//  Constellations.cpp
//  ElectronicGirls-Pleiadi
//
//  Created by XXX on 06/01/16.
//
//

#include "Constellations.h"

Constellations::Constellations()
{
  totConstellations = 0;
}

void Constellations::drawBillboard()
{
  BillboardLayer::drawBillboard();
  for(int a = 0; a < totConstellations; a++)
    constellations[a].draw();
}

void Constellations::addConstellation(int totPoints, ofVec3f points[])
{
  ofVboMesh tempMesh;
  tempMesh.getVertices().resize(totPoints);
  tempMesh.getColors().resize(totPoints);
  tempMesh.getNormals().resize(totPoints,ofVec3f(0));
  
  for(int a = 0; a < totPoints; a++)
  {
    tempMesh.getColors()[a].set(ofColor(255,0,0));
    tempMesh.getVertices()[a].set(points[a]);
    tempMesh.setNormal(a,ofVec3f(1000,0,0));
  }
  tempMesh.setUsage( GL_DYNAMIC_DRAW );
  tempMesh.setMode(OF_PRIMITIVE_POINTS);
  constellations.push_back(tempMesh);
  totConstellations++;
}
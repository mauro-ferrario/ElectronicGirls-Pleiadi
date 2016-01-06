//
//  Constellations.cpp
//  ElectronicGirls-Pleiadi
//
//  Created by XXX on 06/01/16.
//
//

#include "Constellations.h"

void Constellations::setup(int totBillboards, string textureName, ofVec3f spaceSize, vector<ofVec3f> positions)
{
  BillboardLayer::setup(totBillboards, textureName, spaceSize, positions);
}

Constellations::Constellations()
{
  totConstellations = 0;
}

void Constellations::drawBillboard()
{
  BillboardLayer::drawBillboard();
  for(int a = 0; a < totConstellations; a++)
  {
    int totPointConstellation = constellations[a].getVertices().size();
    for(int i = 0; i < totPointConstellation; i++)
    {
      constellations[a].setNormal(i, ofVec3f(billboardsScale*1.5,0,0));
    }
    ofPushMatrix();
    constellations[a].draw();
    ofPopMatrix();
  }
}

void Constellations::addConstellation(vector<ofVec3f> points)
{
  int totPoints = points.size();
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
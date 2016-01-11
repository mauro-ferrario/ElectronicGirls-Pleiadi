//
//  BillboardLayer.cpp
//  ElectronicGirls-Pleiadi
//
//  Created by XXX on 05/01/16.
//
//

#include "BillboardLayer.h"


void BillboardLayer::setup(int totBillboards, string textureName, ofVec3f spaceSize)
{
  this->totBillboards = totBillboards;
  this->spaceSize = spaceSize;
  setupBillboards();
  setupTexture(textureName);
}

void BillboardLayer::setup(int totBillboards, string textureName, ofVec3f spaceSize, vector<ofVec3f> positions)
{
  setup(totBillboards, textureName, spaceSize);
  for(int a = 0; a < totBillboards; a++)
    billboards.getVertices()[a].set(positions[a]);
}

void BillboardLayer::setupBillboards()
{
  billboards.getVertices().resize(totBillboards);
  billboards.getColors().resize(totBillboards);
  billboards.getNormals().resize(totBillboards,ofVec3f(0));
  
  for(int a = 0; a < totBillboards; a++)
  {
    billboardSizeTarget.push_back(1);
    if(a%10==0)
      billboardColor.push_back(ofColor(ofRandom(200,255),ofRandom(200,255),0));
    else
      billboardColor.push_back(ofColor(255));
    billboards.getColors()[a].set(billboardColor[a]);
    billboards.getVertices()[a].set(ofRandom(0, spaceSize.x), ofRandom(0, spaceSize.y), 0);
  }
  billboards.setUsage( GL_DYNAMIC_DRAW );
  billboards.setMode(OF_PRIMITIVE_POINTS);
  billboardShader.load("shaders/Billboard");
}

void BillboardLayer::setupTexture(string textureName)
{
  ofDisableArbTex();
  texture.load(textureName);
  ofEnableArbTex();
}

void BillboardLayer::update()
{
  for (int i=0; i<totBillboards; i++)
  {
    billboardSizeTarget[i] = billboardsScale;
    billboards.setNormal(i,ofVec3f(billboardSizeTarget[i],0,0));
  }
}

void BillboardLayer::update(vector<float> left, vector<float> right)
{
  for (int i=0; i<totBillboards; i++)
  {
    if(i < totBillboards*.5)
    {
      int index = ofMap(i, 0, totBillboards*.5, 0, left.size()-1);
      billboardSizeTarget[i] = ofClamp(billboardsScale * abs(left[index]),0, maxScaleValue);
    }
    else
    {
      int index = ofMap(i, totBillboards*.5, totBillboards-1, 0, right.size()-1);
      billboardSizeTarget[i] = ofClamp(billboardsScale * abs(right[index]), 0, maxScaleValue);
    }
    billboards.setNormal(i,ofVec3f(billboardSizeTarget[i],0,0));
  }
}

void BillboardLayer::draw(bool deactiveBlendMode)
{
  if(useShader&&!deactiveBlendMode)
    billboardShader.begin();
  if(useAddBlendMode&&!deactiveBlendMode)
      ofEnableBlendMode(OF_BLENDMODE_ADD);
  ofEnablePointSprites(); // not needed for GL3/4
  texture.getTexture().bind();
  ofPushMatrix();
  ofTranslate(0, 0, posZ);
  drawBillboard();
  ofPopMatrix();
  texture.getTexture().unbind();
  ofDisablePointSprites(); // not needed for GL3/4
  if(useAddBlendMode&&!deactiveBlendMode)
    ofDisableBlendMode();
  if(useShader&&!deactiveBlendMode)
    billboardShader.end();
}

void BillboardLayer::drawBillboard()
{
  billboards.draw();
}

ofParameterGroup* BillboardLayer::getBillboardParams()
{
  if(!billboardParams)
  {
    billboardParams = new ofParameterGroup();
  }
  if(billboardParams->getName() == "")
  {
    billboardParams->setName("Billboard");
    billboardParams->add(useShader.set("Use shader", true));
    billboardParams->add(useAddBlendMode.set("Use blend mode", true));
    billboardParams->add(posZ.set("Pos z", 0, -1000, 1000));
    billboardParams->add(billboardsScale.set("billboards Scale", 1, 0, 10000));
    billboardParams->add(maxScaleValue.set("Max Scale Value", 1, 0, 1000));
    
  }
  return billboardParams;
}
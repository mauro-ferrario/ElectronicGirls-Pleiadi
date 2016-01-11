//
//  BillboardLayer.h
//  ElectronicGirls-Pleiadi
//
//  Created by XXX on 05/01/16.
//
//

#ifndef __ElectronicGirls_Pleiadi__BillboardLayer__
#define __ElectronicGirls_Pleiadi__BillboardLayer__

#include "ofMain.h"
#include "ofxGui.h"

class BillboardLayer
{
public:
                      BillboardLayer(){};
  void                setup(int totBillboards, string textureName, ofVec3f spaceSize);
  virtual void        setup(int totBillboards, string textureName, ofVec3f spaceSize, vector<ofVec3f> positions);
  void                update();
  void                update(vector<float> left, vector<float> right);
  void                draw(bool deactiveBlendMode = false);
  void                setupTexture(string textureName);
  ofParameterGroup*   getBillboardParams();
  ofParameter<float>  billboardsScale;
  
protected:
  virtual void        drawBillboard();
  int                 totBillboards;
  void                setupBillboards();
  ofVec3f             spaceSize;

private:
  vector<float>       billboardSizeTarget;
  vector<ofColor>     billboardColor;
  ofShader            billboardShader;
  ofImage             texture;
  ofVboMesh           billboards;
  
  
  ofParameterGroup*   billboardParams;
  ofParameter<bool>   useShader;
  ofParameter<bool>   useAddBlendMode;
  ofParameter<float>  posZ;
  ofParameter<float>  maxScaleValue;
};

#endif /* defined(__ElectronicGirls_Pleiadi__BillboardLayer__) */

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
  void              setup(int totBillboards, string textureName, ofVec3f spaceSize);
  void              update();
  void              draw();
  void              setupTexture(string textureName);
  ofParameterGroup* getBillboardParams();
private:
  void              setupBillboards();
  int               totBillboards;
  vector<float>     billboardSizeTarget;
  vector<ofColor>   billboardColor;
  ofShader          billboardShader;
  ofImage           texture;
  ofVboMesh         billboards;
  ofVec3f           spaceSize;
  
  
  ofParameterGroup*   billboardParams;
  ofParameter<bool>   useShader;
  ofParameter<bool>   useAddBlendMode;
  ofParameter<float>  billboardsScale;
  ofParameter<float>  posZ;
  ofEasyCam           cam;
};

#endif /* defined(__ElectronicGirls_Pleiadi__BillboardLayer__) */

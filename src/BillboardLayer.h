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
  void              setup(int totBillboards, string textureName, ofVec3f spaceSize);
  void              setup(int totBillboards, string textureName, ofVec3f spaceSize, vector<ofVec3f> positions);
  void              update();
  void              draw();
  void              setupTexture(string textureName);
  ofParameterGroup* getBillboardParams();
protected:
  virtual void      drawBillboard();
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

//
//  EG-Pleiadi.h
//  ElectronicGirls-Pleiadi
//
//  Created by XXX on 02/01/16.
//
//

#ifndef __ElectronicGirls_Pleiadi__EG_Pleiadi__
#define __ElectronicGirls_Pleiadi__EG_Pleiadi__

#include "ofMain.h"
#include "ofxGUI.h"
#include "GoofyPerlinNoise.h"
#include "GoofyBlurGLSL.h"
#include "BillboardLayer.h"
#include "Constellations.h"

#define TOT_STARS_1 2000
#define TOT_STARS_2 500

class EGPleiadi
{
public:
                        EGPleiadi();
  void                  setup();
  void                  update();
  void                  draw();
  void                  toggleDebug();
  void                  toggleGUI();
  void                  savePreset();
  void                  saveDebugImages();
  void                  audioIn(float * input, int bufferSize, int nChannels);
private:
  void                  setupFBOs();
  void                  setupGUI();
  void                  updateColorSky();
  void                  setupSingleFbo(ofFbo& fbo);
  void                  setupStars();
  void                  startRotationScaleMatrix();
  void                  endRotationScaleMatrix();
  void                  updateStars1();
  void                  updateStars2();
  void                  drawDebug();
  void                  setupConstellations();
  ofVec2f               size;
  ofFbo                 starFbo1;
  ofFbo                 starFbo2;
  ofShader              colorDodge;
  
  BillboardLayer        billboardLayer1;
  Constellations        billboardLayer2;
  
  GoofyBlurGLSL         goofyBlurStar1;
  GoofyBlurGLSL         goofyBlurStar2;
  
  ofParameterGroup*     getStars1ParameterGroup();
  ofParameterGroup*     stars1Params;
  ofParameter<float>    blur1;
  ofParameter<bool>     drawStars1;
  ofParameter<bool>     useBillboard1;
  
  ofParameterGroup*     getStars2ParameterGroup();
  ofParameterGroup*     stars2Params;
  ofParameter<float>    blur2;
  ofParameter<bool>     drawStars2;
  ofParameter<bool>     useBillboard2;
  
  vector<ofVec3f>       stars1;
  vector<ofVec3f>       stars2;
  
  
  ofParameterGroup*     getNoiseParameterGroup();
  ofParameterGroup*     noiseParams;
  GoofyPerlinNoise      noise;
  ofParameter<float>    noisePercent;
  
  ofFbo                 colorSkyFbo;
  ofParameterGroup*     getColorSkyParameterGroup();
  ofParameterGroup*     colorSkyParams;
  ofParameter<bool>     useDynamicColorSky;
  ofParameter<ofColor>  colorSky;
  ofParameter<ofColor>  colorSky2;
  ofParameter<float>    backgroundMultiplier;
  ofParameter<float>    backgroundFadeOutSpeed;
  ofParameter<bool>     drawSky;
  ofImage               colorSkyImage;
  ofImage               colorSkyImage2;
  
  ofxPanel              gui;
  bool                  guiVisible;
  bool                  debugVisible;
  
  ofParameter<bool>     rotationAndScaleEnabled;
  ofParameter<float>    rotationSpeed;
  ofParameter<float>    trail;
  ofParameter<bool>     trailActive;
  ofParameter<float>    scale;
  float                 trailValue;
  
  ofImage               overlay;
  float                 angle;
  
  vector<float>         left;
  vector<float>         right;
  vector<float>         volHistory;
		
  int                   bufferCounter;
  int                   drawCounter;
		
  float                 smoothedVol;
  float                 scaledVol;

};

#endif /* defined(__ElectronicGirls_Pleiadi__EG_Pleiadi__) */

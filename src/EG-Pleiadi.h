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

#define TOT_STARS_1 1500
#define TOT_STARS_2 500

class EGPleiadi
{
public:
                        EGPleiadi();
  void                  setup();
  void                  update();
  void                  draw();
  void                  toggleGUI();
  void                  savePreset();
  void                  saveDebugImages();
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
  ofVec2f               size;
  ofFbo                 starFbo1;
  ofFbo                 starFbo2;
  ofShader              colorDodge;
  
  GoofyBlurGLSL         goofyBlurStar1;
  GoofyBlurGLSL         goofyBlurStar2;
  
  ofParameterGroup*     getStars1ParameterGroup();
  ofParameterGroup*     stars1Params;
  ofParameter<float>    blur1;
  ofParameter<bool>     drawStars1;
  
  ofParameterGroup*     getStars2ParameterGroup();
  ofParameterGroup*     stars2Params;
  ofParameter<float>    blur2;
  ofParameter<bool>     drawStars2;
  
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
  ofParameter<bool>     drawSky;
  ofImage               colorSkyImage;
  
  ofxPanel              gui;
  bool                  guiVisible;
  
  ofParameter<float>    rotationSpeed;
  ofParameter<float>    trail;
  ofParameter<bool>     trailActive;
  ofParameter<float>    scale;
  float                 trailValue;
  
  ofImage               overlay;
  float                 angle;
};

#endif /* defined(__ElectronicGirls_Pleiadi__EG_Pleiadi__) */

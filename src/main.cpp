#include "ofMain.h"
#include "ofApp.h"

#define USE_PROGRAMMABLE_RENDERER

#ifdef USE_PROGRAMMABLE_RENDERER
#include "ofGLProgrammableRenderer.h"
#endif

//========================================================================
int main( ){
  ofGLWindowSettings settings;
  settings.setGLVersion(3,2);
  ofCreateWindow(settings);     // this kicks off the running of my app
  ofRunApp(new ofApp());
}
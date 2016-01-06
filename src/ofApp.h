#pragma once

#include "ofMain.h"
#include "EG-Pleiadi.h"
#include "ofxGui.h"
#include "GoofyBlurGLSL.h"
#include "GoofyPerlinNoise.h"



class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		void audioIn(float * input, int bufferSize, int nChannels);
    void exit();
		ofSoundStream soundStream;
		EGPleiadi pleiadi;
    vector<ofVec3f> points;
    ofxPanel gui;
		ofParameter<bool> vertical;
		ofParameter<bool> horizontal;
    ofFbo fbo;
    GoofyBlurGLSL goofyBlur;


};

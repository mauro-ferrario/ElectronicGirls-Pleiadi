#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
  ofBackground(0);
  ofSetWindowPosition(-2000, 0);
  ofToggleFullscreen();
  pleiadi.setup();
}
//--------------------------------------------------------------
void ofApp::update(){
  pleiadi.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
  pleiadi.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
  if(key == 'g')
    pleiadi.toggleGUI();
  if(key == 's')
  {
    pleiadi.savePreset();
    pleiadi.saveDebugImages();
  }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

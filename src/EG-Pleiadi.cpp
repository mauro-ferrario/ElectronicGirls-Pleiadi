//
//  EG-Pleiadi.cpp
//  ElectronicGirls-Pleiadi
//
//  Created by XXX on 02/01/16.
//
//

#include "EG-Pleiadi.h"


EGPleiadi::EGPleiadi()
{
}

void EGPleiadi::setup()
{
  ofSetFrameRate(60);
  size.x = 1920;
  size.y = 1200;
  noisePercent = 60;
  setupFBOs();
  drawSky = true;
  useDynamicColorSky = false;
  colorSkyImage.load("images/testSky.jpg");
  overlay.load("images/overlay.png");
  colorDodge.load("shaders/colorDodge");
  noise.initPerlinImage(size.x/noisePercent, size.y/noisePercent, 1024, 768);
  setupStars();
  angle = 0;
  setupGUI();
  
  int bufferSize = 256;
  left.assign(bufferSize, 0.0);
  right.assign(bufferSize, 0.0);
  volHistory.assign(400, 0.0);
  
  bufferCounter	= 0;
  drawCounter		= 0;
  smoothedVol     = 0.0;
  scaledVol		= 0.0;

}

void EGPleiadi::setupStars()
{
  for(int a = 0; a < TOT_STARS_1; a++)
    stars1.push_back(ofVec3f(ofRandom(0,size.x), ofRandom(0,size.y), ofRandom(.1,3)));
  for(int a = 0; a < TOT_STARS_2; a++)
    stars2.push_back(ofVec3f(ofRandom(0,size.x), ofRandom(0,size.y), ofRandom(3,10)));
  
  starFbo1.begin();
  ofClear(0,0);
  for(int a = 0; a < TOT_STARS_1; a++)
    ofDrawCircle(stars1[a].x, stars1[a].y, stars1[a].z);
  starFbo1.end();
  
  starFbo2.begin();
  ofClear(0,0);
  for(int a = 0; a < TOT_STARS_2; a++)
    ofDrawCircle(stars2[a].x, stars2[a].y, stars2[a].z);
  starFbo2.end();
  
  goofyBlurStar1.setup(size.x, size.y, "shaders/");
  goofyBlurStar2.setup(size.x, size.y, "shaders/");
  
  billboardLayer1.setup(TOT_STARS_1, "images/spark.png", ofVec3f(size), stars1);
  billboardLayer2.setup(TOT_STARS_2, "images/spark.png", ofVec3f(size), stars2);
  setupConstellations();
}

void EGPleiadi::setupConstellations()
{
  ofXml xml;
  xml.load("constellations.xml");
  int totConstellations =  xml.getNumChildren();
  for(int a = 0; a < totConstellations; a++)
  {
    xml.setTo("constellation["+ofToString(a)+"]");
    vector<ofVec3f> points;
    int totLines = xml.getNumChildren("line");
    if(xml.getValue("name") == "UMi")
    {
      ofVec2f max = ofVec2f(66600000);
      ofVec2f min = ofVec2f(-66600000);
      cout << xml.getValue("name") << " - Lines = "  << totLines << endl;
      for(int i = 0; i < totLines; i++)
      {
        xml.setTo("line["+ofToString(i)+"]");
        
        xml.setTo("point[0]");
        ofVec3f tempPoint;
        tempPoint.x = ofToFloat(xml.getValue("x")) * 100;
        tempPoint.y = ofToFloat(xml.getValue("y")) * 100;

        tempPoint.x += 1920*.5;
        tempPoint.y += 1080*.5;
        
        bool found = false;
        for(int z = 0; z < points.size(); z++)
        {
          if(points[z] == tempPoint)
          {
            found = true;
            z = points.size();
          }
        }
        if(!found)
          points.push_back(tempPoint);
        
        xml.setToParent();
        
        xml.setTo("point[1]");
        tempPoint.x = ofToFloat(xml.getValue("x")) * 100;
        tempPoint.y = ofToFloat(xml.getValue("y")) * 100;
        tempPoint.x += 1920*.5;
        tempPoint.y += 1080*.5;
        found = false;
        for(int z = 0; z < points.size(); z++)
        {
          if(points[z] == tempPoint)
          {
            found = true;
            z = points.size();
          }
        }
        if(!found)
          points.push_back(tempPoint);
        xml.setToParent();
        
        if(tempPoint.x < min.x)
          min.x = tempPoint.x;
        if(tempPoint.y < min.y)
          min.y = tempPoint.y;
        
        if(tempPoint.x > max.x)
          max.x = tempPoint.x;
        if(tempPoint.y > max.y)
          max.y = tempPoint.y;
        
        xml.setToParent();
      }
      billboardLayer2.addConstellation(points);
    }
    xml.setToParent();
  }
}

void EGPleiadi::toggleGUI()
{
  if(debugVisible)
    guiVisible = !guiVisible;
}

void EGPleiadi::toggleDebug()
{
  debugVisible = !debugVisible;
}

void EGPleiadi::setupGUI()
{
  gui.setup();
  gui.add(rotationAndScaleEnabled.set("Apply rotation and scale", false));
  gui.add(scale.set("Scale", 1.5, 0,2));
  gui.add(rotationSpeed.set("Rotation", .5, 0,1));
  gui.add(trail.set("Trail", .5, 0,2));
  gui.add(trailActive.set("Trail active", false));
  gui.add(*getColorSkyParameterGroup());
  gui.add(*getNoiseParameterGroup());
  gui.add(*getStars1ParameterGroup());
  gui.add(*getStars2ParameterGroup());
  guiVisible = true;
  gui.loadFromFile("preset.xml");
  trailValue = 255;
}

void EGPleiadi::setupFBOs()
{
  setupSingleFbo(starFbo1);
  setupSingleFbo(starFbo2);
  setupSingleFbo(colorSkyFbo);
}

void EGPleiadi::setupSingleFbo(ofFbo& fbo)
{
  fbo.allocate(size.x, size.y, GL_RGBA32F_ARB);
  fbo.begin();
  ofClear(0,255);
  fbo.end();
}

void EGPleiadi::update()
{
  //lets scale the vol up to a 0-1 range
  scaledVol = ofMap(smoothedVol, 0.0, 0.17, 0.0, 1.0, true);
  
  //lets record the volume into an array
  volHistory.push_back( scaledVol );
  
  //if we are bigger the the size we want to record - lets drop the oldest value
  if( volHistory.size() >= 400 ){
    volHistory.erase(volHistory.begin(), volHistory.begin()+1);
  }
  
  angle += rotationSpeed;
  if(drawStars1)
    updateStars1();
  if(drawStars2)
    updateStars2();
  if(drawSky)
  {
    noise.update();
    updateColorSky();
  }
}

void EGPleiadi::audioIn(float * input, int bufferSize, int nChannels)
{
  if(!(input))
    return;
  float curVol = 0.0;
  
  // samples are "interleaved"
  int numCounted = 0;
  
  //lets go through each sample and calculate the root mean square which is a rough way to calculate volume
  for (int i = 0; i < bufferSize; i++){
    left[i]		= input[i*2]*0.5;
    right[i]	= input[i*2+1]*0.5;
    
    curVol += left[i] * left[i];
    curVol += right[i] * right[i];
    numCounted+=2;
  }
  
  //this is how we get the mean of rms :)
  curVol /= (float)numCounted;
  
  // this is how we get the root of rms :)
  curVol = sqrt( curVol );
  
  smoothedVol *= backgroundFadeOutSpeed; //0.23;
  smoothedVol += 0.07 * curVol;
  
  bufferCounter++;
  
}

void EGPleiadi::updateStars1()
{
  starFbo1.begin();
  if(trailActive)
  {
    trailValue = trail;
    ofSetColor(0,trailValue);
    ofDrawRectangle(0, 0, size.x, size.y);
  }
  else
  {
    trailValue += 5;
    trailValue = ofClamp(trailValue, 0, 255);
    ofClear(0,255-trailValue);
  }
  
  startRotationScaleMatrix();
  if(useBillboard1)
  {
    billboardLayer1.update(left, right);
    billboardLayer1.draw();
  }
  else
  {
    ofColor c = colorSky;
    c.a = 255;
    ofSetColor(c);
    for(int a = 0; a < TOT_STARS_1; a++)
      ofDrawCircle(stars1[a].x, stars1[a].y, stars1[a].z* ofNoise(sin(float(a*.13/.84))*a*.0001) *2);
  }
  endRotationScaleMatrix();
  starFbo1.end();
  goofyBlurStar1.update(starFbo1, ofVec2f(blur1));
}

void EGPleiadi::updateStars2()
{
  starFbo2.begin();
  ofClear(0,0);
  startRotationScaleMatrix();
  if(useBillboard2)
  {
    billboardLayer2.update();
    billboardLayer2.draw();
  }
  else
  {
    for(int a = 0; a < TOT_STARS_2; a++)
      ofDrawCircle(stars2[a].x, stars2[a].y, stars2[a].z * ofNoise(sin(ofGetFrameNum())*.005*a/100) *1.2);
  }
  endRotationScaleMatrix();
  starFbo2.end();
  goofyBlurStar2.update(starFbo2, ofVec2f(blur2));
}

void EGPleiadi::startRotationScaleMatrix()
{
  ofPushMatrix();
  ofTranslate(size.x*.5, size.y*.5);
  if(rotationAndScaleEnabled)
  {
    ofScale(scale, scale);
    ofRotate(angle);
  }
  ofTranslate(-size.x*.5, -size.y*.5);
}

void EGPleiadi::endRotationScaleMatrix()
{
  ofPopMatrix();
}

void EGPleiadi::savePreset()
{
  gui.saveToFile("preset.xml");
}

void EGPleiadi::saveDebugImages()
{
  noise.perlinImg.save("perlin.jpg");
}

void EGPleiadi::updateColorSky()
{
  colorSkyFbo.begin();
  ofClear(0,255);
  if(useDynamicColorSky)
  {
    
  }
  else
  {
    ofColor c = colorSky;
    c.a = smoothedVol * 20 * 255 * backgroundMultiplier;
    colorSky.set(c);
    ofSetColor(colorSky);
    colorSkyImage.draw(0,0, size.x, size.y);
  }
  colorSkyFbo.end();
}

void EGPleiadi::draw()
{
  ofSetColor(255);
  if(drawSky)
  {
    startRotationScaleMatrix();
    colorDodge.begin();
    colorDodge.setUniform1f("noisePercent", noisePercent);
    colorDodge.setUniformTexture("blur", noise.perlinImg.getTexture(), 1);
    colorDodge.setUniformTexture("colorSky", colorSkyFbo.getTexture(), 2);
    noise.draw(0, 0, size.x, size.y);
    colorSkyFbo.draw(0,0);
    colorDodge.end();
    endRotationScaleMatrix();
  }
  if(drawStars1)
    goofyBlurStar1.draw();
  if(drawStars2)
    goofyBlurStar2.draw();
  if(debugVisible)
    drawDebug();
}

void EGPleiadi::drawDebug()
{
  ofSetColor(255);
  noise.perlinImg.draw(size.x - noise.perlinImg.getWidth() * 10 - 10, size.y - noise.perlinImg.getHeight() * 10 - 10, noise.perlinImg.getWidth() * 10, noise.perlinImg.getHeight() * 10 );
  ofDrawBitmapString(ofToString(ofGetFrameRate()), 20, size.y - 20);
  if(guiVisible)
    gui.draw();
}

ofParameterGroup* EGPleiadi::getStars1ParameterGroup()
{
  if(!stars1Params)
  {
    stars1Params = new ofParameterGroup();
  }
  if(stars1Params->getName() == "")
  {
    stars1Params->setName("Stars 1");
    stars1Params->add(useBillboard1.set("Use billboard", false));
    stars1Params->add(drawStars1.set("Draw star", true));
    stars1Params->add(blur1.set("Blur", 0, 0, 5));
    ofParameterGroup* billboard1 = billboardLayer1.getBillboardParams();
    billboard1->setName("Billboard 1");
    stars1Params->add(*billboardLayer1.getBillboardParams());
    billboard1 = NULL;
  }
  return stars1Params;
}

ofParameterGroup* EGPleiadi::getStars2ParameterGroup()
{
  if(!stars2Params)
  {
    stars2Params = new ofParameterGroup();
  }
  if(stars2Params->getName() == "")
  {
    stars2Params->setName("Stars 2");
    stars2Params->add(useBillboard2.set("Use billboard", false));
    stars2Params->add(drawStars2.set("Draw star", true));
    stars2Params->add(blur2.set("Blur", 0, 0, 5));
    ofParameterGroup* billboard2 = billboardLayer2.getBillboardParams();
    billboard2->setName("Billboard 2");
    stars2Params->add(*billboardLayer2.getBillboardParams());
    billboard2 = NULL;
  }
  return stars2Params;
}

ofParameterGroup* EGPleiadi::getNoiseParameterGroup()
{
  if(!noiseParams)
  {
    noiseParams = new ofParameterGroup();
  }
  if(noiseParams->getName() == "")
  {
    noiseParams->setName("Noise");
    noiseParams->add(noisePercent.set("Noise reduction", 20, 1, 100));
    noiseParams->add(*noise.getParameterGroup());
  }
  return noiseParams;
}

ofParameterGroup* EGPleiadi::getColorSkyParameterGroup()
{
  if(!colorSkyParams)
  {
    colorSkyParams = new ofParameterGroup();
  }
  if(colorSkyParams->getName() == "")
  {
    colorSkyParams->setName("Color Sky");
//    colorSkyParams->add(drawSky.set("Draw Sky", true));
    //    colorSkyParams->add(useDynamicColorSky.set("Use dynamic", false));
    colorSkyParams->add(backgroundFadeOutSpeed.set("Background Fade out speed",1, 0, 1));
    colorSkyParams->add(backgroundMultiplier.set("Background Sky Multipluer",1, 0, 10));
    colorSkyParams->add(colorSky.set("Color sky", ofColor(255,255), ofColor(0,0), ofColor(255,255)));
  }
  return colorSkyParams;
}
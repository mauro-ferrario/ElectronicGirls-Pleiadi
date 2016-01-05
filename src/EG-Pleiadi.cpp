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
  setup();
}

void EGPleiadi::setup()
{
  ofSetFrameRate(60);
  size.x = 1920;
  size.y = 1200;
  noisePercent = 60;
  setupFBOs();
  setupGUI();
  colorSkyImage.load("testSky.jpg");
  overlay.load("overlay.png");
  colorDodge.load("colorDodge");
  noise.initPerlinImage(size.x/noisePercent, size.y/noisePercent, 1024, 768);
  setupStars();
  angle = 0;
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
  
  goofyBlurStar1.setup(size.x, size.y);
  goofyBlurStar2.setup(size.x, size.y);
}

void EGPleiadi::toggleGUI()
{
  guiVisible = !guiVisible;
}

void EGPleiadi::setupGUI()
{
  gui.setup();
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
  angle += rotationSpeed;
  updateStars1();
  updateStars2();
  goofyBlurStar1.update(starFbo1, ofVec2f(blur1));
  goofyBlurStar2.update(starFbo2, ofVec2f(blur2));
  noise.update();
  updateColorSky();
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
  ofColor c = colorSky;
  c.a = 255;
  ofSetColor(c);
  for(int a = 0; a < TOT_STARS_1; a++)
    ofDrawCircle(stars1[a].x, stars1[a].y, stars1[a].z* ofNoise(sin(float(a*.13/.84))*a*.0001) *2);
  endRotationScaleMatrix();
  starFbo1.end();
}

void EGPleiadi::updateStars2()
{
  starFbo2.begin();
  ofClear(0,0);
  startRotationScaleMatrix();
  for(int a = 0; a < TOT_STARS_2; a++)
    ofDrawCircle(stars2[a].x, stars2[a].y, stars2[a].z * ofNoise(sin(ofGetFrameNum())*.005*a/100) *1.2);
  endRotationScaleMatrix();
  starFbo2.end();
}

void EGPleiadi::startRotationScaleMatrix()
{
  ofPushMatrix();
  ofTranslate(size.x*.5, size.y*.5);
  ofScale(scale, scale);
  ofRotate(angle);
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
    ofSetColor(colorSky);
    colorSkyImage.draw(0,0, size.x, size.y);
  }
  colorSkyFbo.end();
}

void EGPleiadi::draw()
{
  ofSetColor(255);
  startRotationScaleMatrix();
  colorDodge.begin();
  colorDodge.setUniform1f("noisePercent", noisePercent);
  colorDodge.setUniformTexture("blur", noise.perlinImg.getTexture(), 1);
  colorDodge.setUniformTexture("colorSky", colorSkyFbo.getTexture(), 2);
  noise.draw(0, 0, size.x, size.y);
  colorSkyFbo.draw(0,0);
  colorDodge.end();
  endRotationScaleMatrix();
  goofyBlurStar1.draw();
  goofyBlurStar2.draw();
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
    stars1Params->add(blur1.set("Blur", 0, 0, 5));
    stars1Params->add(stepSize1.set("Step", 0, 0, 50));
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
    stars2Params->add(blur2.set("Blur", 0, 0, 5));
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
    colorSkyParams->add(useDynamicColorSky.set("Use dynamic", false));
    colorSkyParams->add(colorSky.set("Color sky", ofColor(255,255), ofColor(0,0), ofColor(255,255)));
  }
  return colorSkyParams;
}
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
  colorSkyImage.load("images/testSky.jpg");
  overlay.load("images/overlay.png");
  colorDodge.load("shaders/colorDodge");
  noise.initPerlinImage(size.x/noisePercent, size.y/noisePercent, 1024, 768);
  setupStars();
  angle = 0;
  setupGUI();
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
  
  billboardLayer1.setup(2000, "images/spark.png", ofVec3f(size));
  billboardLayer2.setup(500, "images/spark.png", ofVec3f(size));
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
    billboardLayer1.update();
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
//  billboardLayer1.draw();
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
    colorSkyParams->add(drawSky.set("Draw Sky", true));
    colorSkyParams->add(useDynamicColorSky.set("Use dynamic", false));
    colorSkyParams->add(colorSky.set("Color sky", ofColor(255,255), ofColor(0,0), ofColor(255,255)));
  }
  return colorSkyParams;
}
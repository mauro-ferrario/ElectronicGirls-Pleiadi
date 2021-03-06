//
//  GoofyPerlinNoise.cpp
//
//  Created by xxx on 21/09/13.
//

#include "GoofyPerlinNoise.h"

GoofyPerlinNoise::GoofyPerlinNoise(float resX, float resY, float speed)
{
  noiseParams = NULL;
  this->resX = resX;
  this->resY = resY;
  this->speed = speed;
  this->time = 0;
  this->drawPerlinImage = false;
}

GoofyPerlinNoise::~GoofyPerlinNoise()
{
  removeNoiseParams();
}

void GoofyPerlinNoise::removeNoiseParams()
{
  if(noiseParams)
  {
    noiseParams = NULL;
    delete noiseParams;
  }
}

ofParameterGroup* GoofyPerlinNoise::getParameterGroup()
{
  if(!noiseParams)
     {
         noiseParams = new ofParameterGroup();
     }
  if(noiseParams->getName() == "")
  {
      noiseParams->setName("GoofyNoise");
      noiseParams->add(resX.set("ResX",0.01,.001,.1));
      noiseParams->add(resY.set("ResY",0.01,.001,.1));
      noiseParams->add(speed.set("Speed",.05,.0,.1));
      noiseParams->add(force.set("Force",1,.0,1));
  }
  return noiseParams;
}

void GoofyPerlinNoise::initPerlinImage(int perlinImgWidth, int perlinImgHeight, int width, int height)
{
    perlinImg.allocate(perlinImgWidth, perlinImgHeight, OF_IMAGE_GRAYSCALE);
    perlinWidth = width;
    perlinHeight = height;
    setPerlinImageRect(ofRectangle(0,0,perlinImgWidth,perlinImgHeight));
}

void GoofyPerlinNoise::draw()
{
    perlinImg.draw(perlinImageRect);
}

void GoofyPerlinNoise::draw(int x, int y, int width, int height)
{
    perlinImg.draw(x,y,width,height);
}

float GoofyPerlinNoise::getValue(int x, int y)
{
    return ofNoise(x * resX,y * resY,time);
}

float GoofyPerlinNoise::getValue(int x, int y, float tempResX, float tempResY)
{
    return ofNoise(x * tempResX,y * tempResY, time);
}

void GoofyPerlinNoise::setPerlinImageRect(ofRectangle rect)
{
    perlinImageRect = rect;
}

void GoofyPerlinNoise::update()
{
//  sync.update();
  time += speed;
  if(perlinImg.isAllocated())
  {
    unsigned char* pixels = perlinImg.getPixels();
    for(int a = 0; a < perlinImg.getWidth() * perlinImg.getHeight(); a++)
    {
      int x = a%int(perlinImg.getWidth());
      int y = (a-x)/perlinImg.getWidth();
      x = ofMap(x,0,perlinImg.getWidth(),0,perlinWidth);
      y = ofMap(y,0,perlinImg.getHeight(),0,perlinHeight);
      pixels[a] = getValue(x,y) * 255;
    }
    perlinImg.update();
  }
}
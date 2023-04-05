#include "Strip.h"

Strip::Strip(int stripSize, int16_t pin)
{
  this->stripSize = stripSize;
  pixelStrip = new Adafruit_NeoPixel(stripSize, pin, NEO_RGB + NEO_KHZ800);
  pixelStrip->begin();
}

Strip::~Strip() 
{
  delete pixelStrip;
}

void Strip::Update() 
{
  pixelStrip->show();
}

void Strip::SetPixelColor(uint16_t index, uint8_t r, uint8_t g, uint8_t b)
{
  pixelStrip->setPixelColor(index, r, g, b);
}

void Strip::SetPixelColor(uint16_t index, uint32_t c) 
{
  pixelStrip->setPixelColor(index, c);
}

void Strip::SetPixelColor(Color color, uint16_t index) 
{
  pixelStrip->setPixelColor(index, color.color);
}

void Strip::Fill(Color color) 
{
  pixelStrip->fill(color.color, 0, stripSize);
}

void Strip::FillRange(Color color, uint16_t startIndex, uint16_t range)
{
  if (startIndex > stripSize) return;

  for (int i = startIndex; i < startIndex + range; i++) 
  {
    SetPixelColor(color, i);
  }
}

void Strip::Walk(Color color, uint16_t &index, uint16_t range, uint16_t stepSize) 
{
  FillRange(color, index, range);
  index += stepSize;
  if (index >= stripSize) index = 0;
  if (index < 0) index = stripSize - 1;
}

void Strip::Pulse(Color color, uint16_t minIndex, uint16_t maxIndex, uint16_t &tIndex, uint16_t range, uint16_t stepSize) 
{
  for (int i = tIndex; i < tIndex + range; i++) 
  {
    SetPixelColor(color, i);
  }

  tIndex += stepSize * pulseSign;
  if (tIndex <= minIndex)
  {
    pulseSign *= -1;
    tIndex = minIndex + 1;
  }
  
  if (tIndex >= maxIndex) 
  {
    pulseSign *= -1;
    tIndex = maxIndex - 1;
  }

}

void Strip::Clear()
{
  pixelStrip->clear();
}




#include "Color.h"
#include "MathY.h"

Color::Color(uint8_t r, uint8_t g, uint8_t b, float brightness)
{
  this->brightness = brightness;
  this->r = r * brightness;
  this->g = g * brightness;
  this->b = b * brightness;
  UpdateColor();
}

Color::~Color()
= default;

void Color::SetColor(uint8_t r, uint8_t g, uint8_t b)
{
  this->r = r;
  this->g = g;
  this->b = b;
  UpdateColor();
}

void Color::SetBrightness(float brightness) 
{
  this->brightness = brightness;
  UpdateColor();
}

void Color::AddColor(uint8_t r, uint8_t g, uint8_t b)
{
  SetColor(this->r + r, this->g + g, this->b + b);
}

void Color::UpdateColor()
{
  uint8_t r = this->r * brightness;
  uint8_t g = this->g * brightness;
  uint8_t b = this->b * brightness;
  color = ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
}

bool Color::FlowTo(Color otherColor, float flowSpeed = 0.1f, bool doBrightness)
{
  if (currentFlow == 0) 
  {
    rBegin = r;
    gBegin = g;
    bBegin = b;
    brightnessBegin = brightness;
  }
  r = MathY::Lerp(rBegin, otherColor.r, currentFlow);
  g = MathY::Lerp(gBegin, otherColor.g, currentFlow);
  b = MathY::Lerp(bBegin, otherColor.b, currentFlow);
  if (doBrightness) brightness = MathY::Lerp(brightnessBegin, otherColor.brightness, currentFlow);

  currentFlow += flowSpeed;
  UpdateColor();
  if (currentFlow >= 1) 
  {
    currentFlow = 0;
    return true;
  }
  return false;
}
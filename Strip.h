#include <Adafruit_NeoPixel.h>
#include "Color.h"

class Strip 
{
public:
  Strip(int stripSize, int16_t pin);
  ~Strip();

  void Update();
  void SetPixelColor(uint16_t index, uint8_t r, uint8_t g, uint8_t b);
  void SetPixelColor(uint16_t index, uint32_t c);
  void SetPixelColor(Color color, uint16_t index);
  void Fill(Color color);
  void FillRange(Color color, uint16_t startIndex, uint16_t range);
  void Walk(Color color, uint16_t &index, uint16_t range = 1, uint16_t stepSize = 1);
  void Pulse(Color color, uint16_t minIndex, uint16_t maxIndex, uint16_t &tIndex, uint16_t range = 1, uint16_t stepSize = 1);
  void Clear();

private:
  int stripSize;
  Adafruit_NeoPixel* pixelStrip;
  int pulseSign = 1;
};
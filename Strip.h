#include <Adafruit_NeoPixel.h>
#include "Range.h"

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
  void FillRange(Range &range, bool turnOff = true);
  void Walk(Color color, int &index, uint16_t range = 1, uint16_t stepSize = 1);
  void Walk(Range &range, bool turnOff = true, uint16_t stepSize = 1);
  void Pulse(Color color, uint16_t minIndex, uint16_t maxIndex, int &tIndex, uint16_t range = 1, uint16_t stepSize = 1);
  void Pulse(Range &range, bool turnOff = true, uint16_t stepSize = 1);
  void GapInterval(Range &range, uint16_t gapSize = 1);
  void LitInterval(Range &range, uint16_t litSize = 1, uint16_t gapSize = 1);
  void Clear();

private:
  int stripSize;
  Adafruit_NeoPixel* pixelStrip;
  int pulseSign = 1;
};
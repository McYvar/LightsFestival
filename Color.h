#include <stdint.h>

using namespace std;

class Color
{
public:
  Color();
  Color(uint8_t r, uint8_t g, uint8_t b, float brightness = 1);
  ~Color();
  void SetColor(uint8_t r, uint8_t g, uint8_t b);
  void SetBrightness(float brightness);
  void AddColor(uint8_t r, uint8_t g, uint8_t b);
  void UpdateColor();
  bool FlowTo(Color otherColor, float flowSpeed, bool doBrightness = false); // returns true if color is the same
  uint32_t color;
  uint8_t r;
  uint8_t g;
  uint8_t b;
  float brightness;

private:
  float currentFlow;
  uint8_t rBegin;
  uint8_t gBegin;
  uint8_t bBegin;
  float brightnessBegin;
};
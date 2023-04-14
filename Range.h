#include "Color.h"

class Range
{
public:
  Range(Color color, uint16_t start, uint16_t reachMin, uint16_t reachMax, uint16_t range);
  ~Range();

  void SetLifeTime(int lifeTime);
  bool GetLifeTime();

  Color color = Color(0, 0, 0, 1.0f);
  uint16_t reachMin;
  uint16_t reachMax;
  uint16_t range;
  uint16_t colorIterator;
  int walkIterator;
  int lastWalk;
  int pulseStart;
  int lifeTime;
};
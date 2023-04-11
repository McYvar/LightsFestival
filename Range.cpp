#include "Range.h"

Range::Range(Color color, uint16_t start, uint16_t reachMin, uint16_t reachMax, uint16_t range) 
{
  this->color = color;
  this->reachMin = reachMin;
  this->reachMax = reachMax;
  this->range = range;
  walkIterator = start;
  lastWalk = walkIterator;
  colorIterator = 0;
}

Range::~Range()
= default;
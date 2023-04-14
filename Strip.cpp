#include "Strip.h"

Strip::Strip(int stripSize, int16_t pin) {
  this->stripSize = stripSize;
  pixelStrip = new Adafruit_NeoPixel(stripSize, pin, NEO_RGB + NEO_KHZ800);
  pixelStrip->begin();
}

Strip::~Strip() {
  delete pixelStrip;
}

void Strip::Update() {
  pixelStrip->show();
}

void Strip::SetPixelColor(uint16_t index, uint8_t r, uint8_t g, uint8_t b) {
  pixelStrip->setPixelColor(index, r, g, b);
}

void Strip::SetPixelColor(uint16_t index, uint32_t c) {
  pixelStrip->setPixelColor(index, c);
}

void Strip::SetPixelColor(Color color, uint16_t index) {
  pixelStrip->setPixelColor(index, color.color);
}

void Strip::Fill(Color color) {
  pixelStrip->fill(color.color, 0, stripSize);
}

void Strip::FillRange(Color color, uint16_t startIndex, uint16_t range) {
  for (int i = startIndex; i < startIndex + range; i++) {
    if (i > stripSize) {
      SetPixelColor(color, i - stripSize);
    } else SetPixelColor(color, i);
  }
}

void Strip::FillRange(Range &range, bool turnOff) {
  if (turnOff) {
    FillRange(Color(), range.lastWalk, range.range);
    FillRange(range.color, range.walkIterator, range.range);
  } else FillRange(range.color, range.walkIterator, range.range);

  range.lastWalk = range.walkIterator;
}

void Strip::Walk(Color color, int &index, uint16_t range, uint16_t stepSize) {
  FillRange(color, index, range);
  index += stepSize;
  if (index >= stripSize) index = 0;
  if (index < 0) index = stripSize - 1;
}

void Strip::Walk(Range &range, bool turnOff, uint16_t stepSize) {
  if (turnOff) {
    Walk(Color(), range.walkIterator, range.range, stepSize);
    Walk(range.color, range.walkIterator, range.range, 0);
  } else Walk(range.color, range.walkIterator, range.range, stepSize);
}

void Strip::Pulse(Color color, uint16_t minIndex, uint16_t maxIndex, int &tIndex, uint16_t range, uint16_t stepSize) {
  int fade = (range / 2.0);


  for (int i = tIndex; i < tIndex + range; i++) {
    if (i > stripSize || i < 0) continue;
    SetPixelColor(color, i);
  }

  tIndex += stepSize * pulseSign;
  if (tIndex < minIndex - range) {
    pulseSign *= -1;
    tIndex = minIndex - range;
  }

  if (tIndex > maxIndex) {
    pulseSign *= -1;
    tIndex = maxIndex;
  }
}

void Strip::Pulse(Range &range, bool turnOff, uint16_t stepSize) {
  if (turnOff) {
    Pulse(Color(), range.reachMin, range.reachMax, range.walkIterator, range.range, stepSize);
    Pulse(range.color, range.reachMin, range.reachMax, range.walkIterator, range.range, 0);
  } else Pulse(range.color, range.reachMin, range.reachMax, range.walkIterator, range.range, stepSize);
}

void Strip::GapInterval(Range &range, uint16_t gapSize) {
  for (int i = range.walkIterator + 1; i < range.walkIterator + range.range; i += 1 + gapSize) {
    for (int j = i; j < i + gapSize; j++) {
      if (j > stripSize) SetPixelColor(Color(), j - stripSize);
      else SetPixelColor(Color(), j);
    }
  }
}

void Strip::LitInterval(Range &range, uint16_t litSize, uint16_t gapSize) {
  for (int i = range.walkIterator; i < range.walkIterator + range.range; i += 1 + gapSize) {
    for (int j = i; j < i + litSize; j++) {
      if (j > stripSize) SetPixelColor(range.color, j - stripSize);
      else SetPixelColor(range.color, j);
    }
  }
}

void Strip::Spread(Range &range, bool turnOff, uint16_t stepSize) {
  int oldOffset = range.pulseStart - range.walkIterator;
  if (turnOff) {
    // first upwalking spread
    int upStart = range.pulseStart + oldOffset;
    for (int i = upStart; i < upStart + range.range; i++) {
      if (i > stripSize) break;
      SetPixelColor(Color(), i);
    }

    // then downwalking spread
    int downStart = range.pulseStart - oldOffset;
    for (int i = downStart; i > downStart - range.range; i--) {
      if (i < 0) break;
      SetPixelColor(Color(), i);
    }
  }

  range.walkIterator += stepSize;
  int offset = range.pulseStart - range.walkIterator;

  // first upwalking spread
  int upStart = range.pulseStart + offset;
  for (int i = upStart; i < upStart + range.range; i++) {
    if (i > stripSize) break;
    SetPixelColor(range.color, i);
  }

  // then downwalking spread
  int downStart = range.pulseStart - offset;
  for (int i = downStart; i > downStart - range.range; i--) {
    if (i < 0) break;
    SetPixelColor(range.color, i);
  }
}

void Strip::Clear() {
  pixelStrip->clear();
}

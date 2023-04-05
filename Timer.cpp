#include "Timer.h"

Timer::Timer()
= default;

Timer::~Timer()
= default;

void Timer::Update(unsigned long currentTime) {
  deltaTime = currentTime - lastFrameTime;
  lastFrameTime = currentTime;
}
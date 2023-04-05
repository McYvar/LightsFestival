#include "Strip.h"
#include "Timer.h"

Strip myStrip = Strip(194, D4);
Timer timer;

Color flow = Color(0, 0, 0, 1.0f);
Color red = Color(0, 255, 0, 1.0f);
Color orange = Color(128, 255, 0, 0.1f);
Color yellow = Color(255, 255, 0, 1.0f);
Color green = Color(255, 0, 0, 0.1f);
Color blue = Color(0, 0, 255, 1.0f);
Color purple = Color(0, 255, 255, 0.1f);
Color off = Color(0, 0, 0);
Color colors[] = { red, orange, yellow, green, blue, purple };

uint16_t walkIterator1 = 0;
uint16_t walkIterator2 = 70;

unsigned long intervalTimer = 0;

int colorIterator = 0;
float flowSpeed = 0.05f;

void setup() 
{
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() 
{
  timer.Update(millis());
  intervalTimer += timer.deltaTime;
  if (intervalTimer > 50) 
  {
    intervalTimer = 0;
    
    GoRandom(colorIterator);
    //myStrip.Walk(off, walkIterator1, 10, 1);
    //myStrip.Walk(flow, walkIterator1, 10, 0);
    //myStrip.Pulse(off, 0, 140, walkIterator1, 4);
    //myStrip.Pulse(flow, 0, 140, walkIterator1, 4, 0);
    myStrip.Fill(flow);
  }

  myStrip.Update();
}

void GoNext(int &i) 
{
  if (flow.FlowTo(colors[i], flowSpeed, true)) i++;
  if (i >= size(colors)) i = 0;
}

void GoRandom(int &i) 
{
  if (flow.FlowTo(colors[i], flowSpeed, false)) 
  {
    i = random(0, size(colors));
  }
}
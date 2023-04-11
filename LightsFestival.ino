#include "Strip.h"
#include "Timer.h"

#define LED_AMOUNT 194

Strip myStrip = Strip(LED_AMOUNT, D4); // I have 194
Timer timer;

Color flow = Color(0, 0, 0, 1.0f);
Color red = Color(0, 255, 0, 1.0f);
Color orange = Color(128, 255, 0, 1.0f);
Color yellow = Color(255, 255, 0, 1.0f);
Color green = Color(255, 0, 0, 1.0f);
Color blue = Color(0, 0, 255, 1.0f);
Color purple = Color(0, 255, 255, 1.0f);
Color off = Color(0, 0, 0);
Color colors[] = { red, orange, yellow, green, blue, purple };

unsigned long intervalTimer = 0;
float flowSpeed = 0.05f;

// setup the ranges
// Range(Color, start, begin, end, range)
Range range1 = Range(Color(0, 0, 255, 1.0f), 0, 0, LED_AMOUNT, 5);
Range range2 = Range(Color(255, 0, 0, 1.0f), 15, 0, LED_AMOUNT, 5);


void setup() 
{
  Serial.begin(115200);
  Serial.println("Booting up!");
}

void loop() 
{
  timer.Update(millis());
  intervalTimer += timer.deltaTime;
  if (intervalTimer > 30) 
  {
    intervalTimer = 0;
    myStrip.FillRange(range1);
    GoRandom(range1);
    //myStrip.Walk(range1);
    //myStrip.Walk(range2, true, 2);
  }

  myStrip.Update();
  InputCommands();
}

void InputCommands()
{
  String command = "";
  while (Serial.available()) 
  {
    int read = Serial.read();
    if (read != 10 && read != 13) // cuz 10/13 === return
      command += (char)read;
  }

  if (command == "pulse1")
  {
    //Serial.println(command);
  }
  // asuming it's a number
  else 
  {
    range1.walkIterator += command.toInt();
    if (range1.walkIterator > LED_AMOUNT) range1.walkIterator = 0;
    if (range1.walkIterator < 0) range1.walkIterator = LED_AMOUNT;
  }
  Serial.println(command);
}

void GoNext(uint16_t &i, Color &color) 
{
  if (color.FlowTo(colors[i], flowSpeed, true)) i++;
  if (i >= sizeof(colors) / sizeof(colors[0])) i = 0;
}

void GoNext(Range &range)
{
  GoNext(range.colorIterator, range.color);
}

void GoRandom(uint16_t &i, Color &color)
{
  if (color.FlowTo(colors[i], flowSpeed, false))
  {
    Serial.println("new color");
    i = random(0, sizeof(colors) / sizeof(colors[0]));
  }
}

void GoRandom(Range &range)
{
  GoRandom(range.colorIterator, range.color);
}
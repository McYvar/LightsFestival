#include "Strip.h"
#include "Timer.h"
#include <vector>

#define LED_AMOUNT 194

Strip myStrip = Strip(LED_AMOUNT, D5); // I have 194
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

vector<Range> spreads = vector<Range>();

unsigned long intervalTimer = 0;
float flowSpeed = 0.05f;

// setup the ranges
// Range(Color, start, begin, end, range)
Range range1 = Range(Color(0, 0, 255, 1.0f), 0, 0, LED_AMOUNT, 5);
Range full = Range(Color(), 0, 0, LED_AMOUNT, LED_AMOUNT);

bool doFill = false;

void setup() 
{
  Serial.begin(115200);
  Serial.println("Booting up!");
}

void loop() 
{
  timer.Update(millis());
  intervalTimer += timer.deltaTime;
  if (intervalTimer > 20) 
  {
    if (doFill) 
    {
      full.color.SetBrightness(full.color.brightness += 0.05f);
      myStrip.FillRange(full);
      if (full.color.brightness >= 1) doFill = false;
    }
    else if (full.color.brightness > 0)
    {
      full.color.SetBrightness(full.color.brightness -= 0.05f);
      myStrip.FillRange(full);
    }

    intervalTimer = 0;
    myStrip.FillRange(range1);
    GoRandom(range1);

    for (auto &spread : spreads) 
    {
      myStrip.Spread(spread, true, 3);
    }

    spreads.erase(remove_if(spreads.begin(), spreads.end(),
    [&](Range range) 
    {
      return !range.GetLifeTime();
    }), spreads.end());
  }

  InputCommands();

  myStrip.Update();
}



void InputCommands()
{
  if (!Serial.available()) return;

  String command = "";
  while (Serial.available()) 
  {
    int read = Serial.read();
    if (read != 10 && read != 13) // cuz 10/13 === return
      command += (char)read;
  }

  if (command == "Spread")
  {
    Range temp = Range(range1.color, range1.walkIterator, 0, LED_AMOUNT, 7);
    temp.SetLifeTime(LED_AMOUNT);
    spreads.push_back(temp);
  }
  else if (command == "Fill" && !doFill)
  {
    full.color = range1.color;
    doFill = true;
  }
  // asuming it's a number
  else
  {
    int number = command.toInt();
    if (number >= 0 && number <= 360)
    {
      int value = map(command.toInt(), 0, 360, 0, LED_AMOUNT);    
      range1.walkIterator = value;
    }
  }
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
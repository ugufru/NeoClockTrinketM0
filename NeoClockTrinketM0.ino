
// Flashing variation; only the cardinal.
// For 5v Trinket M0, PCF8523 RTC, and 12 Neopixel Ring

#include <Adafruit_DotStar.h>
#include <Adafruit_NeoPixel.h>
#include <SPI.h>
#include <Wire.h>
#include <TimeLib.h>
#include "RTClib.h"

#define BUTTON_1_PIN 1
#define NEOPIXEL_PIN 3
#define BUTTON_2_PIN 4
#define DOTSTAR_DATA_PIN 7
#define DOTSTAR_CLOCK_PIN 8

#define NEOPIXEL_COUNT 12
#define NEOPIXEL_BRIGHTNESS 64

RTC_PCF8523 rtc;

int hh = 0;
int mm = 0;
int ss = 0;

Adafruit_DotStar dotstar = Adafruit_DotStar(1, DOTSTAR_DATA_PIN, DOTSTAR_CLOCK_PIN, DOTSTAR_BGR);
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NEOPIXEL_COUNT, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

byte hue = 0;
byte offset = 4;
byte offset2 = 3;

int loopCount = 0;


void setup()
{
  initSerial();
  initButtons();
  initNeoPixels();
  initRTC();
}

void initButtons()
{
  pinMode(BUTTON_1_PIN, INPUT_PULLUP);
  pinMode(BUTTON_2_PIN, INPUT_PULLUP);
}

void initSerial()
{
  Serial.begin(9600);
  delay(1000);
  Serial.println("Serial open...");
}

void initNeoPixels()
{
  strip.begin();
  strip.setBrightness(NEOPIXEL_BRIGHTNESS);
  strip.show();

  dotstar.begin();
  dotstar.show();
  dotstar.setBrightness(NEOPIXEL_BRIGHTNESS);
}

void initRTC()
{
  if (rtc.begin() == false) error("Could not initialize RTC.");

  Serial.println("Something, something...");
  Serial.print("rtc.initialized() = ");
  Serial.println(rtc.initialized());
  Serial.print("rtc.lostPower() = ");
  Serial.println(rtc.lostPower());

  if (! rtc.initialized() || rtc.lostPower())
  {
    //   Serial.println("RTC is NOT initialized, let's set the time!");
    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date & time this sketch was compiled
    //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
    //
    // Note: allow 2 seconds after inserting battery or applying external power
    // without battery before calling adjust(). This gives the PCF8523's
    // crystal oscillator time to stabilize. If you call adjust() very quickly
    // after the RTC is powered, lostPower() may still return true.
  }

  // When time needs to be re-set on a previously configured device, the
  // following line sets the RTC to the date & time this sketch was compiled
  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  // This line sets the RTC with an explicit date & time, for example to set
  // January 21, 2014 at 3am you would call:
  // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));

  // When the RTC was stopped and stays connected to the battery, it has
  // to be restarted by clearing the STOP bit. Let's do this to ensure
  // the RTC is running.
  rtc.start();
}

void error(char* message)
{
  Serial.println(message);

  while (true)
  {
    dotstar.setPixelColor(0, 255, 0, 0);
    strip.show();
    delay(50);
    dotstar.setPixelColor(0, 0);
    strip.show();
    delay(50);
  }
}

boolean button1 = true;
boolean button2 = true;

void loop()
{
  loopCount = (loopCount + 1) % 20000;

  if (loopCount == 0)
  {
    button1 = digitalRead(BUTTON_1_PIN);
    button2 = digitalRead(BUTTON_2_PIN);

    //    Serial.print("button 1 = ");
    //    Serial.print(button1);
    //    Serial.print(", button 2 = ");
    //    Serial.println(button2);

    checkHourButton();
    checkRainbowRight();
    updateTime();
  }
}

void checkRainbowLeft()
{
  while (button1 == false)
  {
    doTheRainbowDanceLeft();
    delay(50);
    button1 = digitalRead(BUTTON_1_PIN);
  }
}

void checkHourButton()
{
  if (button1 == false)
  {
    DateTime now = rtc.now();

    int h = now.twelveHour() % 12;

    while (button1 == false)
    {
      h = (h + 1) % 12;

      for (int i = 0; i < 12; i++)
      {
        if (h == i)
        {
          strip.setPixelColor(i, 255, 0, 0);
        }
        else
        {
          strip.setPixelColor(i, 0, 0, 0);
        }
      }
      strip.show();
      delay(500);

      button1 = digitalRead(BUTTON_1_PIN);
    }

    // Set the resulting hour.
    DateTime newTime = DateTime(now.year(), now.month(), now.day(), h, now.minute(), now.second());
    Serial.print("Adjusting RTC time to ");
    Serial.print("h = ");
    Serial.print(newTime.hour());
    Serial.print(", m = ");
    Serial.print(newTime.minute());
    Serial.print(", s = ");
    Serial.println(newTime.second());

    rtc.adjust(newTime);
  }
}

void checkRainbowRight()
{
  while (button2 == false)
  {
    doTheRainbowDanceRight();
    delay(50);
    button2 = digitalRead(BUTTON_2_PIN);
  }
}

void updateTime()
{
  DateTime now = rtc.now();

  int h = now.twelveHour() % 12;
  int m = now.minute();
  int s = now.second();
  int hh = h;
  int mm = m / 5;
  int ss = s / 5;

  //  Serial.print("h = ");
  //  Serial.print(h);
  //  Serial.print(", m = ");
  //  Serial.print(m);
  //  Serial.print(", s = ");
  //  Serial.println(s);
  //    Serial.print(" -|- ");
  //    Serial.print("hh = ");
  //    Serial.print(hh);
  //    Serial.print(", mm = ");
  //    Serial.print(mm);
  //    Serial.print(", ss = ");
  //    Serial.println(ss);

  // The cardinal is white and flashes.

  for (int i = 0; i < 12; i++)
  {
    if (s % 2 == 0)
    {
      if (i == 0)
      {
        strip.setPixelColor(i, 100, 100, 100);
      }
      else
      {
        if (i == ss)
        {
          int v = (s - (ss * 5)) * 32 + 64;
          strip.setPixelColor(i, 0, 0, v);
        }
        else
        {
          if (i == mm)
          {
            strip.setPixelColor(i, 0, 255, 0);
          }
          else
          {
            if (i == hh)
            {
              strip.setPixelColor(i, 255, 0, 0);
            }
            else
            {
              strip.setPixelColor(i, 0, 0, 0);
            }
          }
        }
      }
    }
    else
    {
      if (i == hh)
      {
        strip.setPixelColor(i, 255, 0, 0);
      }
      else
      {
        if (i == mm)
        {
          strip.setPixelColor(i, 0, 255, 0);
        }
        else
        {
          if (i == ss)
          {
            int v = (s - (ss * 5)) * 32 + 64;
            strip.setPixelColor(i, 0, 0, v);
            //            strip.setPixelColor(i, 0, 0, 255);
          }
          else
          {
            if ((i > 0) && (i <= hh))
            {
              strip.setPixelColor(i, 0, 0, 0);
            }
            else
            {
              strip.setPixelColor(i, 0, 0, 0);
            }
          }
        }
      }
    }
  }
  strip.show();
}

void doTheRainbowDanceLeft()
{
  pushPixelLeft(Wheel(hue));

  hue = (hue + offset) % 0xFF;

  offset = (offset + offset2) % 0x40;
  offset2 = (offset2 + 1) % 0x20;
}

void doTheRainbowDanceRight()
{
  pushPixelRight(Wheel(hue));

  hue = (hue + offset) % 0xFF;

  offset = (offset + offset2) % 0x40;
  offset2 = (offset2 + 1) % 0x20;
}

void pushPixelLeft(uint32_t color)
{
  uint16_t count = strip.numPixels();

  for (uint16_t i = 0; i < count - 1; i++)
  {
    uint32_t c = strip.getPixelColor(i + 1);
    strip.setPixelColor(i, c);
  }

  strip.setPixelColor(count - 1, color);
  strip.show();
}

void pushPixelRight(uint32_t color)
{
  uint16_t count = strip.numPixels();

  for (uint16_t i = 0; i < count - 1; i++)
  {
    int v = count - i - 1;
    uint32_t c = strip.getPixelColor(v - 1);
    strip.setPixelColor(v, c);
  }

  strip.setPixelColor(0, color);
  strip.show();
}

uint32_t Wheel(byte WheelPos)
{
  if (WheelPos < 85)
  {
    return strip.gamma32(strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0));
  }
  else if (WheelPos < 170)
  {
    WheelPos -= 85;
    return strip.gamma32(strip.Color(255 - WheelPos * 3, 0, WheelPos * 3));
  }
  else
  {
    WheelPos -= 170;
    return strip.gamma32(strip.Color(0, WheelPos * 3, 255 - WheelPos * 3));
  }
}

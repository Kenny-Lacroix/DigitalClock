#include <Arduino.h>
#include "RTClib.h"
#include <FastLED.h>
#include <TM1637Display.h>

#define LED_PIN 7
#define CLK_PIN 2
#define DIO_PIN 3
#define NUM_LEDS 120

CRGB leds[NUM_LEDS];
RTC_DS3231 rtc;
TM1637Display display = TM1637Display(CLK_PIN, DIO_PIN);

void setup()
{
  rtc.begin();

  display.clear();
  display.setBrightness(2);

  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(5);

  // Check if the RTC lost power and if so, set the time to when sketch was compiled:
  if (rtc.lostPower())
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}

void loop()
{
  // Get current date and time:
  DateTime now = rtc.now();

  // Display the current time in 24 hour format with leading zeros enabled and a center colon:
  int displaytime = (now.hour() * 100) + now.minute();
  display.showNumberDecEx(displaytime, 0b11100000, true);

  // make every 5s red
  if (now.second() % 5 || now.second() == 0)
    leds[now.second()] = CRGB(0, 0, 255);
  else
    leds[now.second()] = CRGB(255, 0, 0);
  FastLED.show();
}
/* SparkFun WS2812 Breakout Board Example
  SparkFun Electronics
  date: July 25, 2013
  license: GNU GENERAL PUBLIC LICENSE

  Requires the Adafruit NeoPixel library. It's awesome, go get it.
  https://github.com/adafruit/Adafruit_NeoPixel

  This simple example code runs three sets of animations on a group of WS2812
  breakout boards. The more boards you link up, the better these animations
  will look.

  For help linking WS2812 breakouts, checkout our hookup guide:
  https://learn.sparkfun.com/tutorials/ws2812-breakout-hookup-guide

  Before uploading the code, make sure you adjust the two defines at the
  top of this sketch: PIN and LED_COUNT. Pin should be the Arduino pin
  you've got connected to the first pixel's DIN pin. By default it's
  set to Arduino pin 4. LED_COUNT should be the number of breakout boards
  you have linked up.
*/
#include <Adafruit_NeoPixel.h>
#include "WS2812_Definitions.h"
#include <Wire.h>

#define PIN 6
#define LED_COUNT 62
bool rainbowActive = false;
// Create an instance of the Adafruit_NeoPixel class called "leds".
// That'll be what we refer to from here on...
Adafruit_NeoPixel leds = Adafruit_NeoPixel(LED_COUNT, PIN, NEO_GRB + NEO_KHZ800);

void setup()
{
  leds.begin();  // Call this to start up the LED strip.
  clearLEDs();   // This function, defined below, turns all LEDs off...
  leds.show();   // ...but the LEDs don't actually update until you call this.
}

void loop()
{
  if (digitalRead(0) == 1) { //rainbow
    rainbowCycleTick();
    rainbowActive = true;
  }

  else if (digitalRead(1) == 1) { //purple
    solid(0x7F, 0x0, 0x7F);
    rainbowActive = false;
  }

  else if (digitalRead(2) == 1) { //orange
    solid(0x7f, 0x1D, 0x0);
    rainbowActive = false;
  }

  else if (digitalRead(3) == 1) { //blue
    solid(0x0, 0x0, 0x7f);
    rainbowActive = false;
  }

  else if (digitalRead(4) == 1) { //green
    solid(0x0, 0x7F, 0x0);
    rainbowActive = false;
  }

  else if (digitalRead(5) == 1) { //red
    rainbowActive = false;
  }

  else if (digitalRead(6) == 1) { //orange and blue
    orangeAndBlue();
    rainbowActive = false;
  }

  if (rainbowActive) {
    rainbowCycleTick();
  }
}

void solid(byte red, byte green, byte blue) {
  clearLEDs();
  for (int i = 0; i < LED_COUNT; i++) {
    leds.setPixelColor(i, red, green, blue);
  }
  leds.show();
}

void eatBlue() {
  for (int i = 0; i < LED_COUNT; i++) {
    for (int boop = 0; boop < i - 2; boop++) {
      leds.setPixelColor(boop, 0x00, 0x00, 0x7F);
    }
    for (int boop = i - 1; boop > i - 2 && boop < i + 2; boop++) {
      leds.setPixelColor(boop, 0xFF, 0x20, 0X00);
    }
    for (int boop = i + 1; boop > i + 2; boop++) {
      leds.setPixelColor(boop, 0x00, 0x00, 0x7F);
    }
    leds.show();
  }
}

// Implements a little larson "cylon" sanner.
// This'll run one full cycle, down one way and back the other

// Cascades a single direction. One time.
void cascade(unsigned long color, byte direction, byte wait)
{
  if (direction == TOP_DOWN)
  {
    for (int i = 0; i < LED_COUNT; i++)
    {
      clearLEDs();  // Turn off all LEDs
      leds.setPixelColor(i, color);  // Set just this one
      leds.show();
      delay(wait);
    }
  }
  else
  {
    for (int i = LED_COUNT - 1; i >= 0; i--)
    {
      clearLEDs();
      leds.setPixelColor(i, color);
      leds.show();
      delay(wait);
    }
  }
}

// Sets all LEDs to off, but DOES NOT update the display;
// call leds.show() to actually turn them off after this.
void clearLEDs()
{
  for (int i = 0; i < LED_COUNT; i++)
  {
    leds.setPixelColor(i, 0);
  }

}

void blueLEDs()
{
  for (int i = 0; i < LED_COUNT; i++)
  {
    leds.setPixelColor(i, 0x00007F);
  }
}

void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for (j = 0; j < 256 * 5; j++) { // 5 cycles of all colors on wheel
    for (i = 0; i < leds.numPixels(); i++) {
      leds.setPixelColor(i, Wheel(((i * 256 / leds.numPixels()) + j) & 255));
    }
    leds.show();
    delay(wait);
  }
}

void rainbowCycleTick() {
  int i;
  static int rainbowInc;

  for (i = 0; i < leds.numPixels(); i++) {
    // tricky math! we use each pixel as a fraction of the full 96-color wheel
    // (thats the i / strip.numPixels() part)
    // Then add in j which makes the colors go around per pixel
    // the % 96 is to make the wheel cycle around
    leds.setPixelColor(i, Wheel( ((i * 256 / leds.numPixels()) + rainbowInc) % 256) );
  }
  leds.show();   // write all the pixels out

  rainbowInc++;
}

void orangeAndBlue() {
  clearLEDs();
  bool blue = true;
  for (int i = 0; i < LED_COUNT; i++) {
    if (i % 5 == 0) {
      blue = !blue;
    }
    if (blue) {
      leds.setPixelColor(i, 178, 25, 0);
    }
    else {
      leds.setPixelColor(i, 0, 0, 178);
    }
  }
  leds.show();
}

uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return leds.Color((255 - WheelPos * 3) / 2, 0, (WheelPos * 3) / 2);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return leds.Color(0, (WheelPos * 3) / 2, (255 - WheelPos * 3) / 2);
  }
  WheelPos -= 170;
  return leds.Color((WheelPos * 3) / 2, (255 - WheelPos * 3) / 2, 0);
}

/**
  \file Mega_Calculator.ino
  \brief Simulator of Standard 9 Digit Calculator with decimal points.
  \remarks comments are implemented with Doxygen Markdown format
  \author  Michael Flaga, michael@.flaga.net
*/

#include <Adafruit_NeoPixel.h>
#define LED_PIN    A0

#define DISPLAY_SIZE 9

Adafruit_NeoPixel* strip;

#include "LEDdisplay.h"
LEDdisplay LEDdisplay(DISPLAY_SIZE);

#include "Calculator.h"
Calculator Calculator(DISPLAY_SIZE);

void setup() {
  Serial.begin(115200); // enable port for debug
  Serial.println("Mega Calculator is starting");

  strip = new Adafruit_NeoPixel((DISPLAY_SIZE * LEDdisplay.ledsPerDigit()), LED_PIN, NEO_GRB + NEO_KHZ800);
  strip->begin();

  strip->clear();
  strip->show();

  strip->setBrightness(255 * .9);
  LEDdisplay.begin(strip, Serial);
  LEDdisplay.setColor(255, 255, 255);

  Calculator.begin();
  LEDdisplay.printTest();

  LEDdisplay.setColor(0, 255, 0);
  LEDdisplay.printDisplay("0.");


} // setup() - end

void loop() {

  char displayStr[DISPLAY_SIZE + 2]; // 1 extra for Decimal and 1 extra for the str null terminator.

  while (Serial.available()) {
    Calculator.parse(displayStr, Serial.read());
    Serial.print("displayStr["); Serial.print(strlen(displayStr)); Serial.print("]=\"");  Serial.print(displayStr); Serial.println("\"");
    LEDdisplay.printDisplay(displayStr);
  }
} // loop() - end

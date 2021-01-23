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

#include "Debouncer.h"
#define LENGTH_OF_ARRAY(x) ((sizeof(x)/sizeof(x[0])))

typedef struct {
  int  name;
  char character;
} Button;

Button pins[] = {
  // displayed in order as seen from Boards Pinout.
  {21, '='},  
  {20, '+'},  
  {19, 'c'},  
  {18, '7'},  
  {17, '4'},  
  {16, '1'},  
  {15, 'n'},  
  {14, '0'},  
//{ 0, 'R'}, // Rx of Serial0, needed for IDE.
//{ 1, 'T'}, // Tx of Serial0
  { 2, '2'},  
  { 3, '5'},  
  { 4, '8'},  
  { 5, '('},  
  { 6, '%'},  
  { 7, '9'},  
  { 8, '6'},  
  { 9, '3'},  
  {10, '.'}, 
  {11, '/'},  
  {12, '*'},  
  {13, '-'}   
};

int debounce_duration_ms = 50;

Debouncer* debounced[LENGTH_OF_ARRAY(pins)];

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

  // initialize Buttons with an array of debouncers.
  for (int position = 0; position < LENGTH_OF_ARRAY(pins); position++) {
    pinMode(pins[position].name, INPUT_PULLUP);
    debounced[position] = new Debouncer(pins[position].name, debounce_duration_ms, Debouncer::Active::L, Debouncer::DurationFrom::TRIGGER);
  }

} // setup() - end

void loop() {

  char displayStr[DISPLAY_SIZE + 2]; // 1 extra for Decimal and 1 extra for the str null terminator.

  // get keys from debounced buttons
  for (int position = 0; position < LENGTH_OF_ARRAY(pins); position++) {
    debounced[position]->update();
    if (debounced[position]->edge())
    {
      Serial.print("Button '");
      Serial.print(pins[position].character);
      Serial.print("' edge detect: ");
      if (debounced[position]->rising())
      {
        Serial.println("released");
      }
      if (debounced[position]->falling())
      {
        Serial.println("pressed");
        
        // process button character into calculator and display
        Calculator.parse(displayStr, pins[position].character);
        Serial.print("displayStr["); Serial.print(strlen(displayStr)); Serial.print("]=\"");  Serial.print(displayStr); Serial.println("\"");
        LEDdisplay.printDisplay(displayStr);
      }
    }
  }
  
  // get keys from serial port
  while (Serial.available()) {
    
    // process key character into calculator and display
    Calculator.parse(displayStr, Serial.read());
    Serial.print("displayStr["); Serial.print(strlen(displayStr)); Serial.print("]=\"");  Serial.print(displayStr); Serial.println("\"");
    LEDdisplay.printDisplay(displayStr);
  }
} // loop() - end

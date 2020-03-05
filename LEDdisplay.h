/**
  \file LEDdisplay.h
  \brief LED Driver of NeoPixels in 7 segment arrangement.
  \remarks comments are implemented with Doxygen Markdown format
  \author  Michael Flaga, michael@flaga.net
*/

#ifndef IFDEBUG
#define IFDEBUG(...) ((void)((DEBUG_LEVEL) && (__VA_ARGS__, 0)))
#define DEBUG_LEVEL 0 // set to 1 to compile in Serial Debug prints
#endif
#include <Adafruit_NeoPixel.h>
#ifndef LEDdisplay_h

#define LEDdisplay_h
#include "Arduino.h"

class LEDdisplay {

  public:

    LEDdisplay(int pinOne);
    ~LEDdisplay();
    void begin(Adafruit_NeoPixel *strip);
    void begin(Adafruit_NeoPixel *strip, Stream &serial);
    int ledsPerDigit();
    void set1Digit(int8_t digit = 0, int pos = 0, bool dp = false );
    void printDisplay(char *str);
    void printTest();
    void setColor(int red, int green, int blue);

  private:
    int _displaySize;
    Stream* _serial = &Serial;
    Adafruit_NeoPixel *_strip;
    int _red;
    int _green;
    int _blue;
};

#endif

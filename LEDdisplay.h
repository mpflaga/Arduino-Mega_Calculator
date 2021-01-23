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

/**
   \brief decode map LEDs to turn on or off for 0-9.

   Stored into Flash memory and referenced by pointer for direct access.
   The index of the array maps LEDs for the corresponding 7-Segment+DP.
*/
static const byte digit_array[][29] PROGMEM = {
  /* REF	{F,F,F,F,A,A,A,A,B,B,B,B,C,C,C,C,D,D,D,D,E,E,E,E,G,G,G,G,DP},*/
  /* 0 */	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
  /* 1 */	{0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  /* 2 */	{0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
  /* 3 */	{0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0},
  /* 4 */	{1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0},
  /* 5 */	{1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0},
  /* 6 */	{1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
  /* 7 */	{0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  /* 8 */	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
  /* 9 */	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0}
};

/**
 * \class LEDdisplay
 * \brief Interface Driver to the f NeoPixels in 7 segment arrangement.
 */
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

/**
  \file LEDdisplay.cpp
  \brief LED Driver of NeoPixels in 7 segment arrangement.
  \remarks comments are implemented with Doxygen Markdown format
  \author  Michael Flaga, michael@flaga.net
*/

#include "Arduino.h"
#include "LEDdisplay.h" 
#include <avr/pgmspace.h>

/**
 * \brief decode map LEDs to turn on or off for 0-9.
 *
 * Stored into Flash memory and referenced by pointer for direct access.
 * The index of the array maps LEDs for the corresponding 7-Segment+DP. 
 */
static const byte digit_array[][29] PROGMEM = {
/* REF	{F,F,F,F,A,A,A,A,B,B,B,B,C,C,C,C,D,D,D,D,E,E,E,E,G,G,G,G,DP},*/
/* 0 */	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0},
/* 1 */	{0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
/* 2 */	{0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0},
/* 3 */	{0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,0},
/* 4 */	{1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,0},
/* 5 */	{1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,0},
/* 6 */	{1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
/* 7 */	{0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
/* 8 */	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
/* 9 */	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,0}
};

/**
 * \brief Constructor
 * \param[in] int Number of Digits in the Display.
 *
 * reserve memory
 */
LEDdisplay::LEDdisplay(int displaySize){
  //IFDEBUG(_serial->println("LEDdisplay Driver Constructed");
  _displaySize = displaySize;
}

/**
 * \brief Destructortructor
 *
 * release alocated memory ,if any.
 */
LEDdisplay::~LEDdisplay(){
  IFDEBUG(_serial->println());
}

/**
 * \brief Initialize the LEDdisplay
 *
 * initialize LEDdisplay along with default private values and pointers.
 */
 void LEDdisplay::begin(Adafruit_NeoPixel *strip){
  _strip = strip;
  _red = 255 / 2;
  _green = 255 / 2;
  _blue = 255 / 2;  
}

/**
 * \brief Initialize the LEDdisplay
 *
 * initialize LEDdisplay with a none default Serial Stream for debug.
 */
void LEDdisplay::begin(Adafruit_NeoPixel *strip, Stream &serial){
  begin(_strip);
  IFDEBUG(_serial = &serial);
  _strip = strip;
  IFDEBUG(_serial->print("LEDdisplay Driver Started! Display size is "));
  IFDEBUG(_serial->print(_displaySize));
  IFDEBUG(_serial->println());
}

/**
 * \brief Get the number of LEDs per stored Digit.
 * \param[out] int
 *
 * Returns an integer number indicating the number of 
 * NeoPixel LEDs are used in each 7-Segment including the Decimal Point.
 */
int LEDdisplay::ledsPerDigit(){
  IFDEBUG(_serial->print("sizeof(digit_array[0])='")); IFDEBUG(_serial->print(sizeof(digit_array[0]))); IFDEBUG(_serial->println("' "));
  IFDEBUG(_serial->print("sizeof(digit_array)='")); IFDEBUG(_serial->print(sizeof(digit_array))); IFDEBUG(_serial->println("' "));

  return sizeof(digit_array[0]);
}

/**
 * \brief Print text string to the display.
 * \param[in] int8_t 0-9
 * \param[in] int    position
 * \param[in] bool   decimal point
 *
 * Puts decimal onto the LED displays at an assigned position. 
 * Allowing the Decimal to be enabled or disabled.
 */
void LEDdisplay::set1Digit(int8_t digit = 0, int pos = 0, bool dp = false ) {
    for(int led_pos=0; led_pos < 29; led_pos++) { // this will cycle from 0 through 260, then not do if 261.

      if (digit >= sizeof(digit_array)) { // blank the digit if not defined
        _strip->setPixelColor( (led_pos + (29 * pos) ), _strip->Color(0,   0,   0));

      } else if (pgm_read_byte_near( &(digit_array[digit][led_pos]))) {//directly read and test the Flash Memory.
        _strip->setPixelColor( (led_pos + (29 * pos) ), _strip->Color(_red, _green, _blue));
      }
    }
    if (dp == true) {
      _strip->setPixelColor(((29 * pos) + 28), _strip->Color(_red, _green, _blue));
    } else {
      _strip->setPixelColor(((29 * pos) + 28), _strip->Color(0,   0,   0));
    }
}

/**
 * \brief Print text string to the display.
 * \param[in] char* string
 *
 * puts a string right justified onto the LED Display and parses Decimal points.
 */
void LEDdisplay::printDisplay(char *inputStr) {

  int strSize = strlen(inputStr);
  _strip->clear();
  
  IFDEBUG(_serial->print("strSize='")); IFDEBUG(_serial->print(strSize)); IFDEBUG(_serial->println("'"));
  for (int displayPos = _displaySize - 1, strPos = strSize - 1 ; displayPos >= 0; displayPos--) {
   
    if ((( '0' <= inputStr[strPos]) && (inputStr[strPos] <= '9' ) || (inputStr[strPos] == '.' )) && (strPos >= 0)) {
      bool dp = false;
      if (inputStr[strPos] == '.' ) {
        dp = true;
        IFDEBUG(_serial->println(" Period Detected "));
        strPos--;
      }
      int8_t strInt = inputStr[strPos] - '0';
      set1Digit(strInt, displayPos, dp );
      
      IFDEBUG(_serial->print("displayPos='")); IFDEBUG(_serial->print(displayPos)); IFDEBUG(_serial->print("' "));
      IFDEBUG(_serial->print("inputStr[")); IFDEBUG(_serial->print(strPos)); IFDEBUG(_serial->print("]=\"")); IFDEBUG(_serial->print(inputStr[strPos])); IFDEBUG(_serial->print("\", "));
      
      IFDEBUG(_serial->print(" '0x")); IFDEBUG(_serial->print(strInt, HEX)); IFDEBUG(_serial->print("', "));
      
      strPos--;
    } else {
      IFDEBUG(_serial->print("displayPos='")); IFDEBUG(_serial->print(displayPos)); IFDEBUG(_serial->print("'"));
      set1Digit(0xFF, displayPos, false); // clear the Digit
      IFDEBUG(_serial->print("<blank>"));
    }
    IFDEBUG(_serial->println());
  }
  _strip->show(); 
}

/**
 * \brief Set the Color of the Display.
 * \param[in] int Red 0-255
 * \param[in] int Green 0-255
 * \param[in] int Blue 0-255
 *
 * Assign the RGB color for the display going forward for next print.
 */
void LEDdisplay::setColor(int red, int green, int blue){
  _red = red;
  _green = green;
  _blue = blue;
}

/**
 * \brief Pre-Operation Self Test aka POST.
 *
 * Lite up ALL LEDs R, G and B, then 0-9 as White.
 */
 void LEDdisplay::printTest(){

  _strip->clear();

  char* testPattern;
  testPattern = new (char [_displaySize + 1]);
  memset(testPattern, '8', _displaySize);
  testPattern[_displaySize] = NULL;

  setColor(255, 0, 0);
  printDisplay(testPattern);
  delay(1000);  
  setColor(0, 255, 0);
  printDisplay(testPattern);
  delay(1000);
  setColor(0, 0, 255);
  printDisplay(testPattern);
  delay(1000);  

  _strip->clear();
  setColor(255, 255, 255);

  for(int8_t number = 0; number < (_displaySize); number++)
  {
  
    if (number % 2) //if odd
    {
        set1Digit(number, number, false);
    }                     
    else                  
    {                     
        set1Digit(number, number, true);
    }
  }
  _strip->show();
  delay(1000);
  _strip->clear();
  _strip->show();

}
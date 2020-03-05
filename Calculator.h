/**
  \file Calculator.cpp
  \brief Simulator of Standard 9 Digit Calculator with decimal points.
  \remarks comments are implemented with Doxygen Markdown format
  \author  Michael Flaga, michael@flaga.net
*/

#include "BigNumber.h"

#define IFDEBUG(...) ((void)((DEBUG_LEVEL) && (__VA_ARGS__, 0)))

#define DEBUG_LEVEL 0 // set to 1 to compile in Serial Debug prints

class Calculator {
  private:
    byte lastKeyWasAnOperation;
    char operationChar;
    bool noNewNumberSinceLastCalculation;
    char* displayStr;
    int _displayStrSize;
    char* numStr0;
    char* numStr1;
    int _numStrSize;
    void zeroStr(char *dest, int length);

  public:
    Calculator(int _size);
    ~Calculator();
    void begin();
    void parse(char *output, char inByte);
};

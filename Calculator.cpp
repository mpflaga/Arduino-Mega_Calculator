/**
  \file Calculator.cpp
  \brief Simulator of Standard 9 Digit Calculator with decimal points.
  \remarks comments are implemented with Doxygen Markdown format
  \author  Michael Flaga, michael@.flaga.net
*/

#include "Calculator.h"

/**
   \brief Constructor
   \param[in] size of memory to allocate for char arrays.

   reserve memory
*/
Calculator::Calculator(int _size) {

  // allocate memory for char arrays.
  _displayStrSize = _size + 2;
  displayStr = new (char [_displayStrSize]); // add two, one each for decimal and str NULL terminator

  _numStrSize = _size + 2;
  numStr0 = new (char [_numStrSize]); // add two, one each for decimal and str NULL terminator
  numStr1 = new (char [_numStrSize]); // add two, one each for decimal and str NULL terminator

  // initialize internal variables.
  byte lastKeyWasAnOperation = 0;
  char operationChar = NULL;
  bool noNewNumberSinceLastCalculation = false;
  BigNumber::begin (_size - 1); // account for zero infront of decimal point.
}

/**
   \brief Destructortructor

   release alocated memory.
*/
Calculator::~Calculator() {
  free(numStr0);
  free(numStr1);
}

/**
   \brief Initialize the Calculator
   initialize Char Arraysy
*/
void Calculator::begin() {
  zeroStr(displayStr, _displayStrSize);
  zeroStr(numStr0, _numStrSize);
  zeroStr(numStr1, _numStrSize);
}

/**
   \brief Clear out a Char Array
   \param[in] pointer to target char arrays.
   \param[in] size of char array.

   Fills 0x00(aka NULL) from start through length.
*/
void Calculator::zeroStr(char *dest, int length) {
  // set entire array to NULL.
  memset(dest, NULL, length);
  dest[0] = '0'; // initial value is Zero Text.
  dest[1] = '.'; // initial value is Zero Text.
}

/**
   \brief Parse A digit into the Calculator.
   \param[in] an ASCII Char value. [0-9+-*\/nCc]

   Processes the character as a keypad button on a calculator.
   0-9 are appended to displayed value.

   One of the following operations loads the current Entry into numStr0
   '+' is addition operation
   '-' is subtraction operation
   '*' is multiplaction operation
   '\/' is divide operation
   'n' is Negative operation
   'C' is clear all accumlators and operations.
   'c' is clear current Entry, typically maps to the CE button.

   '=' loads the current Entry into numStr1 and performs the
       prior entered operation. Loading the resultant into numStr0
       and moving numStr0 into numStr1.
       This allows successive '='s to simply re-produce the last operation
*/
void Calculator::parse(char *output, char inByte) {

  int displayStrLength = strlen(displayStr);

  if ((inByte == '+') || (inByte == '-') || (inByte == '*') || (inByte == '/')) {
    operationChar = inByte;
    lastKeyWasAnOperation = 1;
    IFDEBUG(Serial.print("Detected operationChar =\"")); IFDEBUG(Serial.print(operationChar)); IFDEBUG(Serial.println("\""));

    // move current number into position.
    strcpy(numStr0, displayStr);

  } else if (inByte == '=') {
    IFDEBUG(Serial.println("equal detected"));
    // move current number into position.

    if (!noNewNumberSinceLastCalculation) { // if not just previously completed then load new value.
      strcpy(numStr1, displayStr);
    }
    noNewNumberSinceLastCalculation = true;

    // primary input buffer for next number.
    zeroStr(displayStr, _displayStrSize);

    BigNumber resultant;
    bool didCalulation = false;
    if (operationChar == '+') {
      IFDEBUG(Serial.println("Adding"));
      resultant = BigNumber (numStr0) + BigNumber (numStr1);
      didCalulation = true;

    } else if (operationChar == '-') {
      IFDEBUG(Serial.println("subtracting"));
      resultant = BigNumber (numStr0) - BigNumber (numStr1);
      didCalulation = true;

    } else if (operationChar == '*') {
      IFDEBUG(Serial.println("Multiplying"));
      resultant = BigNumber (numStr0) * BigNumber (numStr1);
      resultant = resultant + BigNumber("0"); // for some dumb reason BigNumber does not show decimal after a multiply, so adding it for consistency.

      didCalulation = true;

    } else if (operationChar == '/') {
      IFDEBUG(Serial.println("Dividing"));
      resultant = BigNumber (numStr0) / BigNumber (numStr1);
      didCalulation = true;

    }

    if (didCalulation == true) {
      char * tempChar = resultant.toString();
      tempChar[_numStrSize - 1] = NULL; // need to size the new string to proper length.
      strcpy(numStr0, tempChar);

      //remove Zero Padding. Note resultant string will always have decimal point.
      for (int8_t i = (_numStrSize - 2) ; ((i > 0) && (numStr0[i] == '0')); i--) {
        numStr0[i] = NULL;
      }

      strcpy(displayStr, numStr0); // copy it to the display
    }

  } else if (inByte == 'b') {
    IFDEBUG(Serial.println("detected Backspace"));
    displayStr[displayStrLength - 1] = NULL;

  } else if (inByte == 'c') {
    IFDEBUG(Serial.println("detected Clear Entry"));
    zeroStr(displayStr, _displayStrSize);

  } else if (inByte == 'C') {
    IFDEBUG(Serial.println("detected Clear all input"));

    zeroStr(displayStr, _displayStrSize);
    zeroStr(numStr0, _displayStrSize);
    zeroStr(numStr1, _displayStrSize);


    lastKeyWasAnOperation = 0;
    operationChar = NULL;
    noNewNumberSinceLastCalculation = false;

  } else if (inByte == 'n') {
    IFDEBUG(Serial.println("detected -"));
    if (displayStr[0] == '0') {
      displayStr[0] = '-';
    } else if (displayStr[0] == '-') {
      for (int8_t i = 0; i < _displayStrSize - 1; i++) { // remove negative by shifting displayStr left one digit.
        displayStr[i] = displayStr[i + 1];
      }
      displayStr[_displayStrSize - 1] = NULL;
    } else {
      // the number is originally positive
      for (int8_t i = _displayStrSize - 1; i > 0; i--) { // add negative by shifting displayStr right one digit.
        displayStr[i] = displayStr[i - 1];
      }
      displayStr[0] = '-';
      displayStr[_displayStrSize - 1] = NULL;
    }
  } else if ((displayStrLength < (_displayStrSize - 1)) && ( '0' <= inByte) && (inByte <= '9' )) {
    // if there is room to add the digit.
    IFDEBUG(Serial.println("sensed a digit and there is room."));

    if (lastKeyWasAnOperation) {
      IFDEBUG(Serial.println("clearing prior work from display."));
      zeroStr(displayStr, _displayStrSize);

      lastKeyWasAnOperation = 0;
    }

    if (noNewNumberSinceLastCalculation) {
      IFDEBUG(Serial.println("clearing prior resultant from display."));
      zeroStr(displayStr, _displayStrSize);

      noNewNumberSinceLastCalculation = false;
    }

    if ((displayStr[0] == '0') && (strlen(displayStr) == 1)) { // remove Zero Place Holder if needed.
      IFDEBUG(Serial.println("changing prior zero to new byte."));
      displayStr[0] = inByte;
      displayStr[1] = '.';
    } else {
      IFDEBUG(Serial.println("adding new byte to end of display str."));

      // convert it to a number and back to get decimal.
      BigNumber tempNumber = (BigNumber(displayStr) * BigNumber(10)) + BigNumber(inByte - '0');
      char * tempChar = tempNumber.toString();
      tempChar[_displayStrSize - 1] = NULL; // need to size the new string to proper length.
      strcpy(displayStr, tempChar);

      //remove Zero Padding. Note resultant string will always have decimal point.
      for (int8_t i = (_displayStrSize - 2) ; ((i > 0) && (displayStr[i] == '0')); i--) {
        displayStr[i] = NULL;
      }
    }
  }
  strcpy(output, displayStr);

  IFDEBUG(Serial.print("displayStr[")); IFDEBUG(Serial.print(strlen(displayStr))); IFDEBUG(Serial.print("]=\"")); IFDEBUG(Serial.print(displayStr)); IFDEBUG(Serial.print("\",")); IFDEBUG(Serial.print("(")); IFDEBUG(Serial.print(BigNumber (displayStr))); IFDEBUG(Serial.print(") "));

  IFDEBUG(Serial.print("numStr0[")); IFDEBUG(Serial.print(strlen(numStr0))); IFDEBUG(Serial.print("]=\"")); IFDEBUG(Serial.print(numStr0)); IFDEBUG(Serial.print("\",")); IFDEBUG(Serial.print("(")); IFDEBUG(Serial.print(BigNumber (numStr0))); IFDEBUG(Serial.print(") "));

  IFDEBUG(Serial.print("numStr1[")); IFDEBUG(Serial.print(strlen(numStr1))); IFDEBUG(Serial.print("]=\"")); IFDEBUG(Serial.print(numStr1)); IFDEBUG(Serial.print("\",")); IFDEBUG(Serial.print("(")); IFDEBUG(Serial.print(BigNumber (numStr1))); IFDEBUG(Serial.print(") "));

  IFDEBUG(Serial.print("operationChar = \"")); IFDEBUG(Serial.print(operationChar)); IFDEBUG(Serial.print("\" "));
  IFDEBUG(Serial.print("lastKeyWasAnOperation = \"")); IFDEBUG(Serial.print(lastKeyWasAnOperation)); IFDEBUG(Serial.print("\" "));
  IFDEBUG(Serial.print("..SinceLastC.. = \"")); IFDEBUG(Serial.print(noNewNumberSinceLastCalculation)); IFDEBUG(Serial.print("\" "));

  IFDEBUG(Serial.println());
}

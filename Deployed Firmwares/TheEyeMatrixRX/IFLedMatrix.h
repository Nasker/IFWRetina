#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

#define X_SIZE 8
#define Y_SIZE 8

class IFLedMatrix {
    Adafruit_8x8matrix matrix = Adafruit_8x8matrix();
    uint8_t thresholdValue = 140;
  public:
    IFLedMatrix() {};
    void initSetup() {
      matrix.begin(0x70);
    }

    void displayString(String displayedString) {
      matrix.setTextSize(1);
      matrix.setTextWrap(false);  // we dont want text to wrap so it scrolls nicely
      matrix.setTextColor(LED_ON);
      int8_t loopLength = displayedString.length() * -6;
      for (int8_t x = 10; x >= loopLength; x--) {
        matrix.clear();
        matrix.setCursor(x, 0);
        matrix.print(displayedString);
        matrix.writeDisplay();
        delay(50);
      }
    }

    void setThreshold(uint8_t _thresholdValue) {
      thresholdValue = _thresholdValue;
    }

    void displayMatrix(uint8_t (& grayArray) [X_SIZE * Y_SIZE]) {
      matrix.clear();
      for (int j = 0; j < Y_SIZE; j++) {
        for (int i = 0; i < X_SIZE; i++) {
          bool isLedOn = grayArray[i + j * X_SIZE] > thresholdValue ;
          matrix.drawPixel(i, j, isLedOn);
        }
      }
      matrix.writeDisplay();
    }

    void rotate(int rotationValue) {
      matrix.setRotation(rotationValue);
    }
};

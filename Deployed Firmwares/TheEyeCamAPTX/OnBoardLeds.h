#define BLINK_LED_PIN 2
#define FLASH_LED_PIN 4

class OnBoardLeds {
    bool blinkyLedState = true;
    bool flashLedState = false;
  public:
    OnBoardLeds() {
      pinMode(BLINK_LED_PIN, OUTPUT);
      pinMode(FLASH_LED_PIN, OUTPUT);
      digitalWrite(FLASH_LED_PIN, false);
    }

    void blinky() {
      blinkyLedState = !blinkyLedState;
      digitalWrite(BLINK_LED_PIN, blinkyLedState);
    }

    void switchFlash() {
      flashLedState = !flashLedState;
      digitalWrite(FLASH_LED_PIN, flashLedState);
    }
};

#define BLINK_LED_PIN 2
#define FLASH_LED_PIN 4
#define FLASH_RATE_MS 200

class OnBoardLeds {
    bool blinkyLedState = true;
    unsigned int long millisPast = 0;
  public:
    OnBoardLeds() {
      pinMode(BLINK_LED_PIN, OUTPUT);
      pinMode(FLASH_LED_PIN, OUTPUT);
    }

    void blinky() {
      if(millis() > millisPast + FLASH_RATE_MS){
        blinkyLedState = !blinkyLedState;
        digitalWrite(BLINK_LED_PIN, blinkyLedState);
        millisPast = millis();
      }
    }

    void toggleFlash(bool state){
      digitalWrite(FLASH_LED_PIN, state);
    }
};

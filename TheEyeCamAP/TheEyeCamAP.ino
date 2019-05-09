#include "IFWiFiServer.h"
#include "IFCamera.h"
#include "OnBoardLeds.h"

IFWiFiServer wifiServer;
IFCamera camera;
OnBoardLeds bLeds;

uint8_t cameraGrayArray[X_SIZE * Y_SIZE];


void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  camera.initSetup();
  camera.setThreshold(100);
  wifiServer.initSetup();
  bLeds.toggleFlash(false);
}

void loop() {
  camera.readNProcess(cameraGrayArray);
  camera.printGrayMatrix(cameraGrayArray);
  //camera.printBinaryMatrix(cameraGrayArray);
  wifiServer.updateNSend(cameraGrayArray);
  bLeds.blinky();
}

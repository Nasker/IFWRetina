#pragma once

#include "esp_camera.h"
#include "camera_pins.h"
#include "img_converters.h"
#include "Arduino.h"
//#include "UDPCommunication.h"

#define X_SIZE 8
#define Y_SIZE 8

#define CAMERA_MODEL_AI_THINKER

class IFCamera {
    //int _grayArray[X_SIZE * Y_SIZE];
    int _thresholdValue;
    long unsigned int millisPast;
   // UDPCommunication* udpToMicroMatrix;
   // UDPCommunication* udpToPC;
  public:
    IFCamera();
    void initSetup();
    void setThreshold(int thresholdValue);
    void readNProcess(uint8_t (& grayArray) [X_SIZE * Y_SIZE]);
    void printGrayMatrix(uint8_t (& grayArray) [X_SIZE * Y_SIZE]);
    void printBinaryMatrix(uint8_t (& grayArray) [X_SIZE * Y_SIZE]);
  private:
    void yuvToGrayscale(int width, int height, uint8_t* frameBufferInput, uint8_t* frameBufferGray);
    void frameBufferToArrayMod(int width, int heigh, uint8_t* frameBufferGray, uint8_t (& grayArray) [X_SIZE * Y_SIZE]);
    float fps();
};

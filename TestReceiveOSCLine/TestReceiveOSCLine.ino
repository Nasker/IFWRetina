#include <WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>
#include <OSCBundle.h>
#include <OSCBoards.h>
#include "IFLedMatrix.h"

#define X_SIZE 8
#define Y_SIZE 8
#define COUNTS_TO_SLEEP 20

IFLedMatrix ledMatrix;
WiFiUDP Udp;

const char* ssid = "TheEyeAP";
const char* password = "iswatchingyou";
const int inPort = 1234;
IPAddress local_IP(10, 10, 10, 3);
IPAddress gateway(10, 10, 10, 1);
IPAddress subnet(255, 255, 255, 0);

unsigned long int lastTime = millis();
uint8_t cameraGrayArray[X_SIZE * Y_SIZE];

void setup() {
  Serial.begin(115200);
  ledMatrix.initSetup();
  ledMatrix.rotate(1);
  ledMatrix.displayString("IFAE RETINA");
  if (!WiFi.config(local_IP, gateway, subnet) ) {
    Serial.println("STA Failed to configure");
    ledMatrix.displayString("LINK FAILED!");
  }
  WiFi.begin(ssid, password);
  int countToSleep = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    countToSleep++;
    if (countToSleep >= COUNTS_TO_SLEEP) {
      lostConnectionSleepCall();
    }
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  ledMatrix.displayString("LINK OK!");
  Udp.begin(inPort);
  initArray();
}

void loop() {
  OSCMsgReceive();    //esperem a rebre missatges OSC
  if (millis() > lastTime + 5000) {
    if (WiFi.status() != WL_CONNECTED) {
      ESP.restart();
    }
  }
}

void lostConnectionSleepCall() {
  Serial.println("Can't connect going to sleep");
  ledMatrix.displayString("NO LINK, SLEEP!");
  delay(1000);
  esp_deep_sleep_start();
}

void OSCMsgReceive() {
  OSCBundle bundleIN;       //paquet generic de missatges OSC, pot contenir whatever
  int size = Udp.parsePacket(); //mirem si hi ha dades als paquets, si rebem algo
  if (size > 0) {   //si rebem algo...
    //Serial.printf("Message Received with size: %d\n", size);
    while (size--) {
      bundleIN.fill(Udp.read());
    }
    bundleIN.route("/camera/grayArray/line0", actOnReceiveLine0);
    bundleIN.route("/camera/grayArray/line1", actOnReceiveLine1);
    bundleIN.route("/camera/grayArray/line2", actOnReceiveLine2);
    bundleIN.route("/camera/grayArray/line3", actOnReceiveLine3);
    bundleIN.route("/camera/grayArray/line4", actOnReceiveLine4);
    bundleIN.route("/camera/grayArray/line5", actOnReceiveLine5);
    bundleIN.route("/camera/grayArray/line6", actOnReceiveLine6);
    bundleIN.route("/camera/grayArray/line7", actOnReceiveLine7);
    bundleIN.route("/camera/adjustThreshold", actOnAdjustThreshold);
    bundleIN.route( "/matrix/rotate", actOnRotation);
    ledMatrix.displayMatrix(cameraGrayArray);
  }
}

void actOnRotation(OSCMessage &msg, int addrOffset) {
  Serial.printf("Received control rotation message with value %d\n", msg.getInt(0));
  ledMatrix.rotate(msg.getInt(0));
}

void actOnAdjustThreshold(OSCMessage &msg, int addrOffset) {
  Serial.printf("Received control threshold message with value %d\n", msg.getInt(0));
  ledMatrix.setThreshold(uint8_t(msg.getInt(0)));
}

void actOnReceiveLine0(OSCMessage &msg, int addrOffset) {
  int line = 0;
  //Serial.printf("Received line %d \n", line);
  for (int i = 0; i < X_SIZE; i++) {
    cameraGrayArray[line * X_SIZE + i] = uint8_t(msg.getInt(i));
    //Serial.printf("%d ", line * X_SIZE + i);
  }
  //Serial.println();
  //printArray();
}

void actOnReceiveLine1(OSCMessage &msg, int addrOffset) {
  int line = 1;
  //Serial.printf("Received line %d \n", line);
  for (int i = 0; i < X_SIZE; i++) {
    cameraGrayArray[line * X_SIZE + i] = uint8_t(msg.getInt(i));
    //Serial.printf("%d ", line * X_SIZE + i);
  }
  //Serial.println();
  //printArray();
}

void actOnReceiveLine2(OSCMessage &msg, int addrOffset) {
  int line = 2;
  //Serial.printf("Received line %d \n", line);
  for (int i = 0; i < X_SIZE; i++) {
    cameraGrayArray[line * X_SIZE + i] = uint8_t(msg.getInt(i));
    //Serial.printf("%d ", line * X_SIZE + i);
  }
  //Serial.println();
  //printArray();
}

void actOnReceiveLine3(OSCMessage &msg, int addrOffset) {
  int line = 3;
  //Serial.printf("Received line %d \n", line);
  for (int i = 0; i < X_SIZE; i++) {
    cameraGrayArray[line * X_SIZE + i] = uint8_t(msg.getInt(i));
    //Serial.printf("%d ", line * X_SIZE + i);
  }
  //Serial.println();
  //printArray();
}

void actOnReceiveLine4(OSCMessage &msg, int addrOffset) {
  int line = 4;
  //Serial.printf("Received line %d \n", line);
  for (int i = 0; i < X_SIZE; i++) {
    cameraGrayArray[line * X_SIZE + i] = uint8_t(msg.getInt(i));
    //Serial.printf("%d ", line * X_SIZE + i);
  }
  //Serial.println();
  //printArray();
}

void actOnReceiveLine5(OSCMessage &msg, int addrOffset) {
  int line = 5;
  //Serial.printf("Received line %d \n", line);
  for (int i = 0; i < X_SIZE; i++) {
    cameraGrayArray[line * X_SIZE + i] = uint8_t(msg.getInt(i));
    //Serial.printf("%d ", line * X_SIZE + i);
  }
  //Serial.println();
  //printArray();
}

void actOnReceiveLine6(OSCMessage &msg, int addrOffset) {
  int line = 6;
  //Serial.printf("Received line %d \n", line);
  for (int i = 0; i < X_SIZE; i++) {
    cameraGrayArray[line * X_SIZE + i] = uint8_t(msg.getInt(i));
    //Serial.printf("%d ", line * X_SIZE + i);
  }
  //Serial.println();
  //printArray();
}

void actOnReceiveLine7(OSCMessage &msg, int addrOffset) {
  int line = 7;
  //Serial.printf("Received line %d \n", line);
  for (int i = 0; i < X_SIZE; i++) {
    cameraGrayArray[line * X_SIZE + i] = uint8_t(msg.getInt(i));
    //Serial.printf("%d ", line * X_SIZE + i);
  }
  //Serial.println();
  //printArray();
}

void initArray() {
  for (int j = 0; j < Y_SIZE; j++) {
    for (int i = 0; i < X_SIZE; i++) {
      //cameraGrayArray[i + j * X_SIZE] = 0;
      cameraGrayArray[i + j * X_SIZE] = 0;
    }
  }
}


void printArray() {
  for (int j = 0; j < Y_SIZE; j++) {
    for (int i = 0; i < X_SIZE; i++) {
      Serial.printf("%d ", cameraGrayArray[i + j * X_SIZE]);
    }
    Serial.println();
  }
  Serial.println("\n");
}

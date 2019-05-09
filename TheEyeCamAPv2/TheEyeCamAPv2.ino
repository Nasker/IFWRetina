#include "IFCamera.h"
#include "OnBoardLeds.h"
#include <WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>
#include <OSCBundle.h>
#include <OSCBoards.h>

const char* ssid = "TheEyeAP";
const char* password = "iswatchingyou";
IPAddress localIP = IPAddress(10, 10, 10, 1);
const IPAddress hostIPPC(10, 10, 10, 2);
const IPAddress hostIPMicro(10, 10, 10, 3);
IPAddress gateway = IPAddress(10, 10, 10, 1);
IPAddress  subnet = IPAddress(255, 255, 255, 0);
const int inPort = 5678;
const int outPortMicro = 1234;
const int outPortPC = 4321;

IFCamera camera;
OnBoardLeds bLeds;
WiFiUDP Udp;

uint8_t cameraGrayArray[X_SIZE * Y_SIZE];

void updateNSend(uint8_t (& grayArray) [X_SIZE * Y_SIZE]) {
  for (int j = 0; j < Y_SIZE; j++) {
    String address = "/camera/grayArray/line" + String(j);
    const char * addr = address.c_str();
    OSCMessage msg(addr);
    for (int i = 0; i < X_SIZE; i++) {
      msg.add(grayArray[i + j * X_SIZE]);
    }
    Udp.beginPacket(hostIPPC, outPortPC);
    msg.send(Udp);
    Udp.endPacket();
    Udp.beginPacket(hostIPMicro, outPortMicro);
    msg.send(Udp);
    Udp.endPacket();
    msg.empty();
  }
}

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  camera.initSetup();
  camera.setThreshold(140);
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig (localIP, gateway, subnet);
  Serial.print("WiFI AP Ready, Its IP:");
  Serial.println(WiFi.softAPIP());
  Udp.begin(inPort);
}

void loop() {
  OSCMsgReceive();
  camera.readNProcess(cameraGrayArray);
  //camera.printGrayMatrix(cameraGrayArray);
  updateNSend(cameraGrayArray);
  bLeds.blinky();
}

void OSCMsgReceive() {
  OSCBundle bundleIN;       //paquet generic de missatges OSC, pot contenir whatever
  int size = Udp.parsePacket(); //mirem si hi ha dades als paquets, si rebem algo
  if (size > 0) {   //si rebem algo...
    Serial.printf("Message Received with size: %d\n", size);
    while (size--) {
      bundleIN.fill(Udp.read());
    }
    bundleIN.route("/camera/flashlight", actOnReceiveFlashLightSwitch);
  }
}

void actOnReceiveFlashLightSwitch(OSCMessage &msg, int addrOffset) {
  Serial.println("Received Flashlight switch message");
  bLeds.switchFlash();
}

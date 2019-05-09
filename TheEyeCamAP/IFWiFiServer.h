#include <WiFi.h>
#include <ArduinoOSC.h>
#include "IFCamera.h"

const char* ssid = "TheEyeAP";
const char* password = "iswatchingyou";
IPAddress local_IP = IPAddress(10, 10, 10, 1);
const char* host_IP_PC = "10.10.10.2";
const char* host_IP_micro = "10.10.10.3";
IPAddress gateway = IPAddress(10, 10, 10, 1);
IPAddress  subnet = IPAddress(255, 255, 255, 0);
const int inPort = 4321;
const int outPortMicro = 1234;
const int outPortPC = 4321;


class IFWiFiServer {
    WiFiUDP udp;
    ArduinoOSCWiFi osc;
  public:
    IFWiFiServer() {};
    void initSetup() {
      WiFi.softAP(ssid, password);
      WiFi.softAPConfig (local_IP, gateway, subnet);
      Serial.print("WiFI AP Ready, Its IP:");
      Serial.println(WiFi.softAPIP());
      osc.begin(udp, inPort);
      //osc.addCallback("/camera/setThreshold", &callback);
      //osc.addCallback("/camera/setFrame", &callback);
    }


    void updateNSend(uint8_t (& grayArray) [X_SIZE * Y_SIZE]) {
      OSCMessage msg_micro;
      OSCMessage msg_PC;
      for (int j = 0; j < Y_SIZE; j++) {
        msg_micro.beginMessage(host_IP_micro, outPortMicro);
        msg_PC.beginMessage(host_IP_PC, outPortPC);
        String address ="/camera/grayArray/line" + String(j); 
        const char * addr = address.c_str();
        msg_micro.setOSCAddress(addr);
        msg_PC.setOSCAddress(addr);
        for (int i = 0; i < X_SIZE; i++) {
          //Serial.printf("%d ",grayArray[i + j * X_SIZE]);
          msg_micro.addArgInt32(grayArray[i + j * X_SIZE]);
          msg_PC.addArgInt32(grayArray[i + j * X_SIZE]);
        }
        //Serial.println();
        osc.send(msg_micro);
        osc.send(msg_PC);
      }
    }

  private:
    void callback(OSCMessage& m) {
      Serial.printf("Received message with this address: %s \n",m.getOSCAddress());
    }

};

#include <WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>
#include <OSCBundle.h>
#include <OSCBoards.h>        //includes necessaris per el modul d'ethernet i OSC

WiFiUDP Udp;                //objecte per a connexió udp

const char* ssid = "TheEyeAP";
const char* password = "iswatchingyou";
IPAddress local_IP = IPAddress(10, 10, 10, 1);
const char* host_IP = "10.10.10.255";
IPAddress gateway = IPAddress(10, 10, 10, 1);
IPAddress  subnet = IPAddress(255, 255, 255, 0);
const int inPort = 4321;
const int outPort = 1234;

void setup() {
  Serial.begin(115200);     //velocitat de comunicació amb el port serie
  Serial.println("START SETUP");
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig (local_IP, gateway, subnet);
  Serial.print("WiFI AP Ready, Its IP:");
  Serial.println(WiFi.softAPIP());
  Udp.begin(inPort);        //arranquem el port on escoltarem en Udp
  Serial.println("FINISH SETUP");
}

void loop() {
  OSCMsgReceive();    //esperem a rebre missatges OSC
}


void OSCMsgReceive() {
  OSCBundle bundleIN;       //paquet generic de missatges OSC, pot contenir whatever
  int size = Udp.parsePacket(); //mirem si hi ha dades als paquets, si rebem algo
  if (size > 0) {   //si rebem algo...
    Serial.print("Message Received with size: ");
    Serial.println(size);
    while (size--) {
      bundleIN.fill(Udp.read()); //plenem el bundle amb el que llegim al port Udp
      //if (bundleIN.hasError())
      //Serial.println("ERROR ON PACKET");
    }
    bundleIN.route("/receive", actOnReceive);  //"Rutejem el rebut a l'etiqueta i la funció que cridem mes abaix.

  }
}

void actOnReceive(OSCMessage &msg, int addrOffset) {
  int receivedValue = msg.getInt(0);    //desempaquetem els Integers que ens venen
  Serial.print("Received ");
  Serial.println(receivedValue);

}

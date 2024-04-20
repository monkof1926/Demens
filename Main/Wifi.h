#include <WiFi.h>
#include <WiFiMulti.h>
#include "Credentials.h"


class Wifi{
  public:
  WiFiMulti wifiMulti;
  WiFiClient client;
  char wifiStatus[512];
  boolean wifiToggle = 0;
  int wifi_disconnection_seconds = 0;

    Wifi(){ 
      wifiMulti.addAP(WIFIHOMESSID, WIFIHOMEPASS); // connects with the wifi ssid and password
    }
  
  void displayWifiUpdate() {
  wifiStatus[0] = '\0';
  sprintf(wifiStatus, "%s %s %s %ddBm %i %s",
          WIFIHOMESSID,
          WiFi.macAddress().c_str(),
          WiFi.localIP().toString().c_str(),
          WiFi.RSSI(),
          WiFi.status()
          );

  Serial.println(wifiStatus);
  }

  void wifiCheck(){
     if (WiFi.status() == WL_CONNECTED) {
      wifiToggle = !wifiToggle;
      wifi_disconnection_seconds = 0;
    } else {
      wifiToggle = 0;
      wifi_disconnection_seconds++;
    }
  }

  void wifiCheck2(){
    switch(wifiMulti.run() != WL_CONNECTED){
      case 1:
      wifiMulti.addAP(WIFIHOMESSID, WIFIHOMEPASS);
      Serial.println("Is home");
      break;
      case 2:
      wifiMulti.addAP(WIFINEIGHBORSSID, WIFINEIGHBORRPASS);
      Serial.println(" Is by home");
      break;
      case 3:
      wifiMulti.addAP(GSMSSID, GSMPASS);
      Serial.println("Is not home");
      break;
    }
  }
};
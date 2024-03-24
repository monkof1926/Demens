#include <TinyGPS++.h>
#include <axp20x.h>
#include <LoRa.h>

class Gpss{

   public: 
    int GPSRxPin; // Pin for the gps
    int GPSTxPin; // Pin for the gps
    int i = 0;

    //tineGps object 
    TinyGPSPlus gps;


    double gpsLat = (gps.location.lat(), 6);
    double gpsLon = (gps.location.lng(), 6);  
    double gpsAlt = (gps.altitude.meters());

    String gpsData;
    byte gpsSoft;

  Gpss(int GPSRxPin, int GPSTxPin){ 
    this -> GPSRxPin = GPSRxPin;
    pinMode(this->GPSRxPin, INPUT);
    this -> GPSTxPin = GPSTxPin;
    pinMode(this->GPSTxPin, INPUT);
  }
  
  void location(){

    if(gpsLat == 0 || gpsLon == 0){
      Serial.print(".");
      i++;
      if(i%30 == 0){
        Serial.print("");
      }
      if(i == 300){
        Serial.println("Gps not working try going out side");
      }
    }else{
      gpsData = "[";
      gpsData += String(gpsLat, 6);
      gpsData += ",";
      gpsData += String(gpsLon, 6);
      gpsData +="]";

      Serial.print(gpsData);
      LoRa.beginPacket();
      LoRa.print(gpsData);
      LoRa.endPacket();

    } 
  }
};




#include <TinyGPS++.h>
#include <axp20x.h>
#include <LoRa.h>

//tineGps object 
TinyGPSPlus gps;

double gpsLat = (gps.location.lat(), 6);
double gpsLon = (gps.location.lng(), 6);  
double gpsAlt = (gps.altitude.meters());

bool inSafeZone = 0;
bool inHomeZone = 0;

class Gpss{

   public: 
    int GPSRxPin; // Pin for the gps
    int GPSTxPin; // Pin for the gps
    int i = 0;

    double safeZoneYMax = 12.534343;
    double safeZoneYMin = 12.528831;
    double safeZoneXMax = 55.719015;
    double safeZoneXMin = 55.719577;

    double HomeZoneYMax = 12.530667;
    double HomeZoneYMin = 12.530570;
    double HomeZoneXMax = 55.719217;
    double HomeZoneXMin = 55.719082;

    bool isSafeY = 0;
    bool isSafeX = 0; 

    

    String gpsData;
    byte gpsSoft;

  Gpss(int GPSRxPin, int GPSTxPin){ 
    this -> GPSRxPin = GPSRxPin;
    pinMode(this->GPSRxPin, INPUT);
    this -> GPSTxPin = GPSTxPin;
    pinMode(this->GPSTxPin, INPUT);
  }

  void gpsStart(){
    Serial.println("The gps is starting");
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
    }

      gpsData = "[";
      gpsData += String(gpsLat, 6);
      gpsData += ",";
      gpsData += String(gpsLon, 6);
      gpsData +="]";

      Serial.println(gpsData);
      LoRa.beginPacket();
      LoRa.print(gpsData);
      LoRa.endPacket();

  }

  void safeZone(){
    if(gpsLat != 6 && gpsLon != 6){
      double checkSafeZoneMax =
      (unsigned long)TinyGPSPlus::distanceBetween(
        gpsLat,
        safeZoneYMax,
        gpsLon,        
        safeZoneYMin
      );

      if(gpsLat > safeZoneYMin && gpsLat < safeZoneYMax){
          isSafeY = 1;
          LoRa.beginPacket();
          LoRa.print("Is in safe zone");
          LoRa.endPacket();
      }else if(gpsLat < safeZoneYMin || gpsLat > safeZoneYMin && gpsLat > safeZoneYMax){
          isSafeY = 0;
          LoRa.beginPacket();
          LoRa.print(gpsLat);
          LoRa.endPacket();
      }else{
        isSafeY = 0;
      }
      double checkSafeZoneMin = 
       (unsigned long)TinyGPSPlus::distanceBetween(
        gpsLat,
        safeZoneXMax,
        gpsLon,        
        safeZoneXMin
      );

      if(gpsLon > safeZoneXMin && gpsLon < safeZoneXMax){
          isSafeX = 1;
          LoRa.beginPacket();
          LoRa.print("Is in safe zone");
          LoRa.endPacket();
      }else if(gpsLon < safeZoneXMin || gpsLon > safeZoneXMin && gpsLon > safeZoneXMax){
          isSafeY = 0;
      }else{
        isSafeX = 0;
        LoRa.beginPacket();
        LoRa.print(gpsLon);
        LoRa.endPacket();
      }
    }else{
      Serial.println("Gps is Starting");
    }
  }

  void HomeSafeZon(){
    if(gpsLat != 6 && gpsLon != 6){
        double checkHomeZoneMax =
        (unsigned long)TinyGPSPlus::distanceBetween(
        gpsLat,
        HomeZoneYMax,
        gpsLon,        
        HomeZoneXMax
      );

      if(gpsLat > HomeZoneYMin && gpsLat < HomeZoneYMax){
          isSafeY = 1;
          inHomeZone = 1;
          LoRa.beginPacket();
          LoRa.print("Is Home");
          LoRa.endPacket();
      }else if (gpsLat < HomeZoneYMin || gpsLat > HomeZoneYMin && gpsLat > HomeZoneYMax){
        if(gpsLat > safeZoneYMin && gpsLat < safeZoneYMax){
          inSafeZone = 1;
          LoRa.beginPacket();
          LoRa.print("Is in safe zone");
          LoRa.endPacket();
          Serial.println("Is in safe zone");
        }else{
          isSafeY = 0;
          LoRa.beginPacket();
          LoRa.print(gpsLat);
          LoRa.endPacket();
        }
      }else if(gpsLat > safeZoneYMin && gpsLat < safeZoneYMax ){
          inSafeZone = 1;
          LoRa.beginPacket();
          LoRa.print("Is Home");
          LoRa.endPacket();
          Serial.println("Is in safe zone");
      }else{
          isSafeY = 0;
          LoRa.beginPacket();
          LoRa.print(gpsLat);
          LoRa.endPacket();
      }

      double checkHomeZoneMin = 
      (unsigned long)TinyGPSPlus::distanceBetween(
        gpsLat,
        HomeZoneYMin,
        gpsLon,        
        HomeZoneXMin
      );

      if(gpsLon > HomeZoneXMin && gpsLon < HomeZoneXMax){
          isSafeY = 1;
          inHomeZone = 1;
          LoRa.beginPacket();
          LoRa.print("Is Home");
          LoRa.endPacket();
      }else if(gpsLon < HomeZoneXMin || gpsLon > HomeZoneXMin && gpsLon >  HomeZoneXMax){
        if(gpsLon > safeZoneXMin && gpsLon < safeZoneXMax){
            inSafeZone = 1;
            LoRa.beginPacket();
            LoRa.print("Is Home");
            LoRa.endPacket();
            Serial.println("Is in safe zone");
        }else{
            isSafeX = 0;
            LoRa.beginPacket();
            LoRa.print(gpsLon);
            LoRa.endPacket();
        }
      }else if(gpsLon > safeZoneXMin && gpsLon < safeZoneXMax){
          inSafeZone = 1;
          LoRa.beginPacket();
          LoRa.print("Is Home");
          LoRa.endPacket();
          Serial.println("Is in safe zone");
      }else{
        isSafeX = 0;
        LoRa.beginPacket();
        LoRa.print(gpsLon);
        LoRa.endPacket();
        Serial.println("Gps is Starting");
      }

    }
  }
};




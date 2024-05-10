#include <TinyGPS++.h>
#include <axp20x.h>
#include <LoRa.h>
#include <TimeLib.h>

//tineGps object 
TinyGPSPlus gps;


double gpsLat = 6;
double gpsLon = 6;
double gpsAlt = (gps.altitude.meters());

bool inSafeZone = 0;
bool inHomeZone = 0;
double HomeZoneLat;
double HomeZoneLon;

byte Second;
byte Minute;
byte Hour;
byte lastSecond;

byte Day;
byte Month;
int Year;

char Time[] = "Time: 00:00:00";
char Date[] = "Date: 00/00-2000";

int time_offset = 7200;

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

    double HomeZoneLat;
    double HomeZoneLon;
    double HomeZoneRadius = 50.0; // meters
    double SafeZoneRadius = 250; // meters

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
    gpsLat = gps.location.lat();
    gpsLon = gps.location.lng();

      if(gpsLat == 6 || gpsLon == 6){
      Serial.print(".");
      i++;
      if(i%30 == 0){
        Serial.print("Gps Starting");
      }
      if(i == 300){
        Serial.println("Gps not working try going out side");
        HomeZoneLat = 55.718830;
        HomeZoneLon = 12.530630;
        Serial.print(HomeZoneLat);
        Serial.println(HomeZoneLon);
      }
    }else{
      
      HomeZoneLat = gpsLat;
      HomeZoneLon = gpsLon;
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
  }

  void HomeZoneCheck(){
    while(gpsLat == 6 && gpsLon == 6){
      // if(gpsLat == 6 && gpsLon == 6){
      //   HomeZoneLat = 55.718830;
      //   HomeZoneLon = 12.530630;
      // }
      if(distance(gpsLat, gpsLon, HomeZoneLat, HomeZoneLon) > HomeZoneRadius ){
        inHomeZone = 1;
        Serial.print(HomeZoneLat);
        Serial.println("Is home");
        LoRa.beginPacket();
        LoRa.print(inHomeZone);
        LoRa.endPacket();
      }else{
        inHomeZone = 0;
      
       }
    }
  }

  void SafeZoneCheck(){
    while(gpsLat == 6 && gpsLon == 6 && inHomeZone == 0){
      // if(gpsLat == 6 && gpsLon == 6){
        //   HomeZoneLat = 55.718830;
        //   HomeZoneLon = 12.530630;
      // }
      if(distance(gpsLat, gpsLon, HomeZoneLat, HomeZoneLon) > SafeZoneRadius ){
        inSafeZone = 1;
        Serial.println("is in safe zone");
        LoRa.beginPacket();
        LoRa.print(inSafeZone);
        LoRa.endPacket();
      }
    }
  }

  double distance(double lat1, double lon1, double lat2, double lon2 ){
    double earthRadius = 6371000;
    double radius1 = radians(lat1);
    double radius2 = radians(lat2);
    double deltaPhi = radians(lat2 - lat1);
    double deltaLamda = radians(lon2 - lon1);

    double a = sin(deltaPhi/2) * sin(deltaPhi/2) + cos(radius1) * cos(radius2) * sin(deltaLamda/2) * sin(deltaLamda/2);

    double c = 2 * atan2(sqrt(a), sqrt(1-a)); // atan2 returns the angle between a positive x and a point

    double d = earthRadius * c;

    return d;
  }

  void timer(){
    // char time [] = Hour + Minute + Second;
   
    if(gps.time.isValid()){
      Second = gps.time.second();
      Minute = gps.time.minute();
      Hour = gps.time.hour();

      Serial.print("Time: ");
      Serial.print(Hour);
      Serial.print(":");
      Serial.print(Minute);
      Serial.print(":");
      Serial.println(Second);
    }
    if(gps.date.isValid()){
      Day = gps.date.day();
      Month = gps.date.month();
      Year = gps.date.year();

      Serial.print("Date: ");
      Serial.print(Day);
      Serial.print("/");
      Serial.print(Month);
      Serial.print("/");
      Serial.println(Year);
    }
    if(lastSecond != gps.time.second()){
      lastSecond = Second;
      setTime(Hour, Minute, Second, Day, Month, Year);
      adjustTime(time_offset);

      Time[12] = second() / 10 + '0';
      Time[13] = second() % 10 + '0';
      Time[9] = minute() / 10 + '0';
      Time[10] = minute() % 10 + '0';
      Time[6] = hour() / 10 + '0';
      Time[7] = hour() % 10 + '0';

      Date[14] = (year() / 10) % 10 +'0';
      Date[15] = year() % 10 + '0';
      Date[9] = month() / 10 +  '0';
      Date[10] = month() % 10 + '0';
      Date[6] = day() / 10 + '0';
      Date[7] = day() % 10 + '0';
    }

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

  void HomeSafeZone(){
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
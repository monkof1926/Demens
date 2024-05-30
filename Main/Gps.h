#include <TinyGPS++.h> // libraies used
#include <axp20x.h>
#include <LoRa.h>
#include <TimeLib.h>

//tineGps object 
TinyGPSPlus gps;

// set the gps latitude and longitude to 6 as set and get the altitude and makes them global values
double gpsLat = 6;
double gpsLon = 6;
double gpsAlt = (gps.altitude.meters());
// makes global values for if in home or safe zone and makes the homezone lat and lon  
bool inSafeZone;
bool inHomeZone;
double HomeZoneLat;
double HomeZoneLon;

// set up time
byte Second;
byte Minute;
byte Hour;
byte lastSecond;
// set date 
byte Day;
byte Month;
int Year;
// the arrays for time and date
char Time[] = "Time:00:00:00";
char Date[] = "Date:00/00-2000";
// the off set to get it to utc-2
int time_offset = 7200;

class Gpss{
  
   public: 
    int GPSRxPin; // Pin for the gps
    int GPSTxPin; // Pin for the gps
    int i = 0; // counter for gps start

    // if the home and safe zone are not able to be a cirkle or if it have points that are dangerous other patient  
    double ZoneYMax = 12.534343;
    double ZoneYMin = 12.528831;
    double ZoneXMax = 55.719015;
    double ZoneXMin = 55.719577;

    double HomeZoneYMax = 12.530667;
    double HomeZoneYMin = 12.530570;
    double HomeZoneXMax = 55.719217;
    double HomeZoneXMin = 55.719082;

    bool isSafeY = 0; // for not cirkler safe zone multiple safe points can be needed
    bool isSafeX = 0; 

    // the radius of the home and safe zone in meters
    double HomeZoneRadius = 50.0; // meters
    double SafeZoneRadius = 250; // meters

    // set up the string and byte with gps data
    String gpsData;
    byte gpsSoft;
  //initialized the gps on the gps pins 
  Gpss(int GPSRxPin, int GPSTxPin){ 
    this -> GPSRxPin = GPSRxPin;
    pinMode(this->GPSRxPin, INPUT);
    this -> GPSTxPin = GPSTxPin;
    pinMode(this->GPSTxPin, INPUT);
  }
  // prints that the gps is started
  void gpsStart(){
    Serial.println("The gps is starting");
  }
  // starts getting the location and if there is no answer from the gps in 5 min prints Gps not working try going out side and then set the home zone lat and lon to predeterment location at the home
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
      // if the gps works print the gps location and set the home zone lat and lon to the firste place the gps is turn on
      HomeZoneLat = gpsLat;
      HomeZoneLon = gpsLon;
      gpsData = "[";
      gpsData += String(gpsLat, 6);
      gpsData += ",";
      gpsData += String(gpsLon, 6);
      gpsData +="]";
      // sends gps data over LoRa
      Serial.println(gpsData);
      LoRa.beginPacket();
      LoRa.print(gpsData);
      LoRa.endPacket();
    }
  }

// check if the patient is in home zone is inspire by from https://fourbitselectronics.wordpress.com/2023/05/12/guide-to-gps-geofencing-with-arduino-and-neo-6m-gps-module/
  void HomeZoneCheck(){
    while(gpsLat != 6 && gpsLon != 6){
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
// check if the patient is in safe zone is inspire by from https://fourbitselectronics.wordpress.com/2023/05/12/guide-to-gps-geofencing-with-arduino-and-neo-6m-gps-module/
  void SafeZoneCheck(){
    while(gpsLat != 6 && gpsLon != 6 && inHomeZone == 0){
      if(distance(gpsLat, gpsLon, HomeZoneLat, HomeZoneLon) > SafeZoneRadius ){
        inSafeZone = 1;
        Serial.println("is in safe zone");
        LoRa.beginPacket();
        LoRa.print(inSafeZone);
        LoRa.endPacket();
      }
    }
  }
// way to calculate distance got the ide from https://fourbitselectronics.wordpress.com/2023/05/12/guide-to-gps-geofencing-with-arduino-and-neo-6m-gps-module/
  double distance(double lat1, double lon1, double lat2, double lon2 ){
    double earthRadius = 6371000;
    double radius1 = radians(lat1);
    double radius2 = radians(lat2);
    double deltaPhi = radians(lat2 - lat1);
    double deltaLamda = radians(lon2 - lon1);

    double axe = sin(deltaPhi/2) * sin(deltaPhi/2) + cos(radius1) * cos(radius2) * sin(deltaLamda/2) * sin(deltaLamda/2);

    double c = 2 * atan2(sqrt(axe), sqrt(1-axe)); // atan2 returns the angle between a positive x and a point

    double d = earthRadius * c;

    return d;
  }
  // set timer by getting it from the gps
  void timer(){   
    // check if the gps is valided if yes then get second, mintute and hour then prints it
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
    // check if the gps is valided if yes then get day, month and year then prints it
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
    //check if a second have happened if yes then set the last the new second to + 1 from the last second and set lastSecond to second
    if(lastSecond != gps.time.second()){
      lastSecond = Second;
      setTime(Hour, Minute, Second, Day, Month, Year);
      adjustTime(time_offset);
      // is need to print the time
      Time[12] = second() / 10 + '0';
      Time[13] = second() % 10 + '0';
      Time[9] = minute() / 10 + '0';
      Time[10] = minute() % 10 + '0';
      Time[6] = hour() / 10 + '0';
      Time[7] = hour() % 10 + '0';
      // is need to print the date
      Date[14] = (year() / 10) % 10 +'0';
      Date[15] = year() % 10 + '0';
      Date[9] = month() / 10 +  '0';
      Date[10] = month() % 10 + '0';
      Date[6] = day() / 10 + '0';
      Date[7] = day() % 10 + '0';
    }
  }
// if the safe zone is not cirkular or if there is dangerous points then uses this to check if the person is in safe zone
  void Zone(){
    if(gpsLat != 6 && gpsLon != 6){
      double checkSafeZoneMax =
      (unsigned long)TinyGPSPlus::distanceBetween(
        gpsLat,
        ZoneYMax,
        gpsLon,        
        ZoneYMin
      );
      // check if the patient is within the y min and max can be replicate if more point is needed
      if(gpsLat > ZoneYMin && gpsLat < ZoneYMax){
          isSafeY = 1;
          LoRa.beginPacket();
          LoRa.print("Is in safe zone");
          LoRa.endPacket();
      }else if(gpsLat < ZoneYMin || gpsLat > ZoneYMin && gpsLat > ZoneYMax){
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
        ZoneXMax,
        gpsLon,        
        ZoneXMin
      );
      /// check if the patient is within the x min and max can be replicate if more point is needed
      if(gpsLon > ZoneXMin && gpsLon < ZoneXMax){
          isSafeX = 1;
          LoRa.beginPacket();
          LoRa.print("Is in safe zone");
          LoRa.endPacket();
      }else if(gpsLon < ZoneXMin || gpsLon > ZoneXMin && gpsLon > ZoneXMax){
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
// if the home zone is not cirkular or if there is dangerous points then uses this to check if the person is in home zone
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
        if(gpsLat > ZoneYMin && gpsLat < ZoneYMax){
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
      }else if(gpsLat > ZoneYMin && gpsLat < ZoneYMax ){
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
      /// check if the patient is within the x min and max can be replicate if more point is needed
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
        if(gpsLon > ZoneXMin && gpsLon < ZoneXMax){
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
      }else if(gpsLon > ZoneXMin && gpsLon < ZoneXMax){
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
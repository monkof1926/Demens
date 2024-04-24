#include <LoRa.h>
#include<PulseSensorPlayground.h>
#include<HTTPClient.h>
#include "Gps.h"
#include "Puls.h"
#include "LoRa.h"
#include "Wifi.h"

#define UPDATEINTERVAL 6000
#define Name "Lukas"
#define Unit "unit_1"

#define SCK     5    // GPIO5  -- SX1278's SCK
#define MISO    19   // GPIO19 -- SX1278's MISnO
#define MOSI    27   // GPIO27 -- SX1278's MOSI
#define SS      18   // GPIO18 -- SX1278's CS
#define RST     14   // GPIO14 -- SX1278's RESET
#define DI0     26   // GPIO26 -- SX1278's IRQ(Interrupt Request)

AXP20X_Class axp;
HardwareSerial GPS(1);
HTTPClient http;

int GPSRxPin = 12; // Pin for the gps
int GPSTxPin = 34; // Pin for the gps

const int pulsePin = 35; // Pin for PulseSensor might need to change to 0
const int pulseBlink = LED_BUILTIN; // Led for puls sensor Might need to change to 35
int pulseFade = 5; // How long a blink will last for puls sensor
int pulseFadeRate = 0; //

unsigned long updateTimer = 0;
unsigned long updateStart = 1000;
unsigned long start = millis();
int pulseThreshold = 550;


double gpsTestLat = (gps.location.lat(), 6);
double gpsTestLon = (gps.location.lng(), 6);  
double gpsTestAlt = (gps.altitude.meters());

String finalurl; 
String url = "https://script.google.com/macros/s/AKfycbwHh7nkWtx6QW5dDdYd0U9nFj4ts0iWXiokkUSIZvDoE0AqEG834wgrDwhWoish8kcG/exec?";
String api_key = "AKfycbwHh7nkWtx6QW5dDdYd0U9nFj4ts0iWXiokkUSIZvDoE0AqEG834wgrDwhWoish8kcG";
String payload;

int temp = 37;

Gpss gps1(GPSRxPin, GPSTxPin);
Wifi wifi;
Puls puls1(pulsePin, pulseBlink, pulseFade, pulseFadeRate);
Lora lora;
String needHelp = "No";
//PulseSensorPlayground pulseSensor;


void setup() {
  Serial.begin(115200);
  GPS.begin(9600, SERIAL_8N1, GPSTxPin, GPSRxPin); 
  gps1.gpsStart();
  wifi.wifiCheck();
  wifi.wifiCheck2();
  SPI.begin(SCK,MISO,MOSI,SS);
  LoRa.setPins(SS,RST,DI0);
  Wire.begin(21, 22);
   if (!axp.begin(Wire, AXP192_SLAVE_ADDRESS)) {
    Serial.println("AXP192 Begin PASS");
  } else {
    Serial.println("AXP192 Begin FAIL");
  }
  axp.setPowerOutPut(AXP192_LDO2, AXP202_ON);
  axp.setPowerOutPut(AXP192_LDO3, AXP202_ON);
  axp.setPowerOutPut(AXP192_DCDC2, AXP202_ON);
  axp.setPowerOutPut(AXP192_EXTEN, AXP202_ON);
  axp.setPowerOutPut(AXP192_DCDC1, AXP202_ON);
  pulseSensor.analogInput(pulsePin);
  pulseSensor.blinkOnPulse(pulseBlink);
  pulseSensor.setThreshold(pulseThreshold);

  if (pulseSensor.begin()) {
    Serial.println("We created a pulseSensor Object !");  //This prints one time at Arduino power-up,  or on Arduino reset.  
  }
  Serial.print("LoRa Sender");
  while(!LoRa.begin(866E6)){
    Serial.println("L");
  LoRa.setSyncWord(0xFF);
  while (1);
  Serial.println("LoRa Works"); 
  }
}

void health(){

}

void loop() { 
  if(millis() - updateTimer > UPDATEINTERVAL || updateTimer == 0 ){
    gps1.location();
    gps1.safeZone();
    gps1.HomeSafeZon();
    puls1.bpm3();
    Serial.println(puls1.bpm3());
    puls1.getNormalPulse();
    puls1.getRestingPulse();
    puls1.normalPulseCheck();
    wifi.wifiCheck2(); 
    lora.LoRaCall();

    statusupdate();
    
    updateTimer = millis();
  }      
}

String statusupdate(){
  Serial.print("[HTTP] begin...\n");

  String finalurl = url + "name=" + Name + "&unit=" + Unit + "&health=" + healthStatus + "&puls=" + BPM  + "&temp=" + temp + "&latitude=" + gpsLat + "&longitude=" + gpsLon + "&ishome=" + inHomeZone + "&insafe=" + inSafeZone + "&needHelp=" + needHelp + "&alarmstatus=" + alarmStatus;
  Serial.println(finalurl);
  http.begin(finalurl);

  int httpCode = http.GET();

  if (httpCode > 0) {
    Serial.printf("[HTTP] GET... code: %d\n", httpCode);

    // file found at server
    if (httpCode == HTTP_CODE_OK) {
      payload = http.getString();
      Serial.println(payload);
    }
  } else {
    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }
  // No longer looks for response
  http.end(); //Free's the resources

  return payload;
}
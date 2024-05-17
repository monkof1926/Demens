#include <LoRa.h>
#include<PulseSensorPlayground.h>
#include<HTTPClient.h>
#include <OneWire.h>
#include "Gps.h"
#include "Puls.h"
#include "LoRa.h"
#include "Wifi.h"
#include "Temperature.h"
#include "Health.h"
#include "Screen.h"

#define UPDATEINTERVAL 6000
#define Name "Lukas"
#define Unit "unit_1"

#define SCK     5    // GPIO5  -- SX1278's SCK
#define MISO    19   // GPIO19 -- SX1278's MISnO
#define MOSI    27   // GPIO27 -- SX1278's MOSI
#define SS      18   // GPIO18 -- SX1278's CS
#define RST     14   // GPIO14 -- SX1278's RESET
#define DI0     26   // GPIO26 -- SX1278's IRQ(Interrupt Request)


int GPSRxPin = 12; // Pin for the gps
int GPSTxPin = 34; // Pin for the gps

const int pulsePin = 35; // Pin for PulseSensor might need to change to 0
const int pulseBlink = LED_BUILTIN; // Led for puls sensor Might need to change to 35
int pulseFade = 5; // How long a blink will last for puls sensor
int pulseFadeRate = 0; // how long it takes for the fading takes

int screenSDA = 21;
int screenSCL = 22;

unsigned long updateTimer = 0;
unsigned long updateStart = 1000;
unsigned long start = millis();
int pulseThreshold = 550;

String finalurl; 
String url = "https://script.google.com/macros/s/AKfycbz15gCPmmGvLAgM-Vh5nfInJRi7SafzYq-hJadaY74RRNUR6bd9mHUvzwJEXuqBRDPwxw/exec?";
String api_key = "AKfycbz15gCPmmGvLAgM-Vh5nfInJRi7SafzYq-hJadaY74RRNUR6bd9mHUvzwJEXuqBRDPwxw";
String payload;


Gpss gps1(GPSRxPin, GPSTxPin);
Wifi wifi;
Puls puls1(pulsePin, pulseBlink, pulseFade, pulseFadeRate);
Lora lora;
Health health;
Screen screen;

AXP20X_Class axp;
HardwareSerial GPS(1);
HTTPClient http;
Temperature temperature(temperaturePin);



void setup() {
  Serial.begin(115200);
  GPS.begin(9600, SERIAL_8N1, GPSTxPin, GPSRxPin); 
  gps1.gpsStart();
  wifi.wifiStart();
  wifi.wifiCheck();
  oneWire.begin(9600);
  sensors.begin();
  wifi.wifiCheck2();
  SPI.begin(SCK,MISO,MOSI,SS);
  LoRa.setPins(SS,RST,DI0);
  Wire.begin(21, 22);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.display();
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

void loop() { 
  if(inHomeZone == 1 && inSafeZone == 1 || inHomeZone == 1 && inSafeZone == 0 || inHomeZone == 0 && inSafeZone == 1  ){
  screen.displayClock();
  }else if(inHomeZone == 0 && inSafeZone == 0 ){
    screen.displayArrow();
  }
  if(millis() - updateTimer > UPDATEINTERVAL || updateTimer == 0 ){
    gps1.location();
    //gps1.safeZone();
    gps1.HomeZoneCheck();
    gps1.SafeZoneCheck();
    //gps1.HomeSafeZone();
    gps1.timer();
    puls1.bpm3();
    puls1.getNormalPulse();
    puls1.pulsStatus();
    puls1.getRestingPulse();
    puls1.normalPulseCheck();
    temperature.tempCheck();
    temperature.normalTempCheck();
    temperature.TempStatus();
    health.calculateHealthScore();
    health.extremLowHealthBuilt();
    health.extremHighHealthBuilt();
    health.HighHealthBuilt();
    health.LowHealthBuilt();
    health.GoodHealthBuilt();
    //health.healthbuilt();
    health.ExtremHealthStatus();
    health.coldTemperature();
    wifi.wifiCheck2(); 
    lora.LoRaCall();

  do
    {
      while (GPS.available())
        gps.encode(GPS.read());
    } while (millis() - start < 1000);
    urlUpdate();
    statusupdate();
    
    updateTimer = millis();
  }     
  if(ExtremHealth == 1){
    statusupdate();
  }
}

String urlUpdate(){

     
  if(inHomeZone == 0 && inSafeZone == 0 || ExtremHealth == 1){

    finalurl = url + "name=" + Name + "&unit=" + Unit + "&health=" + healthStatus + "&puls=" + BPM  + "&temp=" + temp + "&latitude=" + gpsLat + "&longitude=" + gpsLon + "&ishome=" + inHomeZone + "&insafe=" + inSafeZone + "&needHelp=" + needHelp + "&alarmstatus=" + alarmStatus + "&time=" + Time + "&date=" + Date;
    return finalurl;

  }else if(inHomeZone == 0 && inSafeZone == 1){

      finalurl = url + "name=" + Name + "&unit=" + Unit + "&health=" + healthStatus + "&ishome=" + inHomeZone + "&insafe=" + inSafeZone + "&needHelp=" + needHelp + "&alarmstatus=" + alarmStatus;
      return finalurl;
  }else if(inHomeZone == 1){

      finalurl = url + "name=" + Name + "&unit=" + Unit + "&health=" + healthStatus + "&ishome=" + inHomeZone + "&needHelp=" + needHelp + "&alarmstatus=" + alarmStatus;
      return finalurl;
  }else {

    finalurl = url + "name=" + Name + "&unit=" + Unit + "&health=" + healthStatus + "&puls=" + BPM  + "&temp=" + temp + "&latitude=" + gpsLat + "&longitude=" + gpsLon + "&ishome=" + inHomeZone + "&insafe=" + inSafeZone + "&needHelp=" + needHelp + "&alarmstatus=" + alarmStatus;
    return finalurl;
  }
}

String statusupdate(){
  Serial.print("[HTTP] begin...\n");

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
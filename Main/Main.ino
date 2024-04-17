#include <LoRa.h>
#include<PulseSensorPlayground.h>
#include "Gps.h"
#include "Puls.h"
#include "LoRa.h"
#include "Wifi.h"

#define UPDATEINTERVAL 6000

#define SCK     5    // GPIO5  -- SX1278's SCK
#define MISO    19   // GPIO19 -- SX1278's MISnO
#define MOSI    27   // GPIO27 -- SX1278's MOSI
#define SS      18   // GPIO18 -- SX1278's CS
#define RST     14   // GPIO14 -- SX1278's RESET
#define DI0     26   // GPIO26 -- SX1278's IRQ(Interrupt Request)

AXP20X_Class axp;
HardwareSerial GPS(1);
TinyGPSPlus gps;

int GPSRxPin = 12; // Pin for the gps
int GPSTxPin = 34; // Pin for the gps

const int pulsePin = 35; // Pin for PulseSensor might need to change to 0
const int pulseBlink = LED_BUILTIN; // Led for puls sensor Might need to change to 35
int pulseFade = 5; // How long a blink will last for puls sensor
int pulseFadeRate = 0; //

unsigned long updateTimer = 0;
unsigned long updateStart = 1000;
unsigned long start = millis();
int alarmStatus;
int pulseThreshold = 550;


Gpss gps1(GPSRxPin, GPSTxPin);
Wifi wifi;
Puls puls1(pulsePin, pulseBlink, pulseFade, pulseFadeRate);
Lora lora;
//PulseSensorPlayground pulseSensor;


void setup() {
  Serial.begin(115200);
  GPS.begin(9600, SERIAL_8N1, GPSTxPin, GPSRxPin); 
  gps1.gpsStart();
  //puls1.pulseStart();
  //puls1.pulseCheck();
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

void loop() { 
    //puls1.bpm3();
  if(millis() - updateTimer > UPDATEINTERVAL || updateTimer == 0 ){
    gps1.location();
    gps1.safeZone();
    gps1.HomeSafeZon();
    //puls1.BPM2();
    puls1.bpm3();
    //puls1.showBpm();
    //puls1.pulseCheck(); 
    lora.LoRaCall();

    
    
    updateTimer = millis();
  }      
}
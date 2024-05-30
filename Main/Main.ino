/*The differences between the include libaries are if it is between <> it is a libary publics on arduino libary manager and if it is between "" it is a class or libary made for this project*/
#include <LoRa.h> // this are the libarys used in the main.ino file
#include<PulseSensorPlayground.h>
#include<HTTPClient.h>
#include <OneWire.h> 

#include "Gps.h"// this is the class used in this project
#include "Puls.h"
#include "LoRa.h"
#include "Wifi.h"
#include "Temperature.h"
#include "Health.h"
#include "Screen.h"

#define UPDATEINTERVAL 6000 // the interval that it check and sends to the google sheet
#define Name "Lukas" // hardcode name of the patient 
#define Unit "unit_1" // hardcode unit number for the patient

// these are the pins used for LoRa 
#define SCK     5 // GPIO5  -- SX1278's SCK
#define MISO    19 // GPIO19 -- SX1278's MISnO
#define MOSI    27 // GPIO27 -- SX1278's MOSI
#define SS      18 // GPIO18 -- SX1278's CS
#define RST     14 // GPIO14 -- SX1278's RESET
#define DI0     26 // GPIO26 -- SX1278's IRQ(Interrupt Request)


int GPSRxPin = 12; // Pin for the gps
int GPSTxPin = 34; // Pin for the gps

const int pulsePin = 35; // Pin for PulseSensor might need to change to 0
const int pulseBlink = LED_BUILTIN; // Led for puls sensor Might need to change to 35
int pulseFade = 5; // How long a blink will last for puls sensor
int pulseFadeRate = 0; // how long it takes for the fading takes

// the pins for the oled
int screenSDA = 21;
int screenSCL = 22;

// variable used with timers 
unsigned long updateTimer = 0; // when the last update happenede
unsigned long updateStart = 1000; // time between updates in milliseconds 
unsigned long start = millis(); // using the internal time in milliseconds
int pulseThreshold = 550; // The voltage value for when a pulse is measured most be between 0 and 1024 

// Sets up the url that the data is send over
String finalurl; // set up the final url 
String url = "https://script.google.com/macros/s/AKfycbz15gCPmmGvLAgM-Vh5nfInJRi7SafzYq-hJadaY74RRNUR6bd9mHUvzwJEXuqBRDPwxw/exec?"; // google sheets url will add values in urlUpdate and will send it in statusupdate
String api_key = "AKfycbz15gCPmmGvLAgM-Vh5nfInJRi7SafzYq-hJadaY74RRNUR6bd9mHUvzwJEXuqBRDPwxw"; // the application id for the google sheet can be useful if more things are added
String payload; // set up a variable to get the responce back from the google sheet

// makes class constructors for all the classes and set the pin used for some of them
Gpss gps1(GPSRxPin, GPSTxPin);
Wifi wifi;
Puls puls1(pulsePin, pulseBlink, pulseFade, pulseFadeRate);
Lora lora;
Health health;
Temperature temperature(temperaturePin);
Screen screen;

// makes object constructors for the libaries
AXP20X_Class axp;
HardwareSerial GPS(1);
HTTPClient http;

//In the setup the device is started up 
void setup() {
  Serial.begin(115200); // starting the borad up with a Transmission Speed of 115200 
  GPS.begin(9600, SERIAL_8N1, GPSTxPin, GPSRxPin);  // starts the gps with a Transmission Speed of 9600 and on all the pins of the gps 
  gps1.gpsStart(); // runs the gps start check from the class gps
  wifi.wifiStart(); // runs the wifi start check from the class Wifi
  wifi.wifiCheck();// runs the wifi check from the class Wifi
  oneWire.begin(9600); // starts the one wire
  sensors.begin();
  wifi.wifiCheck2(); // check if the wifi is on if not then check what types of wifi there is 
  SPI.begin(SCK,MISO,MOSI,SS); // Start LoRa
  LoRa.setPins(SS,RST,DI0); // Starts LoRa on specifik pins
  Wire.begin(21, 22); // set the oled wire pins 
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // Startes the display 
  display.clearDisplay(); // Clear what is on the display 
  display.display(); // start displaying
   if (!axp.begin(Wire, AXP192_SLAVE_ADDRESS)) { // Start the gps with LoRa on the ttgo t-beam 
    Serial.println("AXP192 Begin PASS"); 
  } else {
    Serial.println("AXP192 Begin FAIL");// it did not work
  }
  axp.setPowerOutPut(AXP192_LDO2, AXP202_ON); // For power mangement of the device 
  axp.setPowerOutPut(AXP192_LDO3, AXP202_ON);
  axp.setPowerOutPut(AXP192_DCDC2, AXP202_ON);
  axp.setPowerOutPut(AXP192_EXTEN, AXP202_ON);
  axp.setPowerOutPut(AXP192_DCDC1, AXP202_ON);
  pulseSensor.analogInput(pulsePin);// Set the pin that the pulse sensor need to look at
  pulseSensor.blinkOnPulse(pulseBlink); // The blink interval for the pulse sensor
  pulseSensor.setThreshold(pulseThreshold); // The voltage value for when a pulse is measured most be between 0 and 1024 
  // see if the pulse sensor is there and prints in the terminal that if it works
  if (pulseSensor.begin()) {
    Serial.println("We created a pulseSensor Object !");  //This prints one time at Arduino power-up,  or on Arduino reset.  
  }
  Serial.print("LoRa Sender");// Start sending on LoRa
  /*
    the frequency for LoRa uses
    EU: 868
    Asia: 433
    North America: 915
    or:169
  */
  while(!LoRa.begin(866E6)){ // While it can transmidt on LoRa EU it will say LoRa Works
    Serial.println("LoRa Works"); 
  LoRa.setSyncWord(0xFF); // the sync word to communicate with this device ranges from 0-0xFF
  while (1); // set it to true while it can commuincate with other LoRa device or things network
  Serial.println("LoRa Works"); // if it works
  }
}

// functions and class will loop sometime on a timer other times constanly
void loop() { 
  // Will constanly check if the user is in either home or safe zone if yes will display the clock else it will display an arrow home
  if(inHomeZone == 1 && inSafeZone == 1 || inHomeZone == 1 && inSafeZone == 0 || inHomeZone == 0 && inSafeZone == 1  ){
  screen.displayClock();
  }else if(inHomeZone == 0 && inSafeZone == 0 ){
    screen.displayArrow();
  }
  // will look at these in a specifik time interval one a minute or if it is just starting
  if(millis() - updateTimer > UPDATEINTERVAL || updateTimer == 0 ){
    gps1.location(); // Will start gps if not started else just send and print gps location
    //gps1.Zone();
    gps1.HomeZoneCheck(); // Check if patient is in home zone
    gps1.SafeZoneCheck(); // check if patient is in safe zone
    //gps1.HomeSafeZone();
    gps1.timer(); // start clock
    puls1.bpm3(); // Gets bpm
    puls1.getNormalPulse(); // set a normal pulse after 10 rotations
    puls1.pulsStatus(); // Get the pulse status for the healthbuilt
    puls1.getRestingPulse(); // after one hour it will get resting bpm
    puls1.normalPulseCheck(); // will check if the normal pulse is within the range set up in Puls.h
    temperature.tempCheck(); // will get the temperature
    temperature.normalTempCheck(); // will see if the temperature is normal
    temperature.TempStatus(); // Get the temperature status for the healthbuilt
    health.calculateHealthScore(); // will calculate the health score from the temperature and pulse status
    health.extremLowHealthBuilt(); // testing if extrem health status
    health.extremHighHealthBuilt(); // testing if extrem health status
    health.HighHealthBuilt(); // testing if the health status is high
    health.LowHealthBuilt(); // testing if the health status is low
    health.GoodHealthBuilt(); // testing if the health status is good
    health.healthbuilt(); // switch case that looks at the health of the patient
    health.ExtremHealthStatus();  // see if there is extrem health conditions
    health.coldTemperature(); // if it is cold starts a timer to get a person to look at tham
    wifi.wifiCheck2();  // see what wifi there is that is no if no one if there use the gsm
    lora.LoRaCall(); // sends the data out on LoRa
    urlUpdate(); // check what type of url is used
    statusupdate(); // sends data to the google sheet and awaits a responce

  // do{// Not need but can the code run smoothly 
  //     while (GPS.available()) // while the gps is on it will read the gps location
  //       gps.encode(GPS.read());
  //   } while (start - UPDATEINTERVAL < 1000); 
   
    
    updateTimer = millis(); // set the update timer to timer and rest the timer loop
  }     
  if(ExtremHealth == 1){ // the the person is in extrem condiction will continuously sends data to google sheet amout ito about 4 a 0.5 min
    statusupdate();
  }
}

String urlUpdate(){

  // set what type of data to send to google sheet if at home or safe zone the data will be less and if not in either of them then send all data and if don't know where they patient is then send all 
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

// sends data to the google sheet and awaits the responce the responce
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
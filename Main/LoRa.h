#include <LoRa.h>
#include <TheThingsNetwork.h>
//#include <TTN_esp32.h>

// #define loraSerial Serial1
// #define debugSerial Serial

// #define freqPlan TTN_FP_EU868
// TheThingsNetwork ttn(loraSerial, debugSerial, freqPlan);

class Lora{
  public:
  // const char *devAddr = "00000000";
  // const char *nwkSKey = "40B95A611DA850A0320FE332F47D69CA";
  // const char *appSKey = "B1070A0E6AEE15031ED52C3A279EA394";

  int counter = 0; 

  void LoRaCall(){
    Serial.print("Sending pactket: ");
    Serial.println(counter);
    // ttn.personalize(devAddr, nwkSKey, appSKey);

    LoRa.beginPacket();
    //LoRa.print(Name);
    //LoRa.print(Unit);
    LoRa.print(gpsLat);
    LoRa.print(gpsLon);
    LoRa.print(BPM);
    //LoRa.print(temp);
    LoRa.endPacket();

    counter++;
  }

};
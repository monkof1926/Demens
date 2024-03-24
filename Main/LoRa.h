#include <LoRa.h>

class Lora{
  public:
  int counter = 0;


  void LoRaCall(){
    Serial.print("Sending pactket: ");
    Serial.println(counter);
  
    LoRa.beginPacket();
    LoRa.print("Hallo");
    LoRa.print(counter);
    LoRa.endPacket();

    counter++;
  }

};

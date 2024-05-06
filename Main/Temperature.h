#include <LoRa.h>
#include <OneWire.h>
#include <DallasTemperature.h>


#define ExtremHighTemp 39.0
#define ExtremLowTemp 36.0

double normalTemp;
double tempInterval = 0;
double highTemp = normalTemp + 1;
double lowTemp = normalTemp - 0.5;
int temphealth;
int normalTempHealth;

int  temperaturePin = 0;

OneWire oneWire(temperaturePin);

DallasTemperature sensors(&oneWire);

double temp = sensors.getTempCByIndex(0);

class Temperature{
  public:
  
  int temperaturePin;
  int tempCounter = 0;

  Temperature(int temperaturePin){
    this ->temperaturePin = temperaturePin;
    pinMode(this -> temperaturePin, INPUT);
  } 

  void tempCheck(){
    if(temp != DEVICE_DISCONNECTED_C){
      Serial.print("Temp is ");
      Serial.println(temp);

      LoRa.beginPacket();
      LoRa.print(temp);
      LoRa.endPacket();
    }else{
      Serial.println("Temperature sensor is not on");
    }
  }

  void normalTempCheck(){
    if(temp != 0){
      tempInterval + temp;
      
      for(tempCounter = 0; tempCounter == 10; tempCounter++){
          normalTemp = tempInterval / 10;
          Serial.print("normal temp: ");
          Serial.println(normalTemp);
          LoRa.beginPacket();
          LoRa.print(normalTemp);
          LoRa.endPacket();
      }
    }
  }

  void TempStatus(){
    if(temp >= highTemp){
      temphealth = 1;
      Serial.println("High temperature please check on them ");
    }else if(temp >= ExtremHighTemp){
      temphealth = 2;
      Serial.println("The Patient have a faber please help them ");
    }else if(temp <= lowTemp){
      temphealth =3;
      Serial.println("Low temp help them!");
    }else if(temp <= ExtremLowTemp){
      temphealth = 4;
      Serial.println("The patitent have extramly low temperature ");
    }else{
      temphealth = 0;
    }
  }

  void normalTempStatus(){
    if(normalTemp >= highTemp || normalTemp >= ExtremHighTemp){
      temphealth = 5;
      normalTempHealth = 1;
      Serial.println("High normal temperature");
    }else if(normalTemp <= lowTemp || normalTemp <= ExtremLowTemp){
      temphealth = 6;
      normalTempHealth = 2;
      Serial.println("Low normal temperature");
    }
  }

};
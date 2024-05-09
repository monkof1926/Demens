#include <LoRa.h>
#include <OneWire.h>
#include <DallasTemperature.h>


#define ExtremHighTemp 39.0
#define ExtremLowTemp 36.0

double normalTemp;
double tempInterval = 0;
double highTemp = normalTemp + 1;
double lowTemp = normalTemp - 0.5;
int tempStatus;
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

  //Health status uses temphealth and puslsestatus to calculate overall health score for good health a score of 80 to 100 is need
  // all good = 50
  // high temperature = 27
  // extremly high temperature = 13
  // low temperature = 6
  // extremly low temperature = 4
  //dont't have all the sensor need to make a real early warning score

  void TempStatus(){
    if(temp >= highTemp){
      tempStatus = 27;
      Serial.println("High temperature please check on them ");
    }else if(temp >= ExtremHighTemp){
      tempStatus = 13;
      Serial.println("The Patient have a faber please help them ");
    }else if(temp <= lowTemp){
      tempStatus = 6;
      Serial.println("Low temp help them!");
    }else if(temp <= ExtremLowTemp){
      tempStatus = 4;
      Serial.println("The patitent have extramly low temperature ");
    }else{
      tempStatus = 50;
    }
  }

  void normalTempStatus(){
    if(normalTemp >= highTemp || normalTemp >= ExtremHighTemp){
      tempStatus = 20;
      normalTempHealth = 1;
      Serial.println("High normal temperature");
    }else if(normalTemp <= lowTemp || normalTemp <= ExtremLowTemp){
      tempStatus = 10;
      normalTempHealth = 2;
      Serial.println("Low normal temperature");
    }
  }

};
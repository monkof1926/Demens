#include<PulseSensorPlayground.h>
#include <LoRa.h>


PulseSensorPlayground pulseSensor;

#define ExtremHighPulse 220
#define ExtremHighNormalPulse 220
#define ExtremLowPulse 30
#define ExtremLowNormalPulse 30

int BPM = pulseSensor.getBeatsPerMinute();
int normalPulse = 0;
int restingPulse;
int HighPulse = 130;
int LowPulse = 40;
int HighNormalPulse = normalPulse + 50;
int LowNormalPulse = normalPulse - 50;
int alarmStatus;
int pulseStatus;

class Puls{
  public:
  #define PROCESSING_VISUALIZER 1 

  volatile int rate[10];
  volatile unsigned long counter = 0;
  volatile unsigned long lastBeatTime = 0;
  unsigned long PulseTimer = 0;
  int PulseInterval = 60000;
  int pulseCounter;
  int pulseIntervalCounter = 0;
  int pulseRestInterval = 6000000;
  int pulseRestCounter;
  int pulseRestIntervalCounter = 0;
 

  int pulsePin; // set the pulse pin
  int pulseBlink; // set led pin
  int pulseFade;  // set fade pin
  int pulseFadeRate; // set fade rate


  unsigned long pulseTimer = 0;
  int pulseInterval = 20;

  int pulseAlarm;

  int pulseSignal = analogRead(pulsePin); 
  int pulseThreshold = 550;

  char pulse;
  Puls(int pulsePin, int pulseBlink, int pulseFade, int pulseFadeRate){
    this -> pulsePin = pulsePin;
    pinMode(this -> pulsePin, INPUT);
    this -> pulseBlink = pulseBlink;
    pinMode(this ->pulseBlink, OUTPUT);
    this -> pulseFade = pulseFade;
    pinMode(this -> pulseFade, OUTPUT);
    this -> pulseFadeRate = pulseFadeRate;
  }

  void pulseStart(){
    pulseSensor.analogInput(pulsePin);
    pulseSensor.blinkOnPulse(pulseBlink);
    pulseSensor.fadeOnPulse(pulseFade);

    pulseSensor.setSerial(Serial);
    //pulseSensor.setOutputType(outputType);
    pulseSensor.setThreshold(pulseThreshold);

  }


  void pulseCheck() {
    if(!pulseSensor.begin()){
      if(pulseSignal > 550){
        pulseSensor.begin();
        Serial.println("The sensor works" + pulseSignal);
      }else if(pulseSignal < 250){
      //for(;;){ // Infinite For loop
        Serial.println("Move the sensor closer");
      //}
      }else{
        Serial.print("The pulse sensor is not working");
      }
      }if(pulseSensor.begin()){ 
        pulseSignal;
        Serial.println("The pulse sensor is running" + pulseSignal);
    }
  }

  void BPM2(){

    if(pulseSensor.UsingHardwareTimer){
      
      pulseSensor.outputSample();
      

    }else if(pulseSensor.sawNewSample()){

      if(--pulseSensor.samplesUntilReport == (byte) 0){
          pulseSensor.samplesUntilReport = SAMPLES_PER_SERIAL_SAMPLE;
          pulseSensor.outputSample();
      }
    }

    if(pulseSensor.sawStartOfBeat()){
      pulseSensor.outputBeat();
    }
  }

  int bpm3(){
    if(pulseSensor.sawStartOfBeat()){
      int BPM21 = pulseSensor.getBeatsPerMinute();
      Serial.println("Your BPM is: ");
      Serial.println(BPM21);
      LoRa.beginPacket();
      LoRa.print(BPM21);
      LoRa.endPacket();   
      BPM = BPM21;  
    }
    return BPM;
  }
  int getNormalPulse(){
    int BPM23 = pulseSensor.getBeatsPerMinute();
    if(pulseSensor.sawStartOfBeat()){
      while(pulseSensor.sawNewSample()){
        pulseCounter += BPM23;
        pulseIntervalCounter++;

        if(pulseIntervalCounter == 10){
          normalPulse = pulseCounter / 10;
          Serial.println("Normal Pulse is ");
          Serial.print(normalPulse);
          pulseIntervalCounter = 0;

        }
      }
      return normalPulse;
    } else{
      Serial.println("Move the sensor closer");
      return normalPulse;
    }
  }

  //Health status uses temphealth and puslsestatus to calculate overall health score for good health a score of 80 to 100 is need
  // all good = 50
  // high pulse = 30
  // extremly high pulse = 20
  // low pulse = 10
  // extremly low pulse = 5
  
  void pulsStatus(){
    int BPM12 = pulseSensor.getBeatsPerMinute();
    
    if(BPM12 <= HighPulse){
      alarmStatus = 1;
      pulseStatus = 30;
      Serial.println("High pulse please lookout ");
  }else if(BPM12 <= ExtremHighPulse){
      alarmStatus = 3;
      pulseStatus = 20;
      Serial.println("Extrem high pulse get them help now!!");
  }else if(BPM12 >= LowPulse){
      alarmStatus = 1;
      pulseStatus = 10;
      Serial.println("Low puls Check it stis ok");
  }else if(BPM12 >= ExtremLowPulse){
      alarmStatus = 3;
      pulseStatus = 5;
      Serial.println("Extrem low pulse please cheack on patient");
    }
  }

  int getRestingPulse(){
    int BPM24 = pulseSensor.getBeatsPerMinute();
    if(pulseSensor.sawStartOfBeat()){
      while(pulseSensor.sawNewSample()){
        pulseRestCounter += BPM24;
        pulseRestIntervalCounter++;

        if(pulseRestIntervalCounter == 60){
          restingPulse = pulseRestCounter / 60;
          Serial.println("Resting pulse is ");
          Serial.print(restingPulse);
          pulseRestIntervalCounter = 0;
        }
      }
      return restingPulse;
    }else{
      Serial.println("Move the sensor closer");
      return restingPulse;
    }
  }


void normalPulseCheck(){
  if(normalPulse == 0){
    alarmStatus = 0;
    pulseStatus = 0;
    Serial.println("Need more time to get normal pulse ");
  }else if(normalPulse <= HighNormalPulse){
      alarmStatus = 1;
      pulseStatus = 1;
      Serial.println("High pulse please lookout ");
  }else if(normalPulse <= ExtremHighNormalPulse){
      alarmStatus = 3;
      pulseStatus = 2;
      Serial.println("Extrem high pulse get them help now!!");
  }else if(normalPulse >= LowNormalPulse && normalPulse > ExtremLowNormalPulse ){
      alarmStatus = 1;
      pulseStatus = 3;
      Serial.println("Low puls Check it stis ok");
  }else if(normalPulse >= ExtremLowNormalPulse){
      alarmStatus = 3;
      pulseStatus = 4;
      Serial.println("Extrem low pulse please cheack on patient");
    }
  }
};

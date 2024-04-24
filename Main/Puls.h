#include<PulseSensorPlayground.h>
#include <LoRa.h>


PulseSensorPlayground pulseSensor;

#define ExtremHighPulse 220
#define ExtremLowPulse 30

int BPM = pulseSensor.getBeatsPerMinute();
int normalPulse = 0;
int restingPulse;
int HighPulse = normalPulse + 50;
int LowPulse = normalPulse - 50;
int alarmStatus;
String healthStatus;

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

    //pulseSensor.setSerial(Serial);
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
    healthStatus = "Need_more_time";
    Serial.println("Need more time to get normal pulse ");
  }else if(normalPulse <= HighPulse){
      alarmStatus = 1;
      healthStatus = "High_puls";
      Serial.println("High pulse please lookout ");
  }else if(normalPulse <= ExtremHighPulse){
      alarmStatus = 3;
      healthStatus = "Extrem_High_puls";
      Serial.println("Extrem high pulse get them help now!!");
  }else if(normalPulse >= LowPulse && normalPulse > ExtremLowPulse ){
      alarmStatus = 1;
      healthStatus = "Low_puls";
      Serial.println("Low puls Check it stis ok");
  }else if(normalPulse >= ExtremLowPulse){
      alarmStatus = 3;
      healthStatus = "Extrem_Low_Puls";
      Serial.println("Extrem low pulse please cheack on patient");
    }
  }
};

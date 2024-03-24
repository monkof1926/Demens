#include<PulseSensorPlayground.h>

PulseSensorPlayground pulseSensor;

#define ExtremHighPulse 220
#define HighPulse 150
#define LowPulse 50
#define ExtremLowPulse 30

class Puls{
  public:
  #define PROCESSING_VISUALIZER 1 
 // #define SERIAL_PLOTTER 2 

  volatile int rate[10];
  volatile unsigned long counter = 0;
  volatile unsigned long lastBeatTime = 0;
  volatile int put = 512; // might need new names
  volatile int the = 512; // might need new names
  volatile int trash = 530; // might need new names
  volatile int amp = 0;
  volatile boolean firstBeat = true;
  volatile boolean secondBeat = false;

  int pulsePin; // set the pulse pin
  int pulseBlink; // set led pin
  int pulseFade;  // set fade pin
  int pulseFadeRate; // set fade rate

  volatile int BPM;
  volatile int Signal;
  volatile int IBI = 600;
  volatile boolean Pulse = false;
  volatile boolean QS = false;
  int outputType = SERIAL_PLOTTER;


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

  void serialOut(){
    switch(outputType){
      case PROCESSING_VISUALIZER:
        sendData('S', Signal); // use the sendDataToSerial function
        break;
      case SERIAL_PLOTTER:
       Serial.print(BPM);
        Serial.print(",");
        Serial.print(IBI);
        Serial.print(",");
        Serial.println(Signal);
      break;
      default:
      break;
    } 
  }

  void pulseStart(){
    pulseSensor.analogInput(pulsePin);
    pulseSensor.blinkOnPulse(pulseBlink);
    pulseSensor.fadeOnPulse(pulseFade);

    pulseSensor.setSerial(Serial);
    pulseSensor.setOutputType(outputType);
    pulseSensor.setThreshold(pulseThreshold);

    if(!pulseSensor.begin()){
      for(;;){
        digitalWrite(pulseBlink, LOW);
        delay(50);
        Serial.println("!");
        digitalWrite(pulseBlink, HIGH);
        delay(50);
      }
    }
  }



  void beatHappens(){
    switch(outputType){
      case PROCESSING_VISUALIZER:
      sendData('B', BPM); // send heart rate with a 'B' prefix and time between with 'Q' prefix
      sendData('Q', IBI);
      break;

      default:
      break;
    }
  }

  void sendData(char symbol, int data){
    Serial.print(symbol);
    Serial.println(data);
  }

  void pulseCheck() {
    if(!pulseSensor.begin()){
      if(pulseSignal > 300){
        pulseSensor.begin();
        Serial.println("The sensor works" + pulseSignal);
      }else if(pulseSignal < 250){
      for(;;){ // Infinite For loop
        Serial.println("Move the sensor closer");
      }
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
};

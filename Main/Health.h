
String healthStatus;
bool ExtremHealth = 0;
bool NeedHelp = 0;

class Health{
  public:
    String healthbuilt(){
      if(pulseStatus == 1){
          healthStatus = "High pulse";
          return healthStatus;
      }else if(pulseStatus == 2){
          healthStatus = "Extremly high pulse";
          ExtremHealth = 1;
          return healthStatus;
      }else if(pulseStatus == 3){
          healthStatus = "Low pulse";
          return healthStatus;
      }else if(pulseStatus == 4){
          healthStatus = "Extremly low pulse";
          ExtremHealth = 1;
          return healthStatus;
      }else if(temphealth == 1){
          healthStatus = "High temperature";
          return healthStatus;
      }else if(temphealth == 2){
          healthStatus = "Extrmly high temperature";
          ExtremHealth = 1;
          return healthStatus;
      }else if(temphealth == 3){
          healthStatus = "Low temperature";
          return healthStatus;
      }else if(temphealth == 4){
          healthStatus = "Extremly low temperature";
          ExtremHealth = 1;
          return healthStatus;
      }else if(pulseStatus == 1 && temphealth == 1 ){
          healthStatus = "High pulse and high temperature";
          return healthStatus;
      }else if(pulseStatus == 2 && temphealth == 2){
          healthStatus = "Extremly high pulse and temperature";
          ExtremHealth = 1;
          return healthStatus;
      }else if(pulseStatus == 3 && temphealth == 3){
          healthStatus = "Low pulse and temperature";
          return healthStatus;
      }else if(pulseStatus == 4 && temphealth == 4){
          healthStatus = "Extremly low pulse and temperature";
          ExtremHealth = 1;
          return healthStatus;
      }else if(pulseStatus == 1 && temphealth == 2){
          healthStatus = " Low pulse and Extremly high temperature";
          ExtremHealth = 1;
          return healthStatus;
      }else if(pulseStatus == 1 && temphealth == 3){
          healthStatus = "Low pulse and low temperaute";
          return healthStatus;
      }else if(pulseStatus == 1 && temphealth == 4){
          healthStatus = "Low pulse and extremly low temperature";
          return healthStatus;
      }else if(pulseStatus == 2 && temphealth == 1){
          healthStatus = "Extremly low pulse and high temperature";
          ExtremHealth = 1;
          return healthStatus;
      }else if(pulseStatus == 2 && temphealth == 3 ){
          healthStatus = "Extremly low pulse and low temperature";
          ExtremHealth = 1;
          return healthStatus;
      }else if(pulseStatus == 2 && temphealth == 4){
          healthStatus = "Extremly low pulse and temperature";
          ExtremHealth = 1;
          return healthStatus;
      }else if(pulseStatus == 3 && temphealth == 4){
          healthStatus = "Low pulse and extremly low temperature";
          ExtremHealth = 1;
          return healthStatus;
      }else{
          healthStatus = "All good";
          return healthStatus;
      }
    }
    
    void ExtremHealthStatus(){
      if(ExtremHealth == 1){
          NeedHelp = 1;
          Serial.print("Need help now at ");
          Serial.println(gpsLat,gpsLon);
      }else{
          NeedHelp = 0;
      }
    }
};
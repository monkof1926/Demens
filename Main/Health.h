
String healthStatus;
bool ExtremHealth = 0;
bool NeedHelp = 0;
bool coldTemp = 0;
int healthScore;
double healthtime = millis();

class Health{
  public:
  int calculateHealthScore(){
    healthScore = tempStatus + pulseStatus;
    while(healthScore == 0){
      if(tempStatus == 5 || tempStatus == 20 || pulseStatus == 5 || pulseStatus == 20){
        Serial.println("Check on them");
        ExtremHealth = 1;
      }else{
      Serial.print("HealthScore is ");
      Serial.println(healthScore);
      return healthScore;
      }
    }if(healthScore == 0){
      if(tempStatus == 0){
        Serial.print("Check temperature sensor ");
        Serial.println(tempStatus);
      }if(pulseStatus == 0){
        Serial.print("Check pulseSensor sensor ");
        Serial.println(pulseStatus);
      }
    }
  }

  String healthbuilt(){
      while(healthScore == 0){
        switch(healthScore){
          case 81 ... 100: // Good health
          if(healthScore == 100){
            healthStatus = "All_good";
            Serial.print("All good ");
            alarmStatus = 0;
            return healthStatus;
          }
          break;
          case 78 ... 80:// high pulse
          if(healthScore == 80){
            healthStatus = "High_pulse";
            Serial.print("High pulse ");
            Serial.println(BPM);
            alarmStatus = 1;
            return healthStatus;
          }
          break;
          case 71 ... 77: // High temperature
          if(healthScore == 77){
            healthStatus = "High_temperature";
            Serial.print("Temperature is high ");
            Serial.println(temp);
            alarmStatus = 1;
            return healthStatus;
          }
          break;
          case 64 ... 70:
          if(healthScore == 70){
            healthStatus = "Extremly_high_pulse";
            Serial.print("Extremly high pulse ");
            Serial.println(BPM);
            alarmStatus = 2;
            ExtremHealth = 1;
            return healthStatus;
          }
          break;
          case 61 ... 63:
          if(healthScore == 63){
            healthStatus = "Extremly_high_temperature";
            Serial.print("Extremly high temperature ");
            Serial.println(temp);
            alarmStatus = 3;
            ExtremHealth = 1;
            return healthStatus;
          }
          break;
          case 58 ... 60:
          if(healthScore == 60){
            healthStatus = "Low_pulse";
            Serial.print("Low pulse ");
            Serial.println(BPM);
            alarmStatus = 2;
            return healthStatus;
          }
          break;
          case 57: 
          if(healthScore == 57){
            healthStatus = "High_temperature_and_pulse";
            Serial.print("High temperature and pulse ");
            Serial.println(temp, BPM);
            alarmStatus = 4;
            return healthStatus;
          }
          break;
          case 56:
          if(healthScore == 56){
            healthStatus = "Low_temperature";
            Serial.print("Low temperature ");
            Serial.println(temp);
            alarmStatus = 3;
            coldTemp = 1;
            return healthStatus;
          }
          break;
          case 55:
          if(healthScore == 55){
            healthStatus = "Extemly_low_pulse";
            Serial.print("Extremly low pulse ");
            Serial.println(BPM);
            alarmStatus = 5;
            ExtremHealth = 1;
            return healthStatus;
          }
          break;
          case 54:
          if(healthScore == 54){
            healthStatus = "Extremly_low_temperature";
            Serial.print("Extremly low temperature ");
            Serial.println(temp);
            alarmStatus = 6;
            coldTemp = 1;
            ExtremHealth = 1;
            return healthStatus;
          }
          break;
          case 33:
          if(healthScore == 33){
            healthStatus = "Extremly_high_temperature_and_pulse";
            Serial.print("Extremly high temperature and pulse ");
            Serial.println(temp, BPM);
            alarmStatus = 4;
            ExtremHealth = 1;
            return healthStatus;
          }
          break;
          case 27:
          if(healthScore == 27){
            healthStatus = "Temp good don't know pulse";
            Serial.print("pulse don't work");
            Serial.println(BPM);
            alarmStatus = 0;
            return healthStatus;
          }
          break;
          case 17:
          if(healthScore == 17){
            healthStatus = "Low_temperature_and_pulse";
            Serial.print("Low temperature and pulse ");
            Serial.println(temp, BPM);
            alarmStatus = 4;
            coldTemp = 1;
            return healthStatus;
          }
          break;
          case 11:
          if(healthScore == 11){
            healthStatus = "Extremly_low_temperature_and_pulse";
            Serial.print("Extremly low temperature and pulse ");
            Serial.println(temp, BPM);
            alarmStatus = 7;
            ExtremHealth = 1;
            return healthStatus;
          }
          break;
          case 0:
          if(healthScore == 0){
            healthStatus = "All_good";
            Serial.print("All good ");
            alarmStatus = 0;
            return healthStatus;
          }
          break;
          defualt:
            healthStatus = "All_good";
            Serial.println("All good ");
            alarmStatus = 0;
            return healthStatus;
        }
      }
    }
    
    void ExtremHealthStatus(){
      if(ExtremHealth == 1){
          NeedHelp = 1;
          Serial.print("Need_help_now_at");
          Serial.println(gpsLat,gpsLon);
      }else{
          NeedHelp = 0;
      }
    }

    void coldTemperature(){
      if(coldTemp == 1){
        Serial.print("They are cold please check on them");
        Serial.println(gpsLat,gpsLon);
      }
    }
};
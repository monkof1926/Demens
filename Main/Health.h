
//making the global string health status that can be passed between class 
String healthStatus;

//global bool descripning if the patient is in a extremly health conditions and need immedily needs helps
bool ExtremHealth = 0;
// global variable that says if the patient needs help now
bool NeedHelp = 0;
// As having a lower temperature needs help faster then a high temperature
bool coldTemp = 0;
// global varuable health score that is used to tell the health of the patient
int healthScore;
// a Timer that starts if the patient have lower temperature as a lower temperature leads to quiker death
double healthtime = millis();
// The global string that is send to the google sheet to tell if the patient needs help
String needHelp;

/*Health class is made to measure the patient health to do this a health score is made by pulsing the temperaute and plus them together
 after this it will look at what the health is then it will send out the general health of the patient and say if they need help. */
class Health{
  /*The discriptors of the health status made in pairs from all good to extrmly lo temperature and pulse 
to get this a health score is calculate by plus the temperature status and the pluse status together and from there a status on the patient health can be derive
and from there a warning can be made if need */
String allGood = "All_good";

String highPulse = "High_pulse";
String highTemperature = "High_temperature";

String lowPulse = "Low_pulse";
String lowTemperature = "Low_temperature";

String extremlyHighPulse = "Extremly_high_pulse";
String extremlyHighTemperature = "Extremly_high_temperature";

String extemlyLowPulse = "Extemly_low_pulse";
String extremlyLowTemperature = "Extremly_low_temperature";

String highTemperatureAndPulse = "High_temperature_and_pulse";
String extremlyHighTemperatureAndPulse = "Extremly_high_temperature_and_pulse";

String lowTemperatureAndPulse = "Low_temperature_and_pulse";
String extremlyLowTemperatureAndPulse = "Extremly_low_temperature_and_pulse";

String startingUp = "Starting_up"; // the default is starting up

String needHelpNow = "Need_help_now_at";// this is if the patient is in need of immediate help

String tempGoodDontKnowPulse = "Temp_good_don't_know_pulse"; // if the pulse sensor is broken and the temperature sensor works
  // all under this is public and can be send and used by all class where it is called
  public:
  //Makes a health score by looking at what the temperature and pulse status is then sees if it is any of the extreme scores if it is then it will ask us to look at the patient if needed 
  int calculateHealthScore(){
    healthScore = tempStatus + pulseStatus;
    while(healthScore == 0){
      if(tempStatus == 4 || tempStatus == 13 || pulseStatus == 5 || pulseStatus == 20){
        Serial.println("Check on them");
        ExtremHealth = 1;
      }else{
      Serial.print("HealthScore is ");
      Serial.println(healthScore);
      return healthScore;
      }
    }if(healthScore == 0){ // if it only gets zero it will ask us to look at the sensors as they might need repair and print what the pathient temperature and pulse is if one of them is zero
      if(tempStatus == 0){
        Serial.print("Check temperature sensor ");
        Serial.println(tempStatus);
      }if(pulseStatus == 0){
        Serial.print("Check pulseSensor sensor ");
        Serial.println(pulseStatus);
      }
    }
  }


/*healthbuilt is a switch case that looks at the health scores and then tells us what the over all health of the patient is and will send a healthStatus to the google sheet so a caretaker can help if needed
  some of the scores are a between 2 numbers this is because there need to be room for more sensors for a couple of example of this there is if the healthscore is between 81 and 100 is good health score 
  if the health score is between 78 and 80 show a high pulse and will print the bpm if the the health score is between 71 and 77 the patient have high temperature one of the score is 
  if the pulse sensor is brokken and because the temperature is high it will then show 27 the defualt is shows the sensors starting up the if statements are for testing when more health scores are added */
  String healthbuilt(){
        switch(healthScore){
          case 81 ... 100: // Good health
          if(healthScore == 100){
            healthStatus = allGood;
            Serial.print("All good ");
            alarmStatus = 0;
            return healthStatus;
          }
          break;
          case 78 ... 80:// high pulse
          if(healthScore == 80){
            healthStatus = highPulse;
            Serial.print(highPulse);
            Serial.println(BPM);
            alarmStatus = 1;
            return healthStatus;
          }
          break;
          case 71 ... 77: // High temperature
          if(healthScore == 77){
            healthStatus = highTemperature;
            Serial.print(highTemperature);
            Serial.println(temp);
            alarmStatus = 1;
            return healthStatus;
          }
          break;
          case 64 ... 70:
          if(healthScore == 70){
            healthStatus = extremlyHighPulse;
            Serial.print(extremlyHighPulse);
            Serial.println(BPM);
            alarmStatus = 2;
            ExtremHealth = 1;
            return healthStatus;
          }
          break;
          case 61 ... 63:
          if(healthScore == 63){
            healthStatus = extremlyHighTemperature;
            Serial.print(extremlyHighTemperature);
            Serial.println(temp);
            alarmStatus = 3;
            ExtremHealth = 1;
            return healthStatus;
          }
          break;
          case 58 ... 60:
          if(healthScore == 60){
            healthStatus = lowPulse;
            Serial.print(lowPulse);
            Serial.println(BPM);
            alarmStatus = 2;
            return healthStatus;
          }
          break;
          case 57: 
          if(healthScore == 57){
            healthStatus = highTemperatureAndPulse;
            Serial.print(highTemperatureAndPulse);
            Serial.println(temp, BPM);
            alarmStatus = 4;
            return healthStatus;
          }
          break;
          case 56:
          if(healthScore == 56){
            healthStatus = lowTemperature;
            Serial.print(lowTemperature);
            Serial.println(temp);
            alarmStatus = 3;
            coldTemp = 1;
            return healthStatus;
          }
          break;
          case 55:
          if(healthScore == 55){
            healthStatus = extemlyLowPulse;
            Serial.print(extemlyLowPulse);
            Serial.println(BPM);
            alarmStatus = 5;
            ExtremHealth = 1;
            return healthStatus;
          }
          break;
          case 54:
          if(healthScore == 54){
            healthStatus = extremlyLowTemperature;
            Serial.print(extremlyLowTemperature);
            Serial.println(temp);
            alarmStatus = 6;
            coldTemp = 1;
            ExtremHealth = 1;
            return healthStatus;
          }
          break;
          case 33:
          if(healthScore == 33){
            healthStatus = extremlyHighTemperatureAndPulse;
            Serial.print(extremlyHighTemperatureAndPulse);
            Serial.println(temp, BPM);
            alarmStatus = 4;
            ExtremHealth = 1;
            return healthStatus;
          }
          break;
          case 27:
          if(healthScore == 27){
            healthStatus = tempGoodDontKnowPulse;
            Serial.print(tempGoodDontKnowPulse);
            Serial.println(BPM);
            alarmStatus = 0;
            return healthStatus;
          }
          break;
          case 17:
          if(healthScore == 17){
            healthStatus = lowTemperatureAndPulse;
            Serial.print(lowTemperatureAndPulse);
            Serial.println(temp, BPM);
            alarmStatus = 4;
            coldTemp = 1;
            return healthStatus;
          }
          break;
          case 11:
          if(healthScore == 11){
            healthStatus = extremlyLowTemperatureAndPulse;
            Serial.print(extremlyLowTemperatureAndPulse);
            Serial.println(temp, BPM);
            alarmStatus = 7;
            ExtremHealth = 1;
            return healthStatus;
          }
          break;
          defualt:
            healthStatus = startingUp;
            Serial.println(startingUp);
            return healthStatus;
        }
    }   
    
    void ExtremHealthStatus(){
      if(ExtremHealth == 1){
          NeedHelp = 1;
          healthStatus = needHelpNow;
          Serial.print(needHelpNow);
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

  /*The next parts are for testing  as the healthbuilt don't allways work so for testing pourpese and can be used insted of healthbuilt if needed
    extremLowHealthBuilt and extremHighHealthBuilt  will react when the health score show extremly bad health either in extrenly high or low ways 
    and HighHealthBuilt show if the patient have a high pulse or temperature and LowHealthBuilt react if the patient have low pulse or temperature
    The GoodHealthBuilt is to see if all is good and all sensors works and the patient is in good health */
  String extremLowHealthBuilt(){
    if(healthScore == 11){
        healthStatus = "Extremly_low_temperature_and_pulse";
            Serial.print("Extremly low temperature and pulse ");
            Serial.println(temp, BPM);
            alarmStatus = 7;
            ExtremHealth = 1;
            needHelp = "Yes_Fast";
            return (healthStatus, needHelp);
    }else if(healthScore == 54){
        healthStatus = "Extremly_low_temperature";
            Serial.print("Extremly low temperature ");
            Serial.println(temp);
            alarmStatus = 6;
            coldTemp = 1;
            ExtremHealth = 1;
            needHelp = "Yes_Fast";
            return (healthStatus, needHelp );
    }else if(healthScore == 55){
      healthStatus = "Extemly_low_pulse";
            Serial.print("Extremly low pulse ");
            Serial.println(BPM);
            alarmStatus = 5;
            ExtremHealth = 1;
            needHelp = "Yes_Fast";
            return (healthStatus, needHelp );
    }
  }

  String extremHighHealthBuilt(){
    if(healthScore == 33){
        healthStatus = "Extremly_high_temperature_and_pulse";
            Serial.print("Extremly high temperature and pulse ");
            Serial.println(temp, BPM);
            alarmStatus = 4;
            ExtremHealth = 1;
            needHelp = "Yes_Fast";
            return (healthStatus, needHelp );
    }else if(healthScore == 63){
        healthStatus = "Extremly_high_temperature";
            Serial.print("Extremly high temperature ");
            Serial.println(temp);
            alarmStatus = 3;
            ExtremHealth = 1;
            needHelp = "Yes_Fast";
            return (healthStatus, needHelp );
    }else if(healthScore == 70){
        healthStatus = "Extremly_high_pulse";
            Serial.print("Extremly high pulse ");
            Serial.println(BPM);
            alarmStatus = 2;
            ExtremHealth = 1;
            needHelp = "Yes_Fast";
            return (healthStatus, needHelp );
    }
  }

  String HighHealthBuilt(){
    if(healthScore == 57){
      healthStatus = "High_temperature_and_pulse";
            Serial.print("High temperature and pulse ");
            Serial.println(temp, BPM);
            alarmStatus = 4;
            needHelp = "Check_on_them";
            return (healthStatus, needHelp );
    }else if(healthScore == 77){
        healthStatus = "High_temperature";
            Serial.print("Temperature is high ");
            Serial.println(temp);
            alarmStatus = 1;
            needHelp = "Ceck_on_them";
            return (healthStatus, needHelp );
    }else if(healthScore == 80){
        healthStatus = "High_pulse";
            Serial.print("High pulse ");
            Serial.println(BPM);
            alarmStatus = 1;
            needHelp = "Check_on_them";
            return (healthStatus, needHelp );
    }
  }

  String LowHealthBuilt(){
    if(healthScore == 56){
        healthStatus = "Low_temperature";
            Serial.print("Low temperature ");
            Serial.println(temp);
            alarmStatus = 3;
            coldTemp = 1;
            needHelp = "Yes";
            return (healthStatus, needHelp );
    }else if(healthScore == 17){
        healthStatus = "Low_temperature_and_pulse";
            Serial.print("Low temperature and pulse ");
            Serial.println(temp, BPM);
            alarmStatus = 4;
            coldTemp = 1;
            needHelp = "Yes";
            return (healthStatus, needHelp );
    }else if(healthScore == 60){
        healthStatus = "Low_pulse";
            Serial.print("Low pulse ");
            Serial.println(BPM);
            alarmStatus = 2;
            needHelp = "Yes";
            return (healthStatus, needHelp );
    }
  }

  String GoodHealthBuilt(){
    if(healthScore == 100){
      healthStatus = "All_Good";
            Serial.print("All good ");
            alarmStatus = 0;
            needHelp = "No";
            return (healthStatus, needHelp );
      
    }
  }
};
#include<Adafruit_SSD1306.h>
//#include<Adafruit_HMC5883.h>

Adafruit_SSD1306 display(4);

double currentLat = gpsLat;
double curremtLon = gpsLon;

//double HomeLocation = (HomeZoneLat, HomeZoneLon); 

//double currentHeading = readCompass();

//double changeAngle currentHeading - HomeLocation;

class Screen{
  public:


    void displayClock(){
    display.clearDisplay();


    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.setTextSize(1);
    display.println(Time);
    display.setTextSize(1);
    display.print(Date);
 
    display.display();

    }

    void displayArrow(){
      display.setTextColor(WHITE);
      display.setCursor(0,0);
      display.setTextSize(1); 
      //display.drawTriangle(24, 1, 3, 55, 45, 55, WHITE);
      display.fillTriangle(50, 1, 15, 30, 40, 75, WHITE);

      display.display();
    }
    void displayImage(){
      
    }

    void drawTriangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
};
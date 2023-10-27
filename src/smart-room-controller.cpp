/* 
 * Project myProject
 * Author: Your Name
 * Date: 
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

#include "Particle.h"
#include <IoTClassroom_CNM.h>
#include <Encoder.h>
#include <neopixel.h>
#include <colors.h>
#include <Adafruit_BME280.h>
#include <Adafruit_SSD1306.h>
#include <hue.h>




SYSTEM_MODE(MANUAL);



const int PIXELCOUNT = 2;
Adafruit_NeoPixel pixel(PIXELCOUNT, SPI1, WS2812B);
Adafruit_BME280 bme;

Adafruit_SSD1306 display(-1);

Encoder enCoder(D8,D9);

int tempC;
int tempF;
int bmeStatus;
int encRead;
int thermoStat;
int thermoSet;
int fanOn;
int heatOn;
int i;
int currTime;
int prevTime;
int bulbState;
float t;
char degree = 0xF8;
bool heat;
bool cool;
bool lftIsclicked;
bool rtIsclicked;
bool encIsclicked;
bool lftIson;
const int BUZZER = D13;
const int wemoHeat=5;
const int wemoCool=3;
unsigned int lastTone;
Button leftButton (D3); 
Button rightButton (D4);
Button encButton (D15);


void setup() {
    

    Serial.begin(9600);
    waitFor(Serial.isConnected,15000);

    WiFi.on();
    WiFi.clearCredentials();
    WiFi.setCredentials("IoTNetwork");

    WiFi.connect();
    while(WiFi.connecting()){
        Serial.printf(".");
    }
    Serial.printf("\n\n");

    pinMode(BUZZER,OUTPUT);

    pixel.begin();
    pixel.show();
    pixel.setBrightness(60);

    bmeStatus = bme.begin(0x76);
    if (bmeStatus == false){
        Serial.print("BME failed to start");
    }
    
    display.begin(SSD1306_SWITCHCAPVCC,0x3C);
    display.setTextColor(WHITE);
    display.clearDisplay();

  



    


}


void loop() {



currTime = millis();
tempC = bme.readTemperature();
tempF = map(tempC,0,100,32,212);
encRead = enCoder.read();
thermoSet = map(encRead,0,96,60,100);
cool = false;
heat = false;
fanOn = thermoSet+3;
heatOn = thermoSet-3;
pixel.clear();
noTone(BUZZER);







if(tempF<heatOn){
    heat = true;
}

if(tempF>fanOn){
    cool = true;
}

if(encRead<0){
    enCoder.write(0);
    encRead = 0;
}
if(encRead>96){
    enCoder.write(96);
    encRead = 96;
}

if(heat){
  
    wemoWrite(wemoHeat,HIGH);
    pixel.setPixelColor(0,red);
    pixel.show();
    tone(BUZZER,2000,500);
}
    

if(cool){
    t = millis()/1000.0;
    while(t<1){
    tone(BUZZER,2000);
    // break;
    }
    noTone(BUZZER);
    wemoWrite(wemoCool,HIGH);
    pixel.setPixelColor(1,blue);
    pixel.show();
}
    

if (cool==false && heat==false){
    wemoWrite(wemoCool,LOW);
    wemoWrite(wemoHeat,LOW);
    pixel.clear();
    pixel.show();
    noTone(BUZZER);
}
   
display.setTextSize(2);
display.printf("Room Temp\n");
display.setTextSize(3);
display.printf("%i%cF\n",tempF,degree);
display.setTextSize(1);
display.printf("Target Temp = %i%cF",thermoSet,degree);
display.display();
display.clearDisplay();
display.setCursor(0,0);

if(leftButton.isClicked()){
    lftIsclicked = !lftIsclicked;     
    Serial.printf("left button %i\n",lftIsclicked);
}


 if(rightButton.isClicked()){
    rtIsclicked = !rtIsclicked;
    Serial.printf("right button %i\n",rtIsclicked);
}




if(lftIsclicked){
    if(rtIsclicked){
          setHue(4,true,HueRed,50,255);
    }

    else{
        setHue(4,true,255,100,0.33);

    }
    
   
      
    }

else{
    setHue(4,false);
}

    

}
 
    
 
        

        

  
 
     
     



     
   


    

    



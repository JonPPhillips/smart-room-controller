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


const int BUZZER = D13;
const int PIXELCOUNT = 2;
Adafruit_NeoPixel pixel(PIXELCOUNT, SPI1, WS2812B);
Adafruit_BME280 bme;

int OLED_RESET (D4);
Adafruit_SSD1306 display(OLED_RESET);

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
char degree = 0xF8;
bool heat;
bool cool;
bool lftIsclicked;
bool rtIsclicked;
bool encIsclicked;
const int wemoHeat=0;
const int wemoCool=3;
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
  
    // wemoWrite(wemoHeat,HIGH);
    pixel.setPixelColor(0,red);
    pixel.show();
    tone(BUZZER,2000,500);
}
    

if(cool){

    wemoWrite(wemoCool,HIGH);
    pixel.setPixelColor(1,blue);
    pixel.show();
    tone(BUZZER,1000,500);
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
}


if(lftIsclicked){
    for(i=1;i<7;i++){
        setHue(4,true,225,100,0.33);
    }
    

    

}
    else{
        for(i=1;i<7;i++){
            setHue(4,false);
        }

    }


if(lftIsclicked);
    if(rightButton.isClicked()){
        rtIsclicked = !rtIsclicked;
    }

    if(rtIsclicked){
        for(i=1;i<8;i++){
            setHue(4,true, HueRainbow[random(8,0)],50,255);
        }
    }
    else{
        for(i=1;i<8;i++){
            setHue(4,true,255,100,0.33);
        }
    }


    

}

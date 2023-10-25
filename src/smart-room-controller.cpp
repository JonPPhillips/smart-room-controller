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


SYSTEM_MODE(SEMI_AUTOMATIC);


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
char degree = 0xF8;






void setup() {
    Serial.begin(9600);

    // Serial.begin(9600);
    // waitFor(Serial.isConnected,15000);

    // WiFi.on();
    // WiFi.clearCredentials();
    // WiFi.setCredentials("IoTNetwork");

    // WiFi.connect();
    // while(WiFi.connecting()){
    //     Serial.printf(".");
    // }
    // Serial.printf("\n\n");

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




tempC = bme.readTemperature();
tempF = map(tempC,0,100,32,212);
encRead = enCoder.read();
thermoSet = map(encRead,0,96,60,100);

if(encRead<0){
    enCoder.write(0);
    encRead = 0;
}
if(encRead>96){
    enCoder.write(96);
    encRead = 96;
}

display.setTextSize(2);
display.printf("Room Temp\n");SYSTEM_MODE(SEMI_AUTOMATIC);
display.setTextSize(3);
display.printf("%i%cF\n",tempF,degree);
display.setTextSize(1);
display.printf("Target Temp = %i%cF",thermoSet,degree);
display.display();
display.clearDisplay();
display.setCursor(0,0);

Serial.printf("tempF = %i\n thermoSet = %i\n",tempF,thermoSet);

while((tempF-3)<thermoSet){
    pixel.setPixelColor(0,red);
    pixel.show();
    tone(BUZZER,1000,1000);    
}
    // else{
    //     pixel.clear();
    //     pixel.show();
    //     noTone(BUZZER);
    // }

while((tempF+3)>thermoSet){
    pixel.setPixelColor(1,blue);
    tone(BUZZER,2000,1000);
    
}
    // else{
    //     pixel.clear();
    //     pixel.show();
    //     noTone(BUZZER);
    // }




}

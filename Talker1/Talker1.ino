// This is Talker1 that takes serial text and transmits it over I2C to the other Uno board. 

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
//#include <Adafruit_I2CDevice.h>			see how we go with wire

#define SCREEN_WIDTH    128  // OLED display width, in pixels
#define SCREEN_HEIGHT   32   // OLED display height, in pixels
#define OLED_RESET      -1   // Reset pin # (or -1 if sharing Arduino reset pin  
#define SCREEN_ADDRESS  0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Initialise variables
int cw = SSD1306_WHITE;
int cb = SSD1306_BLACK;
int x = 0;
int y = 0;

int ledPin = 13;              // select the pin for the LED
char _bufferS[32] = "Hello!";             // for the serial text
char _bufferW[32] = "Hello!";             // for the serial text
int byteNum = 0;

int deviceAddr = 0x09;      // address of the device I want to talk to
int registerAddr = 0;       // the register I want to look at
int dataI2C = 0;            // variable for the data to send or receive
String intStr = String(0);          // string so that I can send ints

void setup() {
  Serial.begin(9600);      		// initialize a serial portt to talk:
  
  pinMode(ledPin, OUTPUT);    	// declare the ledPin as an OUTPUT:
  digitalWrite(ledPin, LOW); 	// turn the ledPin off to show error
  
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {  // Old Address 0x3D for 128x64
    digitalWrite(ledPin, HIGH); // turn the ledPin on to show error
  } 

  display.display();
  display.clearDisplay();
  display.setTextSize(2);
  
  Wire.begin(); // join I2C bus (address optional for master)
}



void loop() {
  display.display();
  display.setCursor(x, y);
  display.setTextColor(cw);

  if (Serial.available()) {

    // read from port 0, send to I2C:
    while (Serial.available()) {
      int inByte = Serial.read();
      _bufferS[byteNum++] = inByte;
      
      if ((inByte > 64) & (inByte < 72)) {
        inByte = inByte - 55 ;
      } else if ((inByte > 96) & (inByte < 103)) {
        inByte = inByte - 87;
      } else if (isDigit(inByte)) {
        inByte = inByte - 48;
      }

      Wire.beginTransmission(0x08); 	// transmit to device #8
      Wire.write(inByte);        	// sends a byte
      Wire.endTransmission();    	// stop transmitting
      
      delay(100);
      display.clearDisplay();          
      display.fillRect(x,y, 127, 63, cb); // Overwrite with black rectangle
    }
    
    display.setCursor(x, y);
    display.println(_bufferS);         // display after the all chars read.
    display.println(_bufferW);         // display after the all chars read.
    display.display();
  } 

  if (Wire.available()) {
    while(Wire.available()) {
      int inByte = Wire.read();
      _bufferW[byteNum++] = inByte;
      delay(100);
      display.clearDisplay();
      display.fillRect(x,y, 127, 63, cb); // Overwrite with black rectangle
    }
      
    display.setCursor(x, y);
    display.println(_bufferS);         // display after the all chars read.
    display.println(_bufferW);         // display after the all chars read.
    display.display();

    Serial.println(_bufferW);
  }
  byteNum = 0;
}

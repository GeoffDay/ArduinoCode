// This is Talker2 that takes serial text and transmits it over I2C to the other Uno board. 
#include <LiquidCrystal.h>
#include <Wire.h>

// initialize the library by associating any needed LCD interface pin with the arduino pin number it is connected to
const int rs = 7, en = 6, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int ledPin = 13;              // select the pin for the LED
char _bufferS[8] = "Hello!";             // for the serial text
char _bufferW[8] = "Hello!";             // for the serial text
int byteNum = 0;

int deviceAddr = 0x09;      // address of the device I want to talk to
int registerAddr = 0x06;       // the register I want to look at
int dataI2C = 0;            // variable for the data to send or receive
String intStr = String(0);          // string so that I can send ints

void setup() {
  lcd.begin(16, 2);          // set up the LCD's number of columns and rows:
  lcd.display();             // idealy an i2c display would save a few pins.

  Serial.begin(9600);      		// initialize a serial portt to talk:
  
  pinMode(ledPin, OUTPUT);    	// declare the ledPin as an OUTPUT:
  digitalWrite(ledPin, LOW); 	// turn the ledPin off to show error
  
  Wire.begin(); // join I2C bus (address optional for master)
}



void loop() {
  if (Serial.available()) {
    lcd.clear();                        // clear the screen
    lcd.setCursor(0,0);

    // read from port 0, send to I2C:
    while (Serial.available()) {
      delay(100);  
      int inByte = Serial.read();       // read a byte
      _bufferS[byteNum++] = inByte;     // stick the raw data into a buffer for the serial
         
      if (inByte != 0x0a) {             // strip off the line feed for lcd
        lcd.write(inByte); 
      }
      // lcd.scrollDisplayRight();
      if ((inByte > 64) & (inByte < 72)) {  // convert to the integer equiv.
        inByte = inByte - 55 ;
      } else if ((inByte > 96) & (inByte < 103)) {
        inByte = inByte - 87;
      } else if (isDigit(inByte)) {
        inByte = inByte - 48;
      }  

      Wire.beginTransmission(0x08); 	// transmit to device #8
      Wire.write(inByte);        	// sends a byte
      Wire.endTransmission();    	// stop transmitting
    }
 
  } 

  if (Wire.available()) {
    while(Wire.available()) {
      int inByte = Wire.read();
      _bufferW[byteNum++] = inByte;
      delay(100);

    }
      
 
    Serial.println(_bufferW);
  }
  byteNum = 0;


}

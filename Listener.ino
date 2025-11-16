// This is Listener1 that takes serial text and transmits it over I2C to the other Uno board. 
#include <Wire.h>
#include <LiquidCrystal_I2C.h> // Library for LCD

LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address 0x27, 16 column and 2 rows

int ledPin = 13;              // select the pin for the LED
char _bufferS[32] = "Hello!";             // for the serial text
char _bufferW[32] = "Hello!";             // for the serial text
int byteNum = 0;

int deviceAddr = 0x08;      // address of the device I want to talk to
int registerAddr = 0;       // the register I want to look at
int dataI2C = 0;            // variable for the data to send or receive


void setup() {
  Serial.begin(9600);      		// initialize a serial portt to talk:
  
  pinMode(ledPin, OUTPUT);    	// declare the ledPin as an OUTPUT:
  digitalWrite(ledPin, LOW); 	// turn the ledPin off to show error

  lcd.init(); //initialize the lcd
  lcd.backlight(); //open the backlight 
  lcd.print(_bufferS);
  Wire.begin(8); // join I2C bus (address optional for master)
  Wire.onRequest(requestEvent); // register event
}


void loop() {
  lcd.setCursor(0, 0);

  // while (Wire.available()) {
  //   digitalWrite(ledPin, HIGH);
  //   char c = Wire.read();
  //   digitalWrite(ledPin, LOW);
  //   Serial.print(c);
  //   lcd.print(c);
  //   delay(200);
  // }
}
// while (Serial.available()) {
//       int inByte = Serial.read();
//       _bufferS[byteNum++] = inByte;
      
//       if ((inByte > 64) & (inByte < 72)) {
//         inByte = inByte - 55 ;
//       } else if ((inByte > 96) & (inByte < 103)) {
//         inByte = inByte - 87;
//       } else if (isDigit(inByte)) {
//         inByte = inByte - 48;
//       }

      // Wire.beginTransmission(0x08); 	// transmit to device #8
      // Wire.write(inByte);        	// sends a byte
      // Wire.endTransmission();    	// stop transmitting
      
  // // read from port 0, send to I2C:
  // while (Serial.available()) {
  //   int inByte = Serial.read();
  //   _buffer[byteNum++] = inByte;
  //   delay(100);
  //   lcd.clear();
  //   lcd.print(inByte);
  // }
   
  // Serial.write(byteNum);

  // Wire.beginTransmission(8); 	// transmit to device #8
	// Wire.write(atoi(_buffer));        	// sends a byte
	// Wire.endTransmission();    	// stop transmitting
  // byteNum = 0;

void requestEvent() {
  digitalWrite(ledPin, HIGH);
  Wire.write("hello "); // respond with message of 6 bytes
  // as expected by master
  digitalWrite(ledPin, LOW);
}
// void loop() {
//   display.display();
//   display.setCursor(x, y);
//   display.setTextColor(cw);

//   // read from port 0, send to I2C:
//   while (Serial.available()) {
//     int inByte = Serial.read();
//     _bufferS[byteNum++] = inByte;
//     delay(100);
//     display.clearDisplay();
//     display.fillRect(x,y, 127, 63, cb); // Overwrite with black rectangle
//   }
  
//   display.setCursor(x, y);
//   display.println(_bufferS);         // display after the all chars read.
//   display.println(_bufferW);         // display after the all chars read.
//   display.display();

//   Wire.beginTransmission(8); 	// transmit to device #8
// 	Wire.write(atoi(_bufferS));        	// sends a byte
// 	Wire.endTransmission();    	// stop transmitting

//   while(Wire.available()) {
//     int inByte = Wire.read();
//     _bufferW[byteNum++] = inByte;
//     delay(100);
//     display.clearDisplay();
//     display.fillRect(x,y, 127, 63, cb); // Overwrite with black rectangle
//   }
    
//   display.setCursor(x, y);
//   display.println(_bufferS);         // display after the all chars read.
//   display.println(_bufferW);         // display after the all chars read.
//   display.display();
//   byteNum = 0;
// }
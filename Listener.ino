// This is Listener that emulates the LTC3360. It reads from the serial and sends via I2C to the other Uno board. 
#include <Wire.h>

byte SLAVE_ADDRESS = 0x09;  // address of the device I want to talk to
byte REG_ADDRESS = 0x06;    // the register that sets the Capacitor shunt voltage
int READ_LENGTH = 0x03;     // how many bytes in the message
unsigned int SHUNT_VALUE = 0x3999;   // the default value. use unsigned
char buffer[40];
int RxBytes;

void I2C_TxHandler(void)
{
  Wire.write(SHUNT_VALUE % 256);
  Wire.write(SHUNT_VALUE / 256);

  sprintf(buffer, "transmitting %02X%02X", SHUNT_VALUE % 256, SHUNT_VALUE / 256);
  Serial.print(buffer);
  Serial.println();
}

void I2C_RxHandler(int numBytes)
{
  byte nBytes = 0;
  byte buff[READ_LENGTH];
  char buffer[2];                     // needed for formatting

  Serial.println();
  Serial.println("Received ");

  while(Wire.available()) {  
    byte oneByte = Wire.read();    // Read Any Received Data
    buff[nBytes++] = oneByte; 
    sprintf(buffer, "%02X", oneByte);
    Serial.println(buffer);
  }

  if (nBytes == 3) {					// print the 2 byte integer version
      RxBytes = (buff[1]  << 8) + buff[2];
      SHUNT_VALUE = RxBytes;		// update current value 			
      Serial.println(SHUNT_VALUE, HEX);
  }  
}

void setup() {
  Serial.begin(9600);      		  // initialize a serial port to talk:  
  Wire.begin(SLAVE_ADDRESS);    // join I2C bus as a slave
  Wire.onReceive(I2C_RxHandler);// setup a handler
  Wire.onRequest(I2C_TxHandler);
}


void loop() {
  // Serial.print(RxBytes);
}

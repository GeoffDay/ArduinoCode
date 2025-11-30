// This is Talker2 that reads data from the serial port to set the Shunt voltage register of the LTC3350 Supercap management IC.
#include <Wire.h>

byte SLAVE_ADDRESS = 0x09;      // address of the device I want to talk to
byte REG_ADDRESS = 0x06;        // the register that sets the Capacitor shunt voltage
byte READ_LENGTH = 0x02;        // 16 bit return
unsigned int NEW_VALUE = 0x3FFF;         // what we want it to be idealy


void setup() {
  Serial.begin(9600);      		  // initialize a serial port to talk: 
  DisplayValues(); 
  Wire.begin();                 // join I2C bus (address optional for master)
}

void DisplayValues() {
  Serial.println("LTC3350 I2C programmer");
  Serial.print("Slave Address is ");
  Serial.println(SLAVE_ADDRESS, HEX); 
  Serial.print("Reg Address is ");
  Serial.println(REG_ADDRESS, HEX);
  Serial.print("New Value is ");
  Serial.println(NEW_VALUE, HEX);
  
  Serial.print("Current Value ");
  ReadValue();

  Serial.println("Press Shh to change Slave Address, Rhh to change Register");
  Serial.println("or Vhhhh to change Value where h is a hex digit.");
  Serial.println("Press X to examine current value, P to program the new value");
  Serial.println("or Space to display all this."); 
  Serial.println();
}


void ReadValue() {
  Wire.beginTransmission(SLAVE_ADDRESS);  
  Wire.write(REG_ADDRESS);            // set register to read
  Wire.endTransmission(false);        // false to not release the line

  Wire.requestFrom(SLAVE_ADDRESS, READ_LENGTH); // request 2 bytes from register 
  byte buff[READ_LENGTH];             // create a buffer 
  Wire.readBytes(buff, READ_LENGTH);  // read into it
  Wire.endTransmission(true);         // end this tx  

  for (int i = 0; i < READ_LENGTH; i++) {
    Serial.print(buff[i], HEX);    // print the initial 
  }

  Serial.println();
}

void ProgramValue() {
  Wire.beginTransmission(SLAVE_ADDRESS);  // start another tx
  Wire.write(REG_ADDRESS);                // set the register to change
  Wire.write(NEW_VALUE / 256);
  // Serial.print(NEW_VALUE / 256);
  Wire.write(NEW_VALUE % 256);
  // Serial.print(NEW_VALUE % 256);
  Wire.endTransmission(true);              // end this one too.
  
  delay(100);

  Serial.print("New Value ");
  ReadValue();
}

byte GetHexByte(){
  byte val = 0;

  while (Serial.available()) {       // something arrived via serial connection?
    delay(100);
    byte c= Serial.read();        // read the new character

    if (isHexadecimalDigit(c)) {
      // hex character to the 4bit equivalent number, using the ascii table indexes
      if (c >= '0' && c <= '9') c = c - '0';
      else if (c >= 'a' && c <= 'f') c = c - 'a' + 10;
      else if (c >= 'A' && c <= 'F') c = c - 'A' + 10;
     
      val = (val << 4) | (c & 0xF); // shift 4 to make space for new digit, and add the 4 bits of the new digit 
    }
  }  
  return val;
}


unsigned int GetHexInt(){
  unsigned int val = 0;

  while (Serial.available()) {       // something arrived via serial connection?
    delay(100);
    byte c= Serial.read();        // read the new character

    if (isHexadecimalDigit(c)) {
      // hex character to the 4bit equivalent number, using the ascii table indexes
      if (c >= '0' && c <= '9') c = c - '0';
      else if (c >= 'a' && c <= 'f') c = c - 'a' + 10;
      else if (c >= 'A' && c <= 'F') c = c - 'A' + 10;
      
      val = (val << 4) | (c & 0xF); // shift 4 to make space for new digit, and add the 4 bits of the new digit 
    }
  }  
  return val;
}

void loop() {
   if (Serial.available()) {}      // something arrived via serial connection?
      delay(100);
      char c= Serial.read(); // read the new character
      
      switch(c) {
        case 'S': SLAVE_ADDRESS = GetHexByte();    // set new Slave address
                  DisplayValues();   
          break;
        case 'R': REG_ADDRESS = GetHexByte();      // set new Register
                  DisplayValues();
          break;
        case 'V': NEW_VALUE = GetHexInt();       // set new Value
                  // Serial.println(NEW_VALUE);
                  // Serial.println(NEW_VALUE, HEX);
                  DisplayValues();
          break;
        case 'X': Serial.print("Current Value ");
                  ReadValue();                  // Examine current settings
          break;
        case 'P': ProgramValue();               // program new Value
          break;
        case ' ': DisplayValues();
          break;
         default: 
          break;
      }
  }

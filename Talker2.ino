// This is Talker2 used to write to the I2C port of the LTC3350 Supercap management IC.
#include <Wire.h>

byte SLAVE_ADDRESS = 0x09;          // address of the device I want to talk to
byte REG_ADDRESS = 0x06;            // the register that sets the Capacitor shunt voltage
byte READ_LENGTH = 0x02;            // 16 bit return
unsigned int NEW_VALUE = 0x3FFF;    // what we want it to be idealy
char buffer[4];                     // needed for formatting

void setup() {
  Serial.begin(9600);      		      // initialize a serial port to talk: 
  DisplayValues(); 
  Wire.begin();                     // join I2C bus (address optional for master)
}

void DisplayValues() {
  Serial.println();
  Serial.println("LTC3350 I2C programmer");
  Serial.print("Slave Address is ");
  Serial.println(SLAVE_ADDRESS, HEX); 
  Serial.print("Reg Address is ");
  Serial.println(REG_ADDRESS, HEX);
 
  sprintf(buffer, "New Value to be written is 0x%04X", NEW_VALUE);
  Serial.println(buffer);               // print the value in hex format
  
  Serial.print("Value read from LTC3350 is 0x");
  ReadValue();

  Serial.println("Press Shh to change Slave Address, Rhh to change Register");
  Serial.println("or Vhhhh to change Value where h is a hex digit.");
  Serial.println("Press X to examine current value, P to program the new value");
  Serial.println("or Space to display all this."); 
  Serial.println();
}


void ReadValue() {
  Wire.beginTransmission(SLAVE_ADDRESS);  
  Wire.write(REG_ADDRESS);                      // set register to read
  Wire.endTransmission(false);                  // false to not release the line

  Wire.requestFrom(SLAVE_ADDRESS, READ_LENGTH); // request 2 bytes from register 
  byte buff[READ_LENGTH];                       // create a buffer 

  Wire.readBytes(buff, READ_LENGTH);            // read into it
  Wire.endTransmission(true);                   // end this tx  

  sprintf(buffer, "%02X%02X", buff[1], buff[0]);
  Serial.print(buffer);                         // print the big buffer. ugly but it works. 

  Serial.println();
}

void ProgramValue() {
  Wire.beginTransmission(SLAVE_ADDRESS);  // start another tx
  Wire.write(REG_ADDRESS);                // set the register to change
  Wire.write(NEW_VALUE & 0x00FF);         // lsb first. Lets make it obvious 
  Wire.write((NEW_VALUE >> 8) & 0x00FF);  // that we are sending LSB first.
  Wire.endTransmission(true);             // end this one too.
  
  delay(100);

  Serial.print("New Value ");
  ReadValue();
}


unsigned int GetHexInt(){
  unsigned int val = 0;

  while (Serial.available()) {      // something arrived via serial connection?
    delay(100);
    byte c= Serial.read();          // read the new character

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
        case 'S': SLAVE_ADDRESS = GetHexInt();    // set new Slave address                
          break;
        case 'R': REG_ADDRESS = GetHexInt();      // set new Register                  
          break;
        case 'V': NEW_VALUE = GetHexInt();       // set new Value                  
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


// byte GetHexByte(){
//   byte val = 0;

//   while (Serial.available()) {       // something arrived via serial connection?
//     delay(100);
//     byte c= Serial.read();           // read the new character

//     if (isHexadecimalDigit(c)) {
//       // hex character to the 4bit equivalent number, using the ascii table indexes
//       if (c >= '0' && c <= '9') c = c - '0';
//       else if (c >= 'a' && c <= 'f') c = c - 'a' + 10;
//       else if (c >= 'A' && c <= 'F') c = c - 'A' + 10;
     
//       val = (val << 4) | (c & 0xF); // shift 4 to make space for new digit, and add the 4 bits of the new digit 
//     }
//   }  
//   return val;
// }

#include <Wire.h>

void setup() {
  Serial.begin(9600);
  Wire.begin(); // join i2c bus
}

void loop() {
  unsigned int data;
  // Stage 5 Pre-Amp.
  // Reference Value DAC has address 0001110, which is 14 in binary
  // ON/OFF DAC has address 0001100, which is 12 in binary

  Wire.beginTransmission(14); // transmit to device #14 0001110
  Wire.write(byte(0)); // command byte 00000000 - write command (DAC A)
  Wire.write(byte(255)); // most significant byte 11111111
  Wire.write(byte(0)); // least significant byte 00000000
  Wire.endTransmission(); // stop transmitting
  
  Wire.beginTransmission(12); // transmit to device #12 0001100
  Wire.write(byte(33)); // commabd byte 00100001 - power up/down command (DAC B)
  Wire.write(byte(255)); // byte 11111111 (don't care)
  Wire.write(byte(49)); // byte 00110001 00 don't care, 11 high impedance power down mode, 00 don't care, 01 apply to DAC B
  Wire.endTransmission(); // stop transmitting

  //ADC 1 - output
  Wire.beginTransmission(20); //transmit to device #20 0010100
  //add the writing commands to select the ADC channels
  Wire.requestFrom(20,2); //request 2 data bytes from ADC (16 bit ADC)
  if (Wire.available() == 1)
  {
     data1 = Wire.read(); //reads the byte transmitted by the ADC
  }
  Serial.print("Digital value of analog input for ADC 1: ");
  Serial.println(data1);
  //ADC 2 - preamp
  Wire.beginTransmission(53); //transmit to device #20 0110101
  //add the writing commands to select the ADC channels
  Wire.requestFrom(53,2); //request 2 data bytes from ADC
  if (Wire.available() == 1)
  {
     data2 = Wire.read(); //reads the byte transmitted by the ADC
  }
  Serial.print("Digital value of analog input for ADC 2: ");
  Serial.println(data2);
 
  Wire.endTransmission(); // stop transmitting
}

#include <Wire.h>

const int buttonPin = 2;    // the number of the pushbutton pin
const int ledPin = 13;      // the number of the LED pin

// Variables will change:
int ledState = LOW;
int buttonState;
int lastButtonState = LOW;
bool send_IIC = 0;

unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;  

byte address = 0b0010100; //<---- Adress do ADC que queremos comunicar
byte command = 0b10110000; //<---- Comando que queremos enviar para iniciar conversão
int address_int = address;

void setup() {
  Serial.begin(9600);
  Wire.begin(); // join i2c bus

  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);

  // set initial LED state
  digitalWrite(ledPin, ledState);
}

void loop() {

//----------BUTTON PRESS STUFF--------------------//
  int reading = digitalRead(buttonPin);

  if (reading != lastButtonState) {
  // reset the debouncing timer
  lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;

      // only toggle the LED if the new button state is HIGH
      if (buttonState == HIGH) {
        ledState = !ledState;
        send_IIC= !send_IIC ;
      }
    }
  }

  digitalWrite(ledPin, ledState);
  lastButtonState = reading;
//----------BUTTON PRESS STUFF--------------------//
  if(send_IIC){
    
    unsigned int data;
    unsigned long combined = 0;
    byte byte1 = 0b0;
    byte byte0 =0b0;
    
    Wire.beginTransmission(address_int); 
    Wire.write(command);
    Wire.endTransmission();

    Wire.requestFrom(address_int, 2 ,true);
    
    while (Wire.available()) { // peripheral may send less than requested
      
      byte1 = Wire.read(); // receive a byte
      byte0 = Wire.read(); 
      // print the byte
    }
    combined = (byte1 << 8) | (byte0); //<--- Aqui temos o valor em inteiro dos dois bytes lidos
    
    Serial.println(combined);

    Wire.endTransmission();
    

    // Wire.beginTransmission(12); // transmit to device #12 0001100
    // Wire.write(byte(33)); // commabd byte 00100001 - power up/down command (DAC B)
    // Wire.write(byte(255)); // byte 11111111 (don't care)
    // Wire.write(byte(49)); // byte 00110001 00 don't care, 11 high impedance power down mode, 00 don't care, 01 apply to DAC B
    // Wire.endTransmission(); // stop transmitting
    // //ADC 1 - output
    // Wire.beginTransmission(20); //transmit to device #20 0010100
    // //add the writing commands to select the ADC channels
    // Wire.requestFrom(20,2); //request 2 data bytes from ADC (16 bit ADC)
    // if (Wire.available() == 1)
    // {
    //    data1 = Wire.read(); //reads the byte transmitted by the ADC
    // }
    // Serial.print("Digital value of analog input for ADC 1: ");
    // Serial.println(data1);
    // //ADC 2 - preamp
    // Wire.beginTransmission(53); //transmit to device #20 0110101
    // //add the writing commands to select the ADC channels
    // Wire.requestFrom(53,2); //request 2 data bytes from ADC
    // if (Wire.available() == 1)
    // {
    //    data2 = Wire.read(); //reads the byte transmitted by the ADC
    // }
    // Serial.print("Digital value of analog input for ADC 2: ");
    // Serial.println(data2);
   
    // Wire.endTransmission(); // stop transmitting
    
     send_IIC = 0;
  }
}

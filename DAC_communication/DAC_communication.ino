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
    
    byte address = 0b0001111; //<---- Address da DAC que queremos comunicar
    int address_int = address;
    
    unsigned int data;
    unsigned long combined = 0;
    byte status_IIC = 0b0;

    //MÉTODO 1
    Wire.beginTransmission(address_int); 
    Wire.write(0b01111111);
    Wire.write(0b00000000);
    Wire.write(0b00000011); //<--setup do LDAC em ambas as DAC's internas
    status_IIC=Wire.endTransmission();
    Serial.println(status_IIC); //<-- Um print para ver se o ack foi recebido, se for 0 está tudo bem
    
    Wire.beginTransmission(address_int); 

    Wire.write(0b00000000);
    Wire.write(0b00111111);
    Wire.write(0b11111111); //<--0.312485V NA SAÍDA DA DAC DE REFERÊNCIA
    status_IIC=Wire.endTransmission();
    Serial.println(status_IIC);
    
    //MÉTODO 2
    /*
    Wire.beginTransmission(address_int);
    Wire.write(0b00011000);
    Wire.write(0b00111111);
    Wire.write(0b11111111);
    status_IIC=Wire.endTransmission();
    Serial.println(status_IIC);
    */
    
    //MÉTODO 3
    /*Wire.beginTransmission(address_int); 
    Wire.write(0b000000000);
    Wire.write(0b00111111);
    Wire.write(0b11111111); //<--setup do LDAC em ambas as DAC's internas
    status_IIC=Wire.endTransmission();
    Serial.println(status_IIC); //<-- Um print para ver se o ack foi recebido, se for 0 está tudo bem
    
    Wire.beginTransmission(address_int); 
    Wire.write(0b00001000);
    Wire.write(0b00111111);
    Wire.write(0b11111111); //<--setup do LDAC em ambas as DAC's internas
    status_IIC=Wire.endTransmission();
    Serial.println(status_IIC); //<-- Um print para ver se o ack foi recebido, se for 0 está tudo bem
    */
     send_IIC = 0;
  }
}

#include <Wire.h>

const int buttonPin = 2;    // the number of the pushbutton pin
const int ledPin = 13;      // the number of the LED pin

// // Variables will change:
// int ledState = LOW;
// int buttonState;
// int lastButtonState = LOW;
bool send_IIC = 1;

// ///----------
// const int high_pin = 3;
// int high_state = HIGH;


// unsigned long lastDebounceTime = 0;
// unsigned long debounceDelay = 50;  

int i=0;

void setup() {
  Serial.begin(9600);
  Wire.begin(); // join i2c bus

//   pinMode(buttonPin, INPUT);
//   pinMode(ledPin, OUTPUT);

//   // set initial LED state
//   digitalWrite(ledPin, ledState);
// ///-------
//   pinMode(high_pin, OUTPUT);
//   digitalWrite(high_pin, high_state);
}

void loop() {

    byte byte2 = 0b0;
    byte byte1 = 0b0;
    byte byte0 =0b1;

//----------BUTTON PRESS STUFF--------------------//
  // int reading = digitalRead(buttonPin);

  // if (reading != lastButtonState) {
  // // reset the debouncing timer
  // lastDebounceTime = millis();
  // }

  // if ((millis() - lastDebounceTime) > debounceDelay) {

  //   // if the button state has changed:
  //   if (reading != buttonState) {
  //     buttonState = reading;

  //     // only toggle the LED if the new button state is HIGH
  //     if (buttonState == HIGH) {
  //       ledState = !ledState;
  //       send_IIC= !send_IIC ;
  //       Serial.println("Sent");

  //     }
  //   }
  // }
  delay(1000);  // digitalWrite(ledPin, ledState);
  // lastButtonState = reading;
//----------BUTTON PRESS STUFF--------------------//
  if(send_IIC){
    
    byte address1 = 0b0001111; //<---- Verde-ground
    byte address2 = 0b0001110; //<---- Azul-aberto
    byte address3 = 0b0001100; //<---- Vermelho-+5

    int address_int[3] = {address1,address2,address3};
    
    // unsigned int data;
    // unsigned long combined = 0;
    byte status_IIC = 0b0;

    //MÉTODO 1
    /*Wire1.beginTransmission(address_int); 
    Wire1.write(0b01111111);
    Wire1.write(0b00000000);
    .write(0b00000111); //<--setup do LDAC em ambas as DAC's internas
    status_IIC=Wire.endTransmission();
    Serial.print("Código de erro setup:");
    Serial.println(status_IIC); //<-- Um print para ver se o ack foi recebido, se for 0 está tudo bem
    
    Wire.beginTransmission(address_int); 

    Wire.write(0b00000000);
    Wire.write(0b00111111);
    Wire.write(0b11111111); //<--0.312485V NA SAÍDA DA DAC DE REFERÊNCIA
    status_IIC=Wire.endTransmission();
    Serial.print("Código de erro aplicar V:");
    Serial.println(status_IIC);
    */
    
    Serial.println("Configure internal reference");

    Wire.beginTransmission(address_int[i]);

    Wire.write(0b00111111); //<-- para ambas
    //Wire.write(0b00011000); //<-- para DAC A
    //Wire.write(0b01011001); //<-- para DAC B
    Wire.write(0x00);

    Wire.write(0x01);

    status_IIC=Wire.endTransmission();

    Serial.print("Código de erro");
    Serial.println(status_IIC);
    
    
    //MÉTODO 2
    Serial.print("Comunicando com o endereço: ");
    Serial.print(address_int[i]);
    Serial.print("-->");
    Serial.println(i);
    Wire.beginTransmission(address_int[i]);
    Wire.write(0b00011111); //<-- para ambas
    //Wire.write(0b00011000); //<-- para DAC A
    //Wire.write(0b01011001); //<-- para DAC B
    //Wire.write(0b00111110);
    //Wire.write(0b01111111);
    //Wire.write(0b01111100);
    //Wire.write(0b11111111);
    Wire.write(0xFF);
    Wire.write(0xFF);
    status_IIC=Wire.endTransmission();
    Serial.print("Código de erro");
    Serial.println(status_IIC);
    delay(100);  // digitalWrite(ledPin, ledState);

    Wire.requestFrom(address_int[i], 3 ,true);
    
    while (Wire.available()) { // peripheral may send less than requested
      //Serial.println("Recebeu");

      byte2 = Wire.read();
      byte1 = Wire.read(); // receive a byte
      byte0 = Wire.read(); 
       //print the byte
    }
    Serial.println("Read back ");
    //Serial.println(byte4);
    //Serial.println(byte3);
    Serial.println(byte2, HEX);
    Serial.println(byte1, HEX);
    Serial.println(byte0, HEX);

    /*
    Wire.beginTransmission(address_int[0]);
    //Wire.write(0b01011111); //<-- para ambas
    //Wire.write(0b00011000); //<-- para DAC A
    Wire.write(0b01011001); //<-- para DAC B
    //Wire.write(0b00111110);
    //Wire.write(0b01111111);
    //Wire.write(0b01111100);
    //Wire.write(0b11111111);
    Wire.write(0b11111111);
    Wire.write(0b11111111);
    status_IIC=Wire.endTransmission();
    Serial.print("Código de erro");
    Serial.println(status_IIC);
    */
    
    /*Wire.beginTransmission(address_int[i]);
    //Wire.write(0b01011111); //<-- para ambas
    Wire.write(0b01011000); //<-- para DAC A
    //Wire.write(0b01011001); //<-- para DAC B
    //Wire.write(0b00111110);
    //Wire.write(0b01111111);
    //Wire.write(0b01111100);
    //Wire.write(0b11111111);
    Wire.write(0b11111111);
    Wire.write(0b11111111);
    status_IIC=Wire.endTransmission();
    Serial.print("Código de erro");
    Serial.println(status_IIC);*/
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
     //send_IIC = 0;

      if(i<2){
       i=i+1;
       }
       else{
         i=0;
         }
     
  }
}

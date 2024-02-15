#include <Wire.h>
#include "I2C_ClearBus.h"


const int buttonPin = 2;    // the number of the pushbutton pin
const int ledPin = 13;      // the number of the LED pin
const int resetPin = 12;



// Variables will change:
int ledState = LOW;
int buttonState;
int lastButtonState = LOW;
bool send_IIC = 1;
bool show_read = 0;
int result = 0;

unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;  

byte address = 0b0110101; //<---- Adress do ADC que queremos comunicar pré-amp
//byte address = 0b0010100;
byte command = 0b10111101; //<---- Comando que queremos enviar para iniciar conversão
int address_int = address;
int command_int=command;
int nr_canal=0;
int max_value= 65536;

int error_count = 0;

byte r_command[] = {0b10110000, 0b10111000 , 0b10110001, 0b10111001, 
                    0b10110010 , 0b10111010, 0b10110011 , 0b10111011 , 
                    0b10110100 , 0b10111100 , 0b10110101 , 0b10111101 , 
                    0b10110110 , 0b10111110 , 0b10110111 , 0b10111111};


void setup() {

  digitalWrite(resetPin, LOW);
  delay(200);

  Serial.begin(9600);
  Wire1.begin(); // join i2c bus

  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(resetPin, OUTPUT);     


  // set initial LED state
  digitalWrite(ledPin, ledState);

  Serial.println("reset");//print reset to know the program has been reset and 
  //the setup function happened
  delay(200);
  


}
//void(* resetFunc) (void) = 0; //declare reset function @ address 0


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
        Serial.println("Sent");

      }
    }
  }
  //Serial.println("loop1");

  digitalWrite(ledPin, ledState);
  lastButtonState = reading;
//----------BUTTON PRESS STUFF--------------------//
  while(1){
    
    unsigned int data;
    byte ADC_read_Status = 0x00;
    unsigned long combined = 0;
    byte byte4 = 0b0;
    byte byte3 = 0b0;
    byte byte2 = 0b0;
    byte byte1 = 0b0;
    byte byte0 =0b1;
    byte status_IIC = 0b0;
    byte status_write = 0b0;



    delay(250);
    
    Serial.println(address_int);
    Wire1.beginTransmission(address_int); 
    status_write=Wire1.write(r_command[nr_canal]);
    status_IIC=Wire1.endTransmission();

    Serial.print("Canal ");
    Serial.println(nr_canal);
    
    Serial.print("N de bytes escritos: ");
    Serial.println(status_write);
    Serial.print("Codigo de erro IIC: ");
    Serial.println(status_IIC);

    if(status_IIC!=0){
      error_count = error_count +1;
      //  Serial.println("resetting");
      //  delay(1000);
      //  digitalWrite(resetPin, LOW);

    }
    

    delay(500);
    
    Wire1.requestFrom(address_int, 3 ,true);
    
    while (Wire1.available()) { // peripheral may send less than requested
      Serial.println("Recebeu");
      //byte4 = Wire1.read();
      //byte3 = Wire1.read();
      byte2 = Wire1.read();
      byte1 = Wire1.read(); // receive a byte
      byte0 = Wire1.read(); 
      show_read=1;
       //print the byte
    }
   
    //if(show_read){

    Serial.print("Valor lido no canal ----------->");
    Serial.print(nr_canal);
    Serial.println(":");
    //Serial.println(byte4);
    //Serial.println(byte3);
    Serial.println(byte2);
    Serial.println(byte1);
    Serial.println(byte0);

    ADC_read_Status = (byte2 >> 6);

    combined = ((byte2 & 0x3F) << 10) | (byte1 << 2) | ((byte0 & 0xC0) >> 6);
    
    Serial.print("Read status HEX: ");
    Serial.print(ADC_read_Status, HEX);
    Serial.print("<++++++++");
    Serial.println("");
    Serial.print("Measurement HEX: ");
    Serial.println(combined, HEX);
    Serial.print("Measurement int:  ");
    Serial.print(combined);
    Serial.print("<++++++++");
    Serial.println("");
    Serial.print("Read status HEX: ");
    Serial.print(ADC_read_Status, HEX);
    Serial.print("<++++++++");
    Serial.println("");
    Serial.print("Measurement HEX: ");
    Serial.println(combined, HEX);
    Serial.print("Measurement int:  ");
    Serial.print(combined);
    Serial.print("<++++++++");
    Serial.println("");
    Serial.print("Error Count = ");
    Serial.println(error_count);






    //Serial.println("Valor da conversão (x/32767)*3 V :");

    //Serial.println(combined);

    //show_read=0;
    //}

    //status_IIC=Wire1.endTransmission();
    //Serial.print("Código de erro:");
    //Serial.println(status_IIC);

    // Wire1.beginTransmission(12); // transmit to device #12 0001100
    // Wire1.write(byte(33)); // commabd byte 00100001 - power up/down command (DAC B)
    // Wire1.write(byte(255)); // byte 11111111 (don't care)
    // Wire1.write(byte(49)); // byte 00110001 00 don't care, 11 high impedance power down mode, 00 don't care, 01 apply to DAC B
    // Wire1.endTransmission(); // stop transmitting
    // //ADC 1 - output
    // Wire1.beginTransmission(20); //transmit to device #20 0010100
    // //add the writing commands to select the ADC channels
    // Wire1.requestFrom(20,2); //request 2 data bytes from ADC (16 bit ADC)
    // if (Wire1.available() == 1)
    // {
    //    data1 = Wire1.read(); //reads the byte transmitted by the ADC
    // }
    // Serial.print("Digital value of analog input for ADC 1: ");
    // Serial.println(data1);
    // //ADC 2 - preamp
    // Wire1.beginTransmission(53); //transmit to device #20 0110101
    // //add the writing commands to select the ADC channels
    // Wire1.requestFrom(53,2); //request 2 data bytes from ADC
    // if (Wire1.available() == 1)
    // {
    //    data2 = Wire1.read(); //reads the byte transmitted by the ADC
    // }
    // Serial.print("Digital value of analog input for ADC 2: ");
    // Serial.println(data2);
   
    // Wire1.endTransmission(); // stop transmitting
     //Serial.print("loop");

    if(nr_canal<15){
      nr_canal=nr_canal+1;
    }
    else
    {
      nr_canal=0;
    }
     
    send_IIC = 1;
  }
}

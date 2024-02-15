#include <Wire.h>
#include "I2C_ClearBus.h"

//const int buttonPin = 2;    // the number of the pushbutton pin
//const int ledPin = 13;      // the number of the LED pin
int resetPin = 12;
const int driver_enable = 3;


// // Variables will change:
// int ledState = LOW;
// int buttonState;
// int lastButtonState = LOW;
bool send_IIC = 0;

byte r_command[] = {0b10110000, 0b10111000 , 0b10110001, 0b10111001, 
                    0b10110010 , 0b10111010, 0b10110011 , 0b10111011 , 
                    0b10110100 , 0b10111100 , 0b10110101 , 0b10111101 , 
                    0b10110110 , 0b10111110 , 0b10110111 , 0b10111111};
                    
// //*Valores de tensão testados : 2.5; 2.0; 1.75; 1.25; 0.625; 0.312; 0.156; 0.078 */
// byte r_DAC_data_H[] = {0xFF , 0xCC , 0xB3, 0x80, 0x40, 0x20, 0x10, 0x08};
// byte r_DAC_data_L[] = {0xFF , 0xCC , 0x32, 0x00, 0x00, 0x00, 0x00, 0x00};
//*Valores de tensão testados : 1.7; 1.6 1.5 1.4 1.3 1.2 1.1 1 0.9 0.8 0.7 0.6 0.5 0.4 0.3 0.2 0.1 */
byte r_DAC_data_H[] = {0x5A, 0x55, 0x50, 0x45, 0x40, 0x3A, 0x35, 0x30, 0x2A, 0x25, 0x20, 0x1A, 0x15, 0x10, 0x0A, 0x05};
byte r_DAC_data_L[] = {0xD6, 0x7E, 0x26, 0x76, 0x1F, 0xC7, 0x6F, 0x17, 0xBF, 0x67, 0x0F, 0xB7, 0x60, 0x08, 0xB0, 0x58};

// ///----------
// const int high_pin = 3;
// int high_state = HIGH;
byte address_ADC_pre = 0b0110101; //<---- Adress do ADC que queremos comunicar
byte address_ADC_pos = 0b0010100;
//byte address = 0b0010100;
//<---- Comando que queremos enviar para iniciar conversão
int address_int_ADC[2] = {address_ADC_pre,address_ADC_pos};

byte address1 = 0b0001111; //<---- Verde-ground
byte address2 = 0b0001110; //<---- Azul-aberto
byte address3 = 0b0001100; //<---- Vermelho-+5

int address_int[3] = {address1,address2,address3};
// unsigned long lastDebounceTime = 0;
// unsigned long debounceDelay = 50;  

int i=0;
int j =0;
int l=0;
int k=0;

void setup() {

  Serial.begin(9600);
  Wire.begin(); // join i2c bus
  Wire1.begin(); // join i2c bus
  //pinMode(resetPin, OUTPUT);     
  pinMode(driver_enable, OUTPUT);
  digitalWrite(driver_enable, LOW);



//   pinMode(buttonPin, INPUT);
//   pinMode(ledPin, OUTPUT);

//   // set initial LED state
//   digitalWrite(ledPin, ledState);
// ///-------
//   pinMode(high_pin, OUTPUT);
//   digitalWrite(high_pin, high_state);
}

void loop() {
    
    // unsigned int data;
    digitalWrite(driver_enable, HIGH);

    unsigned int data;
    byte byte4 = 0b0;
    byte byte3 = 0b0;
    byte byte2 = 0b0;
    byte byte1 = 0b0;
    byte byte0 =0b1;
    byte status_IIC = 0b0;
    byte status_write = 0b0;

    byte ADC_read_Status = 0x00;
    unsigned long combined = 0;
    

    //* DAC setup internal reference  */

    while (k<3)
    {

      Serial.println("Configure internal reference");

      Wire.beginTransmission(address_int[k]);

      Wire.write(0b00111111); //<-- para ambas
      //Wire.write(0b00011000); //<-- para DAC A
      //Wire.write(0b01011001); //<-- para DAC B
      Wire.write(0x00);

      Wire.write(0x01);

      status_IIC=Wire.endTransmission();

      //Serial.print("Codigo de erro");
      //Serial.println(status_IIC);

      delay(1500);
      
      //* Writte data into the DAC's  */

      // Serial.println("Writting DAC ");
      // Serial.print("Data H: ");
      // Serial.println(r_DAC_data_H[l], HEX);
      // Serial.print("Data L:");
      // Serial.println(r_DAC_data_L[l],HEX);
      // Serial.print("Iteracao: ");
      // Serial.print(l);
      // Serial.print("<------------------------");
      // Serial.println(" ");

      Serial.print("Writting DAC: ");
      Serial.println(k);

      Wire.beginTransmission(address_int[k]);

      Wire.write(0b00011111); //<-- para ambas
      //Wire.write(0b00011000); //<-- para DAC A
      //Wire.write(0b01011001); //<-- para DAC B
      // Wire.write(r_DAC_data_H[l]);

      // Wire.write(r_DAC_data_L[l]);

      Wire.write(0x00);

      Wire.write(0x00);


      status_IIC=Wire.endTransmission();
      Serial.print("Código de erro");
      Serial.println(status_IIC);

      delay(150);
      
      Wire.requestFrom(address_int[k], 3 ,true);
      
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
      delay(1500);

      k=k+1;
  }
    
    while (1)
    {
      

    //* Measure both channels with ADC  */
    while(i<1){

      if(i==0){
        Serial.println("-------------------------------------------------------------");
        Serial.println("ADC pre AMP ");
        Serial.println("-------------------------------------------------------------");
      }
      else
      {
        Serial.println("-------------------------------------------------------------");
        Serial.println("ADC pos AMP ");
        Serial.println("-------------------------------------------------------------");
      }

      // Wire1.beginTransmission(address_int_ADC[i]); 
      // status_IIC=Wire1.endTransmission();
            
      //Serial.print("N de bytes escritos: ");
      //Serial.println(status_write);
      //Serial.print("Codigo de erro IIC: ");
      //Serial.println(status_IIC);
      //delay(150);
      while(j<4){

        Wire1.beginTransmission(address_int_ADC[i]); 
        status_write=Wire1.write(r_command[j]);

        status_IIC=Wire1.endTransmission();
        
        //Serial.print("N de bytes escritos: ");
        //Serial.println(status_write);
        Serial.print("Codigo de erro IIC: ");
        Serial.println(status_IIC);

        //delay(1500);

        Serial.print("Valor lido no canal ");
        Serial.print(j);
        

        Serial.println(":");


        delay(150);

    
        combined=read_ADC(r_command[j], address_int_ADC[i]);


        // delay(150);


        // combined=read_ADC(r_command[j], address_int_ADC[i]);

      delay(1500);
      j=j+1;

      }
      j=0;
      i=i+1;
      //delay(1500);

    }
    i=0;

    }
    //l=l+1;
    //}
}


int read_ADC(int command,int address){


    byte byte2 = 0b0;
    byte byte1 = 0b0;
    byte byte0 =0b1;

    byte ADC_read_Status = 0x00;
    unsigned long combined = 0;

   Wire1.requestFrom(address_int_ADC[i], 3 ,true);
    
      while (Wire1.available()) { // peripheral may send less than requested
        //Serial.println("Recebeu");
        //byte4 = Wire1.read();
        //byte3 = Wire1.read();
        byte2 = Wire1.read();
        byte1 = Wire1.read(); // receive a byte
        byte0 = Wire1.read(); 
      
       //print the byte
       }

      ADC_read_Status = (byte2 >> 6);

      combined = ((byte2 & 0x3F) << 10) | (byte1 << 2) | ((byte0 & 0xC0) >> 6);

        Serial.print("Read status HEX: ");
        Serial.print(ADC_read_Status, HEX);
        Serial.println("");
        Serial.print("Measurement HEX: ");
        Serial.println(combined, HEX);
        Serial.print("Measurement int:  ");
        Serial.println(combined);
        Serial.print("------------------------");
        Serial.println("");

      return combined;


}

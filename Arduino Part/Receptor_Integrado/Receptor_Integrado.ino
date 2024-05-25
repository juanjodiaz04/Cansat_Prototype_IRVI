   #include <Wire.h>
#include <SPI.h>
#include <nRF24L01.h>     
#include <RF24.h>


//create an RF24 object
RF24 myRadio (4, 5);
byte addresses[][6] = {"1Node"};

struct Pack {
  float data1;
  float data2;
  float data3;
  float data4;
  float data5;
  float data6;
  float data7;
  float data8;
} packet;

void setup(void)
{
  Serial.begin(115200); 
  myRadio.begin();
  myRadio.setChannel(108);
  myRadio.setPALevel(RF24_PA_MIN);
  myRadio.openReadingPipe(1, addresses[0]);
  myRadio.startListening();
  
Serial.println("Temp   Hum  Press   Alt    ax    ay   az  F_Press");   
}

 void loop() {
  if ( myRadio.available())   
  {
    myRadio.read( &packet, sizeof(packet) );
    Serial.print(packet.data1);
    Serial.print(";");
    Serial.print(packet.data2);
    Serial.print(";");
    Serial.print(packet.data3);
    Serial.print(";");
    Serial.print(packet.data4);
    Serial.print(";");
    Serial.print(packet.data5);
    Serial.print(";");
    Serial.print(packet.data6);
    Serial.print(";");
    Serial.print(packet.data7);
    Serial.println();
  }
}

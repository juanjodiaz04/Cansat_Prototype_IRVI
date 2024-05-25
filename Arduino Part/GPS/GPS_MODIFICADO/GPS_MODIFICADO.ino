 #include <TinyGPS++.h>

TinyGPSPlus gps;
#include <SoftwareSerial.h>
static const int RXPin = 4, TXPin = 3;
SoftwareSerial ss(RXPin, TXPin);
static const uint32_t GPSBaud = 9600;

void setup(){
Serial.begin(57600); // connect serial
ss.begin(GPSBaud);


}
 
void loop(){
    
    while(ss.available()){ // check for gps data
    if(gps.encode(ss.read()))// encode gps data
    { 
    gps.location; // get latitude and longitude

    Serial.print("Position: ");
    
    //Latitude
    Serial.print("Latitude: ");
    Serial.print(gps.location.lat(),6);
    
    Serial.print(",");
    
    //Longitude
    Serial.print("Longitude: ");
    Serial.println(gps.location.lng(),6); 
    delay(2000);
   }
  }
} 

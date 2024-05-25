#include <DHT_U.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <SFE_BMP180.h>
#include <Adafruit_L3GD20.h>
#include <Adafruit_L3GD20_U.h>
#include <LSM303.h>

#define DHTPIN 7
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
SFE_BMP180 bmp180;
LSM303 compass;

float Po = 1023.25;
float gama = (0.061 / 1000);
float ax = 0;
float ay = 0;
float az = 0;
float pressure;

//create an RF24 object

RF24 myRadio (9, 10);  // CE, CSN  
//address through which two modules communicate.
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

void setup()
{
  Serial.begin(115200);
  if (bmp180.begin())
    Serial.println("BMP180 init success");
  else
  {
    // Oops, something went wrong, this is usually a connection problem,
    // see the comments at the top of this sketch for the proper connections.

    Serial.println("BMP180 init fail (disconnected?)\n\n");
    while(1); // Pause forever.
  }
  compass.init();
  compass.enableDefault();;
  dht.begin();
  bmp180.begin();
  myRadio.begin();
  myRadio.setChannel(108);
  myRadio.setPALevel(RF24_PA_MIN);
  myRadio.openWritingPipe(addresses[0]);
  pressure = getPressure();
 Serial.println("Temp   Hum  Press   Alt    ax    ay   az  F_Press"); 
  
}

void loop()
{
    double T,P,A; 
    compass.read();
    P = getPressure();
    T = getTemp();
    A = bmp180.altitude(P, Po);
    ax = compass.a.x * (gama); //Aceleracion con conversión
    ay = compass.a.y * (gama);
    az = compass.a.z * (gama);

    float raw = getPressure(); // replace with your method
    pressure = (7 * pressure + raw) / 8;  // or  15*  /16    dividing by a power of 2 works a bit faster.
  
    packet.data1 = T;
    packet.data2 = dht.readHumidity();
    packet.data3 = P;
    packet.data4 = A; 
    packet.data5 = ax;
    packet.data6 = ay;
    packet.data7 = az;
    packet.data8 = pressure;
  
 
    char text [80];
    sprintf(text,"%d,%d,%d,%d,%d,%d,%d,%d\n", packet.data1, packet.data2, packet.data3, packet.data4, packet.data5, packet.data6, packet.data7, packet.data8);   
    
  Serial.println(text);
  delay(1000);
}

double getPressure()
{
  char status;
  double T,P,p0,a;

  // You must first get a temperature measurement to perform a pressure reading.
  
  // Start a temperature measurement:
  // If request is successful, the number of ms to wait is returned.
  // If request is unsuccessful, 0 is returned.

  status = bmp180.startTemperature();
  if (status != 0)
  {
    // Wait for the measurement to complete:

    delay(status);

    // Retrieve the completed temperature measurement:
    // Note that the measurement is stored in the variable T.
    // Use '&T' to provide the address of T to the function.
    // Function returns 1 if successful, 0 if failure.

    status = bmp180.getTemperature(T);
    if (status != 0)
    {
      // Start a pressure measurement:
      // The parameter is the oversampling setting, from 0 to 3 (highest res, longest wait).
      // If request is successful, the number of ms to wait is returned.
      // If request is unsuccessful, 0 is returned.

      status = bmp180.startPressure(3);
      if (status != 0)
      {
        // Wait for the measurement to complete:
        delay(status);

        // Retrieve the completed pressure measurement:
        // Note that the measurement is stored in the variable P.
        // Use '&P' to provide the address of P.
        // Note also that the function requires the previous temperature measurement (T).
        // (If temperature is stable, you can do one temperature measurement for a number of pressure measurements.)
        // Function returns 1 if successful, 0 if failure.

        status = bmp180.getPressure(P,T);
        if (status != 0)
        {
          return(P);
        }
        else Serial.println("error retrieving pressure measurement\n");
      }
      else Serial.println("error starting pressure measurement\n");
    }
    else Serial.println("error retrieving temperature measurement\n");
  }
  else Serial.println("error starting temperature measurement\n");
}

double getTemp()
{
  char status;
  double T;
  
  status = bmp180.startTemperature();
  if (status != 0)
  {
    // Wait for the measurement to complete:

    delay(status);
    
    status = bmp180.getTemperature(T);
    if (status != 0)
    {
     return(T);
        }
        else Serial.println("error retrieving temperature measurement\n");
      }
  else Serial.println("error starting temperature measurement\n");
}

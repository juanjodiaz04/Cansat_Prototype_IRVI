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
float H = 0;
//float pitch = 0;
//float yaw = 0;
//float  q0 = 0;
//float  q1 = 0;
//float  q2 = 0;
//float  q3 = 0;

void setup()
{
  Serial.begin(9600);
  bmp180.begin(); 
  compass.init();
  compass.enableDefault();;
  dht.begin();
  bmp180.begin();
  pressure = getPressure();
  
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
    H = dht.readHumidity();

    float raw = getPressure(); // replace with your method
    pressure = (7 * pressure + raw) / 8;  // or  15*  /16    dividing by a power of 2 works a bit faster.
  

   
    Serial.print(T);
    Serial.print(",");
    Serial.print(H);
    Serial.print(",");
    Serial.print(pressure);
    Serial.print(",");
    Serial.print(A);
    Serial.print(",");
    Serial.print(ax);
    Serial.print(",");
    Serial.print(ay);
    Serial.print(",");
    Serial.print(az);
    Serial.println();
    
    
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

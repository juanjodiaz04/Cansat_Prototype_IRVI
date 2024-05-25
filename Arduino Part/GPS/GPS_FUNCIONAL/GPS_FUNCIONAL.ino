#include <SoftwareSerial.h>
#include <TinyGPS.h>

SoftwareSerial serial1(6, 7); // TX, RX
TinyGPS gps1;

void setup() {
   serial1.begin(9600);
   Serial.begin(9600);

   Serial.println("Esperando señal de los satelites...");
}

void loop() {
  bool recibido = false;
  static unsigned long delayPrint;

  while (serial1.available()) {
     char cIn = serial1.read();
     recibido = (gps1.encode(cIn) || recibido);  
  }

  if ( (recibido) && ((millis() - delayPrint) > 1000) ) {  //Se muestra solo después de recibir la primera señal. Después de la primera señal, se muestra cada segundo.
     delayPrint = millis();
     
     //Serial.println("----------------------------------------");
     
     //Latitude e Longitude
     //long latitude, longitude; 
     float latitude, longitude; //Las variables pueden ser flotantes, por lo que no necesita hacer ningún cálculo.
     
     unsigned long idadeInfo;
     gps1.f_get_position(&latitude, &longitude, &idadeInfo);   //El método f_get_position es más adecuado para devolver las coordenadas en variables flotantes, por lo que no tiene que hacer ningún cálculo

     if (latitude != TinyGPS::GPS_INVALID_F_ANGLE) {
        Serial.print("Latitud: ");
        Serial.println(latitude, 6);  //Mostra a latitude com a precisão de 6 dígitos decimais
     }

     if (longitude != TinyGPS::GPS_INVALID_F_ANGLE) {
        Serial.print("Longitud: ");
        Serial.println(longitude, 6);  //Mostra a longitude com a precisão de 6 dígitos decimais
     }

     /*if ( (latitude != TinyGPS::GPS_INVALID_F_ANGLE) && (longitude != TinyGPS::GPS_INVALID_F_ANGLE) ) {
        Serial.print("Link para Google Maps:   https://maps.google.com/maps/?&z=10&q=");
        Serial.print(latitude,6);
        Serial.print(",");
        Serial.println(longitude, 6);           
     }

     if (idadeInfo != TinyGPS::GPS_INVALID_AGE) {
        Serial.print("Edad de informacion(ms): ");
        Serial.println(idadeInfo);
     }*/


     //Dia e Hora
     int ano;
     byte mes, dia, hora, minuto, segundo, centesimo;
     gps1.crack_datetime(&ano, &mes, &dia, &hora, &minuto, &segundo, &centesimo, &idadeInfo);

     /*Serial.print("Data (GMT): ");
     Serial.print(dia);
     Serial.print("/");
     Serial.print(mes);
     Serial.print("/");
     Serial.println(ano);*/

     Serial.print("Horario (GMT): ");
     Serial.print(hora);
     Serial.print(":");
     Serial.print(minuto);
     Serial.print(":");
     Serial.print(segundo);
     Serial.print(":");
     Serial.println(centesimo);


     //altitude
     float altitudeGPS;
     altitudeGPS = gps1.f_altitude();

     if ((altitudeGPS != TinyGPS::GPS_INVALID_ALTITUDE) && (altitudeGPS != 1000000)) {
        Serial.print("Altitud (m): ");
        Serial.println(altitudeGPS);
     }


     /*velocidade
     float velocidade;
     //velocidade = gps1.speed();        
     velocidade = gps1.f_speed_kmph();   

     Serial.print("Velocidad (km/h): ");
     Serial.println(velocidade, 2);  



     //sentito (em centesima de graus)
     unsigned long sentido;
     sentido = gps1.course();

     Serial.print("Sentido (grados): ");
     Serial.println(float(sentido) / 100, 2);


     
     unsigned short satelites;
     unsigned long precisao;
     satelites = gps1.satellites();
     precisao =  gps1.hdop();

     if (satelites != TinyGPS::GPS_INVALID_SATELLITES) {
        Serial.print("Satelites: ");
        Serial.println(satelites);
     }

     if (precisao != TinyGPS::GPS_INVALID_HDOP) {
        Serial.print("Precision (centesimos de segundo): ");
        Serial.println(precisao);
     }*/


     //float distancia_entre;
     //distancia_entre = gps1.distance_between(lat1, long1, lat2, long2);

     //float sentido_para;
     //sentido_para = gps1.course_to(lat1, long1, lat2, long2);
  }
}

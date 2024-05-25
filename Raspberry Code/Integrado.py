import time #importa time por el comando sleep
import os
import subprocess
import camarita2 as cam
try:
    from smbus2 import SMBus
except ImportError:
    from smbus import SMBus
from bme280 import BME280

# Inicializa el BME280
bus = SMBus(1)
bme280 = BME280(i2c_dev=bus)

#Para detección de Apogeo
altura_minima = 5.0 # 5 metros minimo para activarse ( cambiar )
ascensoActual = 0
ascensoMaximo = 0
descensoMaximo = 0
descensoActual = 0
alturaBase = 0
deltaAltura = 0

#Altura geográfica
loc_a = 1509
date  = time.strftime('%d%b%y') 
hour = time.strftime('%H-%M-%S')
# Descartando el primer valor
temperatura = bme280.get_temperature()
presion = bme280.get_pressure()
humedad = bme280.get_humidity()
altitud = bme280.get_altitude(qnh =1017.2)
a_loc = altitud - loc_a
print('Comienzo de lectura en 3 segundos')
print('Inicio:', hour, ' ', date )
time.sleep(3)


data_file = open('datos.txt','a')
data_file.write('Inicio:' + str(hour) + ' ' + str(date) + '\n')

while True:
    temperatura = bme280.get_temperature()
    presion = bme280.get_pressure()
    humedad = bme280.get_humidity()
    altitud = bme280.get_altitude(qnh =1013.25)
    a_loc = altitud - loc_a
    print('{:05.2f}*C {:05.2f}hPa {:05.2f}% {:05.2f}m'.format(temperatura, presion, humedad, altitud))
    time.sleep(3)
    data_file = open('datos.txt','a')
    data_file.write(str(temperatura) + ',' + str (presion) + ',' + str(humedad) + ',' + str(altitud )+ '\n')
    data_file.close()

    #Deteccion Apogeo
    Altura = float( round ( bme280.get_altitude(qnh =1013.25) ,3))
    Presion = float( round (bme280.get_pressure() ,3))
    Temperatura =  float( round (bme280.get_temperature() ,3))
        
    ##Algoritmo de deteccion de cambio de altura 
    if alturaBase == 0:
            alturaBase = Altura
            ascensoMaximo = Altura
            descensoMaximo = Altura

    elif(Altura > ascensoMaximo):
            ascensoMaximo = Altura

    elif(Altura < descensoMaximo):
SS            descensoMaximo = Altura

    ascensoActual = Altura - descensoMaximo
    descensoActual = float (round (ascensoMaximo - Altura , 3 ))
    deltaAltura = float (round (ascensoMaximo - descensoMaximo, 3 ))
    
    if (descensoActual > altura_minima):
        print("Ha descendido más de 5 metros ")
        print("Código de la cámara en ejecución")
        cam.take_pictures()
        break
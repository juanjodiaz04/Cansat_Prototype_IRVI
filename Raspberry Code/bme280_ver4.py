import time #importa time por el comando sleep
try:
    from smbus2 import SMBus
except ImportError:
    from smbus import SMBus
from bme280 import BME280

# Inicializa el BME280
bus = SMBus(1)
bme280 = BME280(i2c_dev=bus)

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
    altitud = bme280.get_altitude(qnh =1017.2)
    a_loc = altitud - loc_a
    print('{:05.2f}*C {:05.2f}hPa {:05.2f}% {:05.2f}m'.format(temperatura, presion, humedad, a_loc))
    time.sleep(3)
    data_file = open('datos.txt','a')
    data_file.write(str(temperatura) + ',' + str (presion) + ',' + str(humedad) + ',' + str(a_loc) + '\n')
    data_file.close()
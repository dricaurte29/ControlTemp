import serial
import csv


puerto_serial = 'COM4'  


try:
    ser = serial.Serial(puerto_serial, baudrate=115200, timeout=1)
except serial.SerialException as e:
    print(f"Error al abrir el puerto serial: {e}")
    exit(1)


nombre_archivo = 'datos_arduino.csv'

with open(nombre_archivo, mode='w', newline='') as archivo_csv:
    
    csv_writer = csv.writer(archivo_csv)

    
    csv_writer.writerow(['U', 'seg', 'tempF', 'pwm'])

    
    while True:
        linea = ser.readline().decode('utf-8').strip()
        
        
        partes = linea.split(',')
        
        if len(partes) == 4 and partes[0].startswith('U:') and partes[1].startswith('seg:') \
                and partes[2].startswith('tempF:') and partes[3].startswith('pwm:'):
            valores = [parte.split(':')[1] for parte in partes]
            csv_writer.writerow(valores)
            print(f"Datos guardados: {valores}")


ser.close()

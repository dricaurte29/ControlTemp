import csv
import matplotlib.pyplot as plt


archivo_csv = 'datos_arduino.csv'
columna_seg = []
columna_pwm = []
columna_tempF = []

with open(archivo_csv, mode='r') as archivo:
    lector_csv = csv.DictReader(archivo)
    for fila in lector_csv:
        columna_seg.append(float(fila['seg']))
        columna_pwm.append(float(fila['pwm']))
        columna_tempF.append(float(fila['tempF']))


plt.figure(figsize=(10, 6))
plt.plot(columna_seg, columna_pwm, marker='o', color='blue', label='PWM vs Segundos', linestyle='-')
plt.plot(columna_seg, columna_tempF, marker='x', color='red', label='TempF vs Segundos', linestyle='-')
plt.title('PWM y TempF vs Segundos')
plt.xlabel('Segundos')
plt.ylabel('PWM / TempF')
plt.grid(True)
plt.legend()
plt.tight_layout()

plt.show()

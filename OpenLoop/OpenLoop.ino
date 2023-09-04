// Añadir librerías
#include <OneWire.h> // (!!!! DIGITAL SENSOR !!!!)
#include <DallasTemperature.h> // (!!!! DIGITAL SENSOR !!!!)

// Definir variables
#define SensorInput_GPIO 8 
#define OutputPWM_GPIO 9  

// Variables for Conversion
#define Uunits 100 // Set units for Control Output (u) [MAX value of (u) @ MAX pwmDutyCycle] (e.g. MAX Transistor Collector Current [mA])
//#define scaleVadc 5.0 // Set analog read voltage value 3.3 [V] for ESP32 ADC (!!!! ANALOG SENSOR !!!!)

// Execution Time Control
long previousMillis = 0;  // For main loop function
long Ts = 1000; // Sample time in ms
long previousMillis2 = 0; // For auxiliary functions (squarewaves)

// Measurement Variables
OneWire oneWire(SensorInput_GPIO); // (!!!! DIGITAL SENSOR !!!!)
DallasTemperature TempSensor(&oneWire); // (!!!! DIGITAL SENSOR !!!!)
//int tempAverages = 20; // # of samples for filtering (!!!! ANALOG SENSOR !!!!)
float tempF = 0.0; 

int pasos[8][2]={{13,13},{10,15},{7,18},{-11,21},{13,24},{-14,28},{-7,34},{-10,40}};

// Control System Variables
float U_op = 50.0; // Direct Control Output - FOR OPENLOOP or FEEDFORWARD - Transistor Collector Current [mA]
float U_t = 0.0; // Control Output
int Tp = 8;
unsigned int pwmDuty = 0; // Control Output (Converted to PWM Duty Cycle)

void calibracion(void){
  // Measurement, Control, Output Command Signal, Serial Data Communication
  unsigned long currentMillis = millis(); // Update current time from the beginning
  if (currentMillis - previousMillis >= Ts) {
    previousMillis = currentMillis;
    TempSensor.requestTemperatures();  tempF = TempSensor.getTempCByIndex(0); 
    float U_t = U_op;

    if (currentMillis/1000 > Tp){
      for (int i=0; i<8; i++) {
        if (currentMillis/1000 > pasos[i][1]){
          U_t = U_t + pasos[i][0];
        }
      }
    }
    if (currentMillis/1000 > pasos[7][1]) 
    digitalWrite(13,HIGH);  
    float U_tl = min(max(U_t, 0), Uunits); // Saturated Control Output
    pwmDuty = int((U_tl/Uunits)*255);
    analogWrite(OutputPWM_GPIO, pwmDuty);
    
    
    Serial.print("U:");
    Serial.print(U_t);
    Serial.print(",");
    Serial.print("seg:");
    Serial.print(millis()/1000);
    Serial.print(",");
    Serial.print("tempF:");
    Serial.print(tempF);
    Serial.print(",");
    Serial.print( "pwm:");
    Serial.println(pwmDuty);     
  }
}

// the setup function runs once when you press reset or power the board
void setup() {
  
  Serial.begin(115200); 
  pinMode(13,OUTPUT);
  // Set up inputs (!!!! DIGITAL SENSOR !!!!)
  TempSensor.begin(); // Sensor start command
  TempSensor.setResolution(12); // Sets the maximum supported resolution 12bit. (DS18B20 Accuracy 0.5°C)
}

// the loop function runs over and over again forever
void loop() {
  calibracion();
}

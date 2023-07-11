#include <Wire.h>
#include <Adafruit_INA219.h>

Adafruit_INA219 ina219;

// Define pins 
int base = 18;

// Reporting frequency
float freq = 10; // Hz
// Delay after changing state of transistor
int del = 10; 

// Sensor variables 
float current_mA = 0;
float voltage = 0;
float power_mW = 0;

// Tracking time
unsigned long last = 0;
//unsigned long led_last = 0;
float t = 0;
//int led = 2000;
//bool state = false;

// Run once
void setup(void) 
{
  pinMode(base, OUTPUT);
  pinMode(2, OUTPUT);
  Serial.begin(115200);
  while (!Serial) {
      // will pause Zero, Leonardo, etc until serial console opens
      delay(1);
  }
    
  Serial.println("Hello!");
  
  // Initialize the INA219.
  // By default the initialization will use the largest range (32V, 2A).  However
  // you can call a setCalibration function to change this range (see comments).
  if (! ina219.begin()) {
    Serial.println("Failed to find INA219 chip");
    while (1) { delay(10); }
  }
  // To use a slightly lower 32V, 1A range (higher precision on amps):
  //ina219.setCalibration_32V_1A();
  // Or to use a lower 16V, 400mA range (higher precision on volts and amps):
  ina219.setCalibration_16V_400mA();

  Serial.println("Measuring voltage and current with INA219 ...");

  last = millis();
//  led_last = millis();
}

// Run indefinitely
void loop(void) 
{ 
  if (millis() - last > 1000.0/freq){
    last = millis();
    t = last/1000.0; // Time in seconds

    // measure current by turning on the transistor to measure I_sc
    digitalWrite(base, HIGH);
    delay(del);
    current_mA = ina219.getCurrent_mA();
    
    // measure voltage by turning transistor low for V_oc
    digitalWrite(base, LOW);
    delay(del);
    voltage = ina219.getBusVoltage_V();
    
    // *** END OF POWER COLLECTION 

    // Max power (mW) is at roughly 0.7x the voltage
    power_mW = (0.7*voltage)*current_mA;

    // Format: Time, Voltage, Current, Estimated Power
//    Serial.print(t); Serial.print(", "); 
//    Serial.print(voltage); Serial.print(", "); 
//    Serial.print(current_mA); Serial.print(", ");
    Serial.println(power_mW);
  }
}

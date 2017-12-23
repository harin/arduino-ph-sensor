#include <Wire.h> 

#define PH_401_VOLTAGE 3.00
#define PH_700_VOLTAGE 2.20
#define PH_SENSOR_PIN A0

double m = 2.99 / ( PH_700_VOLTAGE - PH_401_VOLTAGE);
double b = 7 - ( m * PH_700_VOLTAGE );

void setup() {
  Serial.begin(9600);
  pinMode(PH_SENSOR_PIN, INPUT);

  Wire.begin();
}

void loop() {

  double value = analogRead(PH_SENSOR_PIN);
  double voltage = value * 5.0/ 1024.0;
  double pH = (m * voltage) + b;
  Serial.print("voltage = ");
  Serial.print(voltage, 4);
  Serial.print("\t pH = ");
  Serial.println(pH, 4);
  
  // Transmit data
  Wire.beginTransmission(9);
  String phStr = String(pH, 4);
  Wire.write((int) 0);
  Wire.endTransmission();
  
  delay(1000);
}

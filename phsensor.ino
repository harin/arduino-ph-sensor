#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <OneWire.h> 
#include <DallasTemperature.h>
#include <TM1637Display.h>


#define PH_401_VOLTAGE 3.00
#define PH_700_VOLTAGE 2.20
#define PH_SENSOR_PIN A0
#define DHT_PIN 13
#define DHT_TYPE DHT11
#define ONE_WIRE_BUS 2 
#define READ_FREQUENCY_MS 60000

OneWire oneWire(ONE_WIRE_BUS); 
DallasTemperature waterTempSensor(&oneWire);


double m = 2.99 / ( PH_700_VOLTAGE - PH_401_VOLTAGE);
double b = 7 - ( m * PH_700_VOLTAGE );
int buf[10],temp;
DHT dht = DHT(DHT_PIN, DHT_TYPE);

void setup() {
  Serial.begin(9600);
  pinMode(PH_SENSOR_PIN, INPUT);
  dht.begin();
  waterTempSensor.begin(); 
}

void readPH() {
    // read 10 values 
  for(int i = 0; i < 10; i++) {
    buf[i] = analogRead(PH_SENSOR_PIN);
    delay(10);
  }

  // sort
  for(int i = 0; i < 9; i++) {
    for(int j = i + 1; j < 10; j++) {
      if(buf[i] > buf[j]) {
        temp = buf[i];
        buf[i] = buf[j];
        buf[j] = temp;
      }
    }
  }

  // average and drop head and tail
  double avgValue = 0;
  for(int i = 2; i < 8; i++) {
    avgValue += buf[i];
  }

  avgValue = avgValue / 6;
  double voltage = avgValue * 5 / 1024;

  double pH = (m * voltage) + b;
  
  Serial.print("voltage = ");
  Serial.print(voltage, 4);
  Serial.print("\t pH = ");
  Serial.print(pH, 4);
}

void readTemp() {
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  Serial.print("temp = ");
  Serial.print(t);
  Serial.print("\t humidity = ");
  Serial.print(h);
}

void readWaterTemp() {
  float temp = waterTempSensor.getTempCByIndex(0);
  Serial.print("\t watertemp =");
  Serial.print(temp);
}

void loop() {
  readPH();
  Serial.print("\t");
  readTemp();
  readWaterTemp();
  Serial.print("\n");
  delay(READ_FREQUENCY_MS);
}

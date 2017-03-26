#include <DHT.h>
#include <Wire.h>
#include <BH1750.h>
#include <avr/pgmspace.h>
#include <LowPower.h>

#define DHTPIN 3
#define DHTPIN_VCC 4
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

BH1750 lightMeter(0x23);

void readTempHum();
void readLight();

void setup() {
  pinMode(DHTPIN_VCC,OUTPUT);
  digitalWrite(DHTPIN_VCC,LOW);

  Serial.begin(115200);
  Serial.println(F("DHTxx test!"));
  dht.begin();

  lightMeter.begin(BH1750_CONTINUOUS_HIGH_RES_MODE);
  Serial.println(F("BH1750 Test"));
  
  delay(100); // give some time to send data over Serial before going to sleep
}

void loop() {
  // Enter power down state for 8 s with ADC and BOD module disabled
  LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
  delay(100); // give him some time to wake up from sleep :D
  readTempHum();
  readLight();
  delay(100); // give some time to send data over Serial
}


void readTempHum() {
  digitalWrite(DHTPIN_VCC, HIGH);

  // Wait at least 2 seconds seconds between measurements.
  unsigned long previousMillis = millis();
  unsigned long currentMillis = millis();
  while (currentMillis - previousMillis <= 2300) {
    // save the last time you read the sensor
    currentMillis = millis();
  }

  Serial.print(F("Humidity: "));
  Serial.print(dht.readHumidity());
  Serial.print(F(" %\t"));
  Serial.print(F("Temperature: "));
  Serial.print(dht.readTemperature());
  Serial.println(F(" *C "));

  digitalWrite(DHTPIN_VCC,LOW);
}

void readLight() {
  Serial.print(F("Light: "));
  Serial.print(lightMeter.readLightLevel());
  Serial.println(F(" lx"));
}

#include <Wire.h>
#include <Adafruit_BME280.h>

Adafruit_BME280 bme;

const int LDR_PIN = 34;

void initBME280() {
  if (!bme.begin(0x76)) {
    Serial.println("BME280 sensor not found");
    while(1);
  }

  Serial.println("BME280 initialized successfully!");
}

void readLight() {
  int lightState = digitalRead(LDR_PIN);

  if (lightState == LOW) {
    Serial.println("Day");
  } else {
    Serial.println("Night");
  }
}

void readTemperature() {
  float temperature = bme.readTemperature();
  Serial.println("Temperature: ");
  Serial.println(temperature);
  Serial.println(" C");
}

void readPressure() {
  float pressure = bme.readPressure();
  Serial.println("Pressure: ");
  Serial.println(pressure);
  Serial.println(" Pa");
}

void readHumidity() {
  float humidity = bme.readHumidity();
  Serial.println("Humidity: ");
  Serial.println(humidity);
  Serial.println(" %");
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("ESP32 initialized successfully!");
  pinMode(LDR_PIN, INPUT);

  // initialize sensors
  initBME280();
}

void loop() {
  // put your main code here, to run repeatedly:
  readTemperature();
  readPressure();
  readHumidity();
  readLight();
  Serial.println("---");
  delay(1 * 1000);
}

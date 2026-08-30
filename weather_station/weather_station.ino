#include <Adafruit_BME280.h>
#include <Wire.h>

enum LogLevel { INFO, WARNING, ERROR };

struct LogData {
  float temperature;
  float pressure;
  float humidity;
  const char *light;
};

Adafruit_BME280 bme;

const int LDR_PIN = 34;

const char *getLevelName(LogLevel level) {
  switch (level) {
  case INFO:
    return "INFO";
  case WARNING:
    return "WARNING";
  case ERROR:
    return "ERROR";
  default:
    return "UNKNOWN";
  }
}

void logEvent(LogLevel level, const char *source, const char *message) {
  const char *levelName = getLevelName(level);

  Serial.printf("[%lu] [%s] [%s] %s\n", millis(), levelName, source, message);
}

void logData(LogLevel level, const LogData &data) {
  const char *levelName = getLevelName(level);

  Serial.printf(
      "[%lu] [%s] temperature=%.2f pressure=%.2f humidity=%.2f light=%s\n",
      millis(), levelName, data.temperature, data.pressure, data.humidity,
      data.light);
}

void initBME280() {
  if (!bme.begin(0x76)) {
    logEvent(WARNING, "BME280", "Not found");
    while (1)
      ;
  }

  logEvent(INFO, "BME280", "Found");
}

const char *readLight() {
  int lightState = digitalRead(LDR_PIN);

  if (lightState == LOW) {
    return "DAY";
  } else {
    return "NIGHT";
  }
}

float readTemperature() { return bme.readTemperature(); }

float readPressure() { return bme.readPressure(); }

float readHumidity() { return bme.readHumidity(); }

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  logEvent(INFO, "ESP32", "Initialized");
  pinMode(LDR_PIN, INPUT);

  // initialize sensors
  initBME280();
}

void loop() {
  // put your main code here, to run repeatedly:
  float temperature = readTemperature();
  float pressure = readPressure();
  float humidity = readHumidity();
  const char *light = readLight();

  logData(INFO, {temperature, pressure, humidity, light});

  delay(10 * 1000);
}

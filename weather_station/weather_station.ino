#include <Adafruit_BME280.h>
#include <Wire.h>

enum LogLevel { INFO, WARNING, ERROR };

struct LogData {
  float temperature;
  float pressure;
  float humidity;
  const char *light;
  int water;
};

Adafruit_BME280 bme;

const int LDR_PIN = 34;

const int WATER_SIGNAL_PIN = 35;
const int WATER_POWER_PIN = 25;
const int WATER_SETTLE_MS = 10;

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

  Serial.printf("[%lu] [%s] temperature=%.2f pressure=%.2f humidity=%.2f "
                "light=%s water=%d\n",
                millis(), levelName, data.temperature, data.pressure,
                data.humidity, data.light, data.water);
}

void initBME280() {
  if (!bme.begin(0x76)) {
    logEvent(WARNING, "BME280", "Not found");
    while (1)
      ;
  }

  logEvent(INFO, "BME280", "Found");
}

void initWaterSensor() {
  pinMode(WATER_POWER_PIN, OUTPUT);
  digitalWrite(WATER_POWER_PIN, LOW);

  analogReadResolution(12);
  analogSetPinAttenuation(WATER_SIGNAL_PIN, ADC_11db);

  logEvent(INFO, "WATER", "Initialized");
}

int readWaterLevel() {
  digitalWrite(WATER_POWER_PIN, HIGH);
  delay(WATER_SETTLE_MS);

  int waterLevel = analogRead(WATER_SIGNAL_PIN);

  digitalWrite(WATER_POWER_PIN, LOW);

  return waterLevel;
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
  initWaterSensor();
}

void loop() {
  // put your main code here, to run repeatedly:
  float temperature = readTemperature();
  float pressure = readPressure();
  float humidity = readHumidity();
  const char *light = readLight();
  int water = readWaterLevel();

  logData(INFO, {temperature, pressure, humidity, light, water});

  delay(10 * 1000);
}

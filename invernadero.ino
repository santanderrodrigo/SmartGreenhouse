#include "DHTSensor.h"
#include "LCDDisplay.h"
#include "ActuatorController.h"

// Constants
#define DHTPIN 2          // Pin where the DHT11 is connected
#define DHTTYPE DHT11     // DHT 11
#define FAN_PIN 3         // Pin for the fan motor
#define PUMP_PIN 4        // Pin for the water pump
#define GREEN_LED_PIN 5   // Pin for the green LED
#define RED_LED_PIN 6     // Pin for the red LED

#define TEMP_THRESHOLD 25 // Temperature threshold in °C
#define HUM_THRESHOLD 40  // Humidity threshold in %

#define TEMP_INTERVAL 2000 // Interval for temperature check in milliseconds
#define HUM_INTERVAL 2000  // Interval for humidity check in milliseconds

Sensor* sensor; // DHT11 sensor
Display* display; // LCD display
ActuatorController* actuatorController; // Actuator controller
unsigned long previousTempMillis = 0; // Previous time for temperature check
unsigned long previousHumMillis = 0; // Previous time for humidity check

void setup() {
  Serial.begin(9600);
  
  sensor = new DHTSensor(DHTPIN, DHTTYPE);
  sensor->begin();
  
  display = new LCDDisplay(7, 8, 9, 10, 11, 12); // Example pin configuration
  display->begin();
  
  actuatorController = new ActuatorController(FAN_PIN, PUMP_PIN, GREEN_LED_PIN, RED_LED_PIN);
  actuatorController->begin();
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousTempMillis >= TEMP_INTERVAL) {
    previousTempMillis = currentMillis;
    checkTemperature();
  }

  if (currentMillis - previousHumMillis >= HUM_INTERVAL) {
    previousHumMillis = currentMillis;
    checkHumidity();
  }
}

void checkTemperature() {
  float temperature = sensor->readTemperature();

  if (isnan(temperature)) {
    Serial.println("Failed to read temperature from sensor!");
    return;
  }

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" *C");

  display->showTemperature(temperature);

  if (temperature >= TEMP_THRESHOLD) {
    actuatorController->turnFanOn();
  } else {
    actuatorController->turnFanOff();
  }
}

void checkHumidity() {
  float humidity = sensor->readHumidity();

  if (isnan(humidity)) {
    Serial.println("Failed to read humidity from sensor!");
    return;
  }

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  display->showHumidity(humidity);

  if (humidity <= HUM_THRESHOLD) {
    actuatorController->turnPumpOn();
  } else {
    actuatorController->turnPumpOff();
  }
}
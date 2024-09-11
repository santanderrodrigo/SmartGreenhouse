#include "DHTSensor.h"
#include "LCDDisplay.h"
#include "ActuatorController.h"

// Constants
#define DHTPIN 13          // Pin where the DHT11 is connected
#define DHTTYPE DHT11     // DHT 11
#define FAN_PIN 6         // Pin for the fan motor
#define PUMP_PIN 7        // Pin for the water pump
#define GREEN_LED_PIN 11   // Pin for the green LED
#define RED_LED_PIN 12     // Pin for the red LED

#define TEMP_THRESHOLD 25 // Temperature threshold in °C
#define HUM_THRESHOLD 40  // Humidity threshold in %

#define TEMP_HYSTERESIS 0 // Temperature hysteresis in °C
#define HUM_HYSTERESIS 0  // Humidity hysteresis in %

#define TEMP_INTERVAL 2000 // Interval for temperature check in milliseconds
#define HUM_INTERVAL 2000  // Interval for humidity check in milliseconds

Sensor* sensor; // DHT11 sensor
Display* display; // LCD display
ActuatorController* actuatorController; // Actuator controller
unsigned long previousTempMillis = 0; // Previous time for temperature check
unsigned long previousHumMillis = 0; // Previous time for humidity check

bool fanOn = false; // State of the fan
bool pumpOn = false; // State of the pump

// Setup function
void setup() {
  Serial.begin(9600);
  
  sensor = new DHTSensor(DHTPIN, DHTTYPE); 
  sensor->begin(); // Initialize the sensor
  
  // RW Pin is not used, so it is connected to GND, because the display stays in write mode
  display = new LCDDisplay(10, 8, 5, 4, 3, 2); // Display pin configuration
  display->begin(); // Initialize the display
  
  actuatorController = new ActuatorController(FAN_PIN, PUMP_PIN, GREEN_LED_PIN, RED_LED_PIN);
  actuatorController->begin(); // Initialize the actuator controller
}

// Main loop
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

  if (fanOn) {
    if (temperature <= TEMP_THRESHOLD - TEMP_HYSTERESIS) {
      actuatorController->turnFanOff();
      fanOn = false;
    }
  } else {
    if (temperature >= TEMP_THRESHOLD) {
      actuatorController->turnFanOn();
      fanOn = true;
    }
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

  if (pumpOn) {
    if (humidity >= HUM_THRESHOLD + HUM_HYSTERESIS) {
      actuatorController->turnPumpOff();
      pumpOn = false;
    }
  } else {
    if (humidity <= HUM_THRESHOLD) {
      actuatorController->turnPumpOn();
      pumpOn = true;
    }
  }
}
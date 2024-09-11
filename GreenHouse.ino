#include "DHTSensor.h"
#include "LCDDisplay.h"
#include "ActuatorController.h"

// Declaramos las constantes
#define DHTPIN 13          // Pin donde está conectado el DHT11
#define DHTTYPE DHT11      // DHT 11
#define FAN_PIN 6          // Pin para el motor del ventilador
#define PUMP_PIN 7         // Pin para la bomba de agua
#define GREEN_LED_PIN 11   // Pin para el LED verde
#define RED_LED_PIN 12     // Pin para el LED rojo

#define RW_PIN 9           // Pin para el pin RW de la pantalla LCD

#define TEMP_THRESHOLD 25  // Umbral de temperatura en °C
#define HUM_THRESHOLD 40   // Umbral de humedad en %

#define TEMP_HYSTERESIS 0  // Histéresis de temperatura en °C
#define HUM_HYSTERESIS 0   // Histéresis de humedad en %

#define TEMP_INTERVAL 2000 // Intervalo para la verificación de temperatura en milisegundos
#define HUM_INTERVAL 2000  // Intervalo para la verificación de humedad en milisegundos

Sensor* sensor; // Sensor DHT11
Display* display; // Pantalla LCD
ActuatorController* actuatorController; // Controlador de actuadores
unsigned long previousTempMillis = 0; // Tiempo anterior para la verificación de temperatura
unsigned long previousHumMillis = 0; // Tiempo anterior para la verificación de humedad

bool fanOn = false; // Estado del ventilador
bool pumpOn = false; // Estado de la bomba

// Función de configuración
void setup() {
  Serial.begin(9600); // Inicializamos el puerto serie en 9600 baudios
  
  sensor = new DHTSensor(DHTPIN, DHTTYPE); 
  sensor->begin(); // Inicializamos el sensor
  
  // El pin RW no se usa, así que se conecta a GND, porque la pantalla permanece en modo escritura
  // seteamos el pin RW como salida y lo ponemos en LOW
  pinMode(RW_PIN, OUTPUT);
  display = new LCDDisplay(10, 8, 5, 4, 3, 2); // Configuración de pines de la pantalla
  display->begin(); // Inicializamos la pantalla
  
  actuatorController = new ActuatorController(FAN_PIN, PUMP_PIN, GREEN_LED_PIN, RED_LED_PIN);
  actuatorController->begin(); // Inicializamos el controlador de actuadores
}

// Bucle principal
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
    Serial.println("¡Error al leer la temperatura del sensor!");
    return;
  }

  Serial.print("Temperatura: ");
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
    Serial.println("¡Error al leer la humedad del sensor!");
    return;
  }

  Serial.print("Humedad: ");
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
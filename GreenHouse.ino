#include "DHTSensor.h" // Incluímos la clase DHTSensor
#include "LCDDisplay.h" // Incluímos la clase LCDDisplay
#include "ActuatorController.h" // Incluímos la clase ActuatorController
#include "SoilMoistureSensor.h" // Incluímos la clase SoilMoistureSensor
#include "ControlProfile.h" // Include the ControlProfile header
#include "TaskScheduler.h" // Incluímos la clase TaskScheduler
#include "TimerManager.h" // Incluímos la clase TimerManager

// Perfiles de configuración de pines para Arduino Uno y Arduino Mega
#if defined(ARDUINO_AVR_MEGA2560)

  //Sensores
  #define DHT_PIN 14          // Pin donde está conectado el DHT11 --> en mega usamos el puerto 14
  #define HUMEDITY_SOIL_1_PIN A0 // Pin donde está conectado el sensor de humedad del suelo 1 
  #define HUMEDITY_SOIL_2_PIN A1 // Pin donde está conectado el sensor de humedad del suelo 2 
  #define HUMEDITY_SOIL_3_PIN A2 // Pin donde está conectado el sensor de humedad del suelo 3 

  //Actuadores
  #define FAN_PIN 25          // Pin para el motor del ventilador
  #define PUMP_1_PIN 22         // Pin para la bomba de agua
  #define PUMP_2_PIN 23
  #define PUMP_3_PIN 24
  #define SPRAYER_PIN 26

  //LCD Display
  #define RW_PIN 6           // Pin RW de la pantalla LCD
  #define RS_PIN 7          
  #define E_PIN 8
  #define D4_PIN 9
  #define D5_PIN 11
  #define D6_PIN 12
  #define D7_PIN 13

  //ENC28J60 
  #define ENC28J60_SCK_PIN 52
  #define ENC28J60_MISO_PIN 50
  #define ENC28J60_MOSI_PIN 51
  #define ENC28J60_CS_PIN 10 //IO2 en el diagrama

#else
  #error "Esta configuración está diseñada para Arduino Arduino Mega. Por favor, selecciona esta placa en el menú Herramientas > Placa."
#endif

#define DHTTYPE DHT11    // DHT 11 or DHT 22
#define TEMP_INTERVAL_SCHEDULER 5000 // Intervalo de tiempo para verificar la temperatura
#define DISPLAY_INTERVAL_SCHEDULER 1000 // Intervalo de tiempo para actualizar la pantalla
#define TEMP_EXTREME_THRESHOLD 38.0 // Umbral de temperatura extrema
#define SPRAYER_ON_DURATION 300000 // Duración del aspersor encendido (5 minutos)
#define SPRAYER_OFF_DURATION 600000 // Duración del aspersor apagado (10 minutos)

Sensor* dhtSensor; // Sensor DHT11
SoilMoistureSensor* soilSensor1;
SoilMoistureSensor* soilSensor2;
SoilMoistureSensor* soilSensor3;
Display* display; // Pantalla LCD
ActuatorController* actuatorController; // Controlador de actuadores
ControlProfile* controlProfile; // Perfil de control

float currentTemperature = 0.0; // Variable para almacenar la temperatura actual
float currentHumidity = 0.0; // Variable para almacenar la humedad actual

int currentScreen = 0; // Variable para rastrear la pantalla actual

TaskScheduler dhtScheduler(TEMP_INTERVAL_SCHEDULER);
TaskScheduler displayScheduler(DISPLAY_INTERVAL_SCHEDULER);
TimerManager timerManager;

// Función de configuración, se ejecuta una vez al inicio
void setup() {
  Serial.begin(9600);

  dhtSensor = new DHTSensor(DHT_PIN, DHTTYPE);
  dhtSensor->begin();

  soilSensor1 = new SoilMoistureSensor(HUMEDITY_SOIL_1_PIN);
  soilSensor2 = new SoilMoistureSensor(HUMEDITY_SOIL_2_PIN);
  soilSensor3 = new SoilMoistureSensor(HUMEDITY_SOIL_3_PIN);
  soilSensor1->begin();
  soilSensor2->begin();
  soilSensor3->begin();

  display = new LCDDisplay(RS_PIN, RW_PIN, E_PIN, D4_PIN, D5_PIN, D6_PIN, D7_PIN);
  display->begin();
  display->showMessage(0, 0, "Iniciando...");
  delay(500);
  display->showMessage(0, 1, "Smart GreenHouse");

  actuatorController = new ActuatorController(FAN_PIN, PUMP_1_PIN, PUMP_2_PIN, PUMP_3_PIN, SPRAYER_PIN);
  actuatorController->begin();

  // Crear un perfil de control
  controlProfile = new ControlProfile(25.0, 40.0, 2.0, 2.0, 15.0, 30.0, 30.0, 60.0, 20.0, 50.0, 60000, 300000);

  // Añadir temporizadores al TimerManager
  timerManager.addTimer("sprayerOn", SPRAYER_ON_DURATION);
  timerManager.addTimer("sprayerOff", SPRAYER_OFF_DURATION);
}

// Bucle principal, se ejecuta continuamente
void loop() {
  if (dhtScheduler.shouldRun()) {
    currentTemperature = checkTemperature();
    currentHumidity = checkHumidity();
  }

  if (displayScheduler.shouldRun()) {
    updateDisplay();
  }

  // Control de temperatura y humedad
  controlTemperatureHumidity();

  // Control de humedad del suelo
  controlSoilMoisture();
}

// Función para verificar la temperatura
float checkTemperature() {
  float temperature = dhtSensor->readTemperature();

  if (isnan(temperature)) {
    Serial.println("¡Error al leer la temperatura del sensor!");
    return NAN;
  }

  Serial.print("Temperatura: ");
  Serial.print(temperature);
  Serial.println(" *C");

  display->showTemperature(temperature);

  return temperature;
}

// Función para verificar la humedad
float checkHumidity() {
  float humidity = dhtSensor->readHumidity();

  if (isnan(humidity)) {
    Serial.println("¡Error al leer la humedad del sensor!");
    return NAN;
  }

  Serial.print("Humedad: ");
  Serial.print(humidity);
  Serial.println(" %");

  display->showHumidity(humidity);

  return humidity;
}

// Función para controlar la temperatura y la humedad
void controlTemperatureHumidity() {
  float tempThreshold = controlProfile->getTempThreshold();
  float tempHysteresis = controlProfile->getTempHysteresis();
  float humThreshold = controlProfile->getHumThreshold();
  float humHysteresis = controlProfile->getHumHysteresis();

  bool fanShouldBeOn = false;
  bool sprayerShouldBeOn = false;

  // Control del ventilador basado en la temperatura
  if (currentTemperature >= tempThreshold) {
    fanShouldBeOn = true;
  } else if (currentTemperature <= tempThreshold - tempHysteresis) {
    fanShouldBeOn = false;
  }

  // Control del ventilador basado en la humedad (solo si la temperatura no lo prioriza)
  if (!fanShouldBeOn) {
    if (currentHumidity >= humThreshold) {
      fanShouldBeOn = true;
    } else if (currentHumidity <= humThreshold - humHysteresis) {
      fanShouldBeOn = false;
    }
  }

  // Aplicar el estado del ventilador
  if (fanShouldBeOn) {
    if (!actuatorController->isOn(FAN)) {
      actuatorController->turnOn(FAN);
    }
  } else {
    if (actuatorController->isOn(FAN)) {
      actuatorController->turnOff(FAN);
    }
  }

  // Control del aspersor basado en la temperatura extrema
  if (currentTemperature >= TEMP_EXTREME_THRESHOLD) {
    if (actuatorController->isOn(SPRAYER)) {
      // Apagar el aspersor después de tiempo (definido por SPRAYER_ON_DURATION)
      if (timerManager.expired("sprayerOn")) {
        actuatorController->turnOff(SPRAYER);
        timerManager.start("sprayerOff");
      }
    } else {
      // Encender el aspersor después de tiempo (definido por SPRAYER_OFF_DURATION)
      if (timerManager.expired("sprayerOff")) {
        actuatorController->turnOn(SPRAYER);
        timerManager.start("sprayerOn");
      }
    }
  } else {
    // Control del aspersor basado en la humedad (solo si la temperatura extrema no lo prioriza)
    if (currentHumidity <= humThreshold - humHysteresis) {
      sprayerShouldBeOn = true;
    } else {
      sprayerShouldBeOn = false;
    }

    // Aplicar el estado del aspersor
    if (sprayerShouldBeOn) {
      if (!actuatorController->isOn(SPRAYER)) {
        actuatorController->turnOn(SPRAYER);
      }
    } else {
      if (actuatorController->isOn(SPRAYER)) {
        actuatorController->turnOff(SPRAYER);
      }
    }
  }
}

// Función para controlar la humedad del suelo
void controlSoilMoisture() {
  float minSoilMoisture = controlProfile->getMinSoilMoisture();
  float maxSoilMoisture = controlProfile->getMaxSoilMoisture();
  unsigned long infiltrationTime = controlProfile->getInfiltrationTime();
  unsigned long irrigationTime = controlProfile->getIrrigationTime();

  float soilMoisture1 = soilSensor1->readValue();
  float soilMoisture2 = soilSensor2->readValue();
  float soilMoisture3 = soilSensor3->readValue();

  // Control de la bomba 1
  if (soilMoisture1 < minSoilMoisture) {
    if (!actuatorController->isOn(PUMP1) && timerManager.expired("pump1Infiltration")) {
      actuatorController->turnOn(PUMP1);
      timerManager.addTimer("pump1Irrigation", irrigationTime);
    }
  } else if (soilMoisture1 > maxSoilMoisture) {
    if (actuatorController->isOn(PUMP1) && timerManager.expired("pump1Irrigation")) {
      actuatorController->turnOff(PUMP1);
      timerManager.addTimer("pump1Infiltration", infiltrationTime);
    }
  }

  // Control de la bomba 2
  if (soilMoisture2 < minSoilMoisture) {
    if (!actuatorController->isOn(PUMP2) && timerManager.expired("pump2Infiltration")) {
      actuatorController->turnOn(PUMP2);
      timerManager.addTimer("pump2Irrigation", irrigationTime);
    }
  } else if (soilMoisture2 > maxSoilMoisture) {
    if (actuatorController->isOn(PUMP2) && timerManager.expired("pump2Irrigation")) {
      actuatorController->turnOff(PUMP2);
      timerManager.addTimer("pump2Infiltration", infiltrationTime);
    }
  }

  // Control de la bomba 3
  if (soilMoisture3 < minSoilMoisture) {
    if (!actuatorController->isOn(PUMP3) && timerManager.expired("pump3Infiltration")) {
      actuatorController->turnOn(PUMP3);
      timerManager.addTimer("pump3Irrigation", irrigationTime);
    }
  } else if (soilMoisture3 > maxSoilMoisture) {
    if (actuatorController->isOn(PUMP3) && timerManager.expired("pump3Irrigation")) {
      actuatorController->turnOff(PUMP3);
      timerManager.addTimer("pump3Infiltration", infiltrationTime);
    }
  }
}

// Función para actualizar la pantalla
void updateDisplay() {
  display->clear();

  if (currentScreen == 0) {
    // Pantalla 0: Mostrar temperatura y humedad
    String tempMessage = "Temp: " + String(currentTemperature) + "C";
    String humMessage = "Hum: " + String(currentHumidity) + "%";
    display->showMessage(0, 0, tempMessage.substring(0, 16).c_str());
    display->showMessage(1, 0, humMessage.substring(0, 16).c_str());

    currentScreen = 1; // Cambiar a la siguiente pantalla
  } else if (currentScreen == 1) {
    // Pantalla 1: Mostrar estado del ventilador y rociador
    String fanMessage = "Fan: " + String(actuatorController->isOn(FAN) ? "On" : "Off");
    String sprayerMessage = "SPR: " + String(actuatorController->isOn(SPRAYER) ? "On" : "Off");
    display->showMessage(0, 0, fanMessage.substring(0, 16).c_str());
    display->showMessage(1, 0, sprayerMessage.substring(0, 16).c_str());

    currentScreen = 2; // Cambiar a la siguiente pantalla
  } else {
    // Pantalla 2: Mostrar estado de las bombas
    String pumpMessage1 = "P1: " + String(actuatorController->isOn(PUMP1) ? "On" : "Off") +
                          " P2: " + String(actuatorController->isOn(PUMP2) ? "On" : "Off");
    String pumpMessage2 = "P3: " + String(actuatorController->isOn(PUMP3) ? "On" : "Off");
    display->showMessage(0, 0, pumpMessage1.substring(0, 16).c_str());
    display->showMessage(1, 0, pumpMessage2.substring(0, 16).c_str());

    currentScreen = 0; // Cambiar a la siguiente pantalla
  }
}

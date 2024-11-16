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
  #error "Esta configuración está diseñada para Arduino Uno o Arduino Mega. Por favor, selecciona una de estas placas en el menú Herramientas > Placa."
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

bool fanOn = false; // Estado del ventilador
bool pump1On = false; // Estado de la bomba del circuito 1
bool pump2On = false; // Estado de la bomba del circuito 2
bool pump3On = false; // Estado de la bomba del circuito 3
bool sprayerOn = false; // Estado del aspersor de humedad

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

  // Control del ventilador basado en la temperatura
  if (currentTemperature >= tempThreshold) {
    if (!fanOn) {
      actuatorController->turnFanOn();
      fanOn = true;
    }
  } else if (currentTemperature <= tempThreshold - tempHysteresis) {
    if (fanOn) {
      actuatorController->turnFanOff();
      fanOn = false;
    }
  }

  // Control del ventilador basado en la humedad
  if (currentHumidity >= humThreshold) {
    if (!fanOn) {
      actuatorController->turnFanOn();
      fanOn = true;
    }
  } else if (currentHumidity <= humThreshold - humHysteresis) {
    if (fanOn) {
      actuatorController->turnFanOff();
      fanOn = false;
    }
  }

  // Control del aspersor basado en la humedad
  if (currentHumidity <= humThreshold - humHysteresis) {
    if (!sprayerOn) {
      actuatorController->turnSprayerOn();
      sprayerOn = true;
    }
  } else {
    if (sprayerOn) {
      actuatorController->turnSprayerOff();
      sprayerOn = false;
    }
  }

  // Control del aspersor basado en la temperatura extrema
  if (currentTemperature >= TEMP_EXTREME_THRESHOLD) {
    if (sprayerOn) {
      if (timerManager.shouldRun("sprayerOn")) { // Apagar después de 5 minutos
        actuatorController->turnSprayerOff();
        sprayerOn = false;
        timerManager.start("sprayerOff"); // Iniciar el temporizador de apagado
      }
    } else {
      if (timerManager.shouldRun("sprayerOff")) { // Encender después de 10 minutos
        actuatorController->turnSprayerOn();
        sprayerOn = true;
        timerManager.start("sprayerOn"); // Iniciar el temporizador de encendido
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
    if (!pump1On) {
      actuatorController->turnPump1On();
      pump1On = true;
    }
  } else if (soilMoisture1 > maxSoilMoisture) {
    if (pump1On) {
      actuatorController->turnPump1Off();
      pump1On = false;
    }
  }

  // Control de la bomba 2
  if (soilMoisture2 < minSoilMoisture) {
    if (!pump2On) {
      actuatorController->turnPump2On();
      pump2On = true;
    }
  } else if (soilMoisture2 > maxSoilMoisture) {
    if (pump2On) {
      actuatorController->turnPump2Off();
      pump2On = false;
    }
  }

  // Control de la bomba 3
  if (soilMoisture3 < minSoilMoisture) {
    if (!pump3On) {
      actuatorController->turnPump3On();
      pump3On = true;
    }
  } else if (soilMoisture3 > maxSoilMoisture) {
    if (pump3On) {
      actuatorController->turnPump3Off();
      pump3On = false;
    }
  }
}

// Función para actualizar la pantalla
void updateDisplay() {
  display->clear();
  if (currentScreen == 0) {
    String tempMessage = "Temp: " + String(currentTemperature) + "C";
    String humMessage = "Hum: " + String(currentHumidity) + "%";
    display->showMessage(0, 0, tempMessage.c_str());
    display->showMessage(1, 0, humMessage.c_str());
    currentScreen = 1;
  } else {
    String fanMessage = "Fan: " + String(fanOn ? "On" : "Off");
    String pumpMessage1 = "P1:" + String(pump1On ? "On" : "Off") + " P2:" + String(pump2On ? "On" : "Off");
    String pumpMessage2 = "P3:" + String(pump3On ? "On" : "Off");
    display->showMessage(0, 0, fanMessage.c_str());
    display->showMessage(1, 0, pumpMessage1.c_str());
    display->showMessage(1, 8, pumpMessage2.c_str());
    currentScreen = 0;
  }
}
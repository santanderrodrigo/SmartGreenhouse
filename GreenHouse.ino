#include "DHTSensor.h"
#include "LCDDisplay.h"
#include "ActuatorController.h"
#include "SoilMoistureSensor.h"
#include "ControlProfile.h"
#include "TaskScheduler.h"
#include "TimerManager.h"
#include "EthernetController.h"

// Perfiles de configuración de pines para Arduino Uno y Arduino Mega
#if defined(ARDUINO_AVR_MEGA2560)

  //Sensores
  #define DHT_PIN 14
  #define HUMEDITY_SOIL_1_PIN A0
  #define HUMEDITY_SOIL_2_PIN A1
  #define HUMEDITY_SOIL_3_PIN A2

  //Actuadores
  #define FAN_PIN 25
  #define PUMP_1_PIN 22
  #define PUMP_2_PIN 23
  #define PUMP_3_PIN 24
  #define SPRAYER_PIN 26

  //LCD Display
  #define RW_PIN 6
  #define RS_PIN 7
  #define E_PIN 8
  #define D4_PIN 9
  #define D5_PIN 11
  #define D6_PIN 12
  #define D7_PIN 13

  #define ENC28J60_CS_PIN 53

#else
  #error "Esta configuración está diseñada para Arduino Arduino Mega. Por favor, selecciona esta placa en el menú Herramientas > Placa."
#endif

#define DHTTYPE DHT22
#define TEMP_INTERVAL_SCHEDULER 5000
#define DISPLAY_INTERVAL_SCHEDULER 5000
#define TEMP_EXTREME_THRESHOLD 38.0
#define SPRAYER_ON_DURATION 300000
#define SPRAYER_OFF_DURATION 600000

#define TEMP_THRESHOLD_VALUE 25.0
#define HUM_THRESHOLD_VALUE 50.0
#define TEMP_HYSTERESIS_VALUE 2.0
#define HUM_HYSTERESIS_VALUE 2.0
#define MIN_TEMP_VALUE 15.0
#define MAX_TEMP_VALUE 30.0
#define MIN_HUM_VALUE 30.0
#define MAX_HUM_VALUE 60.0
#define SOIL_MOISTURE_THERSHOLD_VALUE 20.0
#define MIN_SOIL_MOISTURE_VALUE 20.0
#define MAX_SOIL_MOISTURE_VALUE 50.0
#define INFILTRATION_TIME_VALUE 600
#define IRRIGATION_TIME_VALUE 1200

Sensor* dhtSensor;
SoilMoistureSensor* soilSensor1;
SoilMoistureSensor* soilSensor2;
SoilMoistureSensor* soilSensor3;
Display* display;
ActuatorController* actuatorController;
ControlProfile* controlProfile;

float currentTemperature = NAN;
float currentHumidity = NAN;

bool actuatorsState[5] = {false, false, false, false, false};
float configParameters[8] = {
    TEMP_THRESHOLD_VALUE, HUM_THRESHOLD_VALUE, TEMP_HYSTERESIS_VALUE, HUM_HYSTERESIS_VALUE,
    MIN_SOIL_MOISTURE_VALUE, MAX_SOIL_MOISTURE_VALUE, INFILTRATION_TIME_VALUE, IRRIGATION_TIME_VALUE
};

int currentScreen = 0;

TaskScheduler dhtScheduler(TEMP_INTERVAL_SCHEDULER);
TaskScheduler displayScheduler(DISPLAY_INTERVAL_SCHEDULER);
TimerManager timerManager;

EthernetController ethernetController;

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

    actuatorController->turnOn(FAN);
    actuatorController->turnOn(PUMP1);
    actuatorController->turnOn(PUMP2);
    actuatorController->turnOn(PUMP3);
    actuatorController->turnOn(SPRAYER);

    controlProfile = new ControlProfile(TEMP_THRESHOLD_VALUE, HUM_THRESHOLD_VALUE, TEMP_HYSTERESIS_VALUE, HUM_HYSTERESIS_VALUE,
                                        MIN_TEMP_VALUE, MAX_TEMP_VALUE, MIN_HUM_VALUE, MAX_HUM_VALUE,
                                        MIN_SOIL_MOISTURE_VALUE, MAX_SOIL_MOISTURE_VALUE, SOIL_MOISTURE_THERSHOLD_VALUE,
                                        INFILTRATION_TIME_VALUE, IRRIGATION_TIME_VALUE);

    timerManager.addTimer("sprayerOn", SPRAYER_ON_DURATION);
    timerManager.addTimer("sprayerOff", SPRAYER_OFF_DURATION);

    ethernetController.setup();
    ethernetController.setParameters(&currentTemperature, &currentHumidity, actuatorsState, configParameters);
}

void loop() {
    if (dhtScheduler.shouldRun()) {
        currentTemperature = checkTemperature();
        currentHumidity = checkHumidity();
    }

    if (displayScheduler.shouldRun()) {
        updateDisplay();
        controlTemperatureHumidity();
        controlSoilMoisture();
    }

    updateActuatorsState();
    ethernetController.loop();
}

float checkTemperature() {
    float temperature = dhtSensor->readTemperature();

    if (isnan(temperature)) {
        Serial.println("¡Error al leer la temperatura del sensor!");
        return NAN;
    }

    Serial.print("Temperatura: ");
    Serial.print(temperature);
    Serial.println(" *C");

    return temperature;
}

float checkHumidity() {
    float humidity = dhtSensor->readHumidity();

    if (isnan(humidity)) {
        Serial.println("¡Error al leer la humedad del sensor!");
        return NAN;
    }

    Serial.print("Humedad: ");
    Serial.print(humidity);
    Serial.println(" %");

    return humidity;
}

void controlTemperatureHumidity() {
    float tempThershold = controlProfile->getTempThreshold();
    float humThreshold = controlProfile->getHumThreshold();
    float tempHysteresis = controlProfile->getTempHysteresis();
    float humHysteresis = controlProfile->getHumHysteresis();

    if (currentTemperature > tempThershold && !actuatorController->isOn(FAN)) {
        actuatorController->turnOn(FAN);
    } else if (currentTemperature < (tempThershold - tempHysteresis) && actuatorController->isOn(FAN)) {
        actuatorController->turnOff(FAN);
    }

    if (currentHumidity < humThreshold && !actuatorController->isOn(SPRAYER)) {
        actuatorController->turnOn(SPRAYER);
    } else if (currentHumidity > (humThreshold - humHysteresis) && actuatorController->isOn(SPRAYER)) {
        actuatorController->turnOff(SPRAYER);
    }
}

void controlSoilMoisture() {
    float soilMoistureThreshold = controlProfile->getSoilMoistureThreshold();
    float soilMoistureHysteresis = 2;
    unsigned long infiltrationTime = controlProfile->getInfiltrationTime();
    unsigned long irrigationTime = controlProfile->getIrrigationTime();

    float soilMoisture1 = soilSensor1->readValue();
    float soilMoisture2 = soilSensor2->readValue();
    float soilMoisture3 = soilSensor3->readValue();

    Serial.print("Humedad del suelo 1: ");
    Serial.println(soilMoisture1);

    Serial.print("Humedad del suelo 2: ");
    Serial.println(soilMoisture2);

    Serial.print("Humedad del suelo 3: ");
    Serial.println(soilMoisture3);

    if (soilMoisture1 < soilMoistureThreshold && !actuatorController->isOn(PUMP1)) {
        actuatorController->turnOn(PUMP1);
        timerManager.start("pump1");
    } else if (soilMoisture1 > (soilMoistureThreshold + soilMoistureHysteresis) && actuatorController->isOn(PUMP1)) {
        actuatorController->turnOff(PUMP1);
        timerManager.stop("pump1");
    }

    if (soilMoisture2 < soilMoistureThreshold && !actuatorController->isOn(PUMP2)) {
        actuatorController->turnOn(PUMP2);
        timerManager.start("pump2");
    } else if (soilMoisture2 > (soilMoistureThreshold + soilMoistureHysteresis) && actuatorController->isOn(PUMP2)) {
        actuatorController->turnOff(PUMP2);
        timerManager.stop("pump2");
    }

    if (soilMoisture3 < soilMoistureThreshold && !actuatorController->isOn(PUMP3)) {
        actuatorController->turnOn(PUMP3);
        timerManager.start("pump3");
    } else if (soilMoisture3 > (soilMoistureThreshold + soilMoistureHysteresis) && actuatorController->isOn(PUMP3)) {
        actuatorController->turnOff(PUMP3);
        timerManager.stop("pump3");
    }
}

void updateDisplay() {
    display->clear();

    Serial.println("Actualizando display..");

    if (currentScreen == 0) {
        if (isnan(currentTemperature)) {
            display->showMessage(0, 0, "Temp: --");
        } else {
            display->showTemperature(currentTemperature);
        }

        if (isnan(currentHumidity)) {
            display->showMessage(1, 0, "Hum: --");
        } else {
            display->showHumidity(currentHumidity);
        }

        currentScreen = 1;
    } else if (currentScreen == 1) {
        if (actuatorController->isOn(FAN)) {
            display->showMessage(0, 0, "Fan: ON");
        } else {
            display->showMessage(0, 0, "Fan: OFF");
        }

        if (actuatorController->isOn(SPRAYER)) {
            display->showMessage(1, 0, "Sprayer: ON");
        } else {
            display->showMessage(1, 0, "Sprayer: OFF");
        }

        currentScreen = 2;
    } else {
        if (actuatorController->isOn(PUMP1)) {
            display->showMessage(0, 0, "Pump1: ON");
        } else {
            display->showMessage(0, 0, "Pump1: OFF");
        }

        if (actuatorController->isOn(PUMP2)) {
            display->showMessage(1, 0, "Pump2: ON");
        } else {
            display->showMessage(1, 0, "Pump2: OFF");
        }

        if (actuatorController->isOn(PUMP3)) {
            display->showMessage(0, 1, "Pump3: ON");
        } else {
            display->showMessage(0, 1, "Pump3: OFF");
        }

        currentScreen = 0;
    }

    Serial.println("Fan: ");
    Serial.println(actuatorController->isOn(FAN) ? "ON" : "OFF");
    Serial.println("Sprayer: ");
    Serial.println(actuatorController->isOn(SPRAYER) ? "ON" : "OFF");
    Serial.println("Pump1: ");
    Serial.println(actuatorController->isOn(PUMP1) ? "ON" : "OFF");
    Serial.println("Pump2: ");
    Serial.println(actuatorController->isOn(PUMP2) ? "ON" : "OFF");
    Serial.println("Pump3: ");
    Serial.println(actuatorController->isOn(PUMP3) ? "ON" : "OFF");
}

void updateActuatorsState() {
    actuatorsState[0] = actuatorController->isOn(FAN);
    actuatorsState[1] = actuatorController->isOn(SPRAYER);
    actuatorsState[2] = actuatorController->isOn(PUMP1);
    actuatorsState[3] = actuatorController->isOn(PUMP2);
    actuatorsState[4] = actuatorController->isOn(PUMP3);
}
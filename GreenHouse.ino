#include "DHTSensor.h" // Incluímos la clase DHTSensor
#include "LCDDisplay.h" // Incluímos la clase LCDDisplay
#include "ActuatorController.h" // Incluímos la clase ActuatorController
#include "SoilMoistureSensor.h" // Incluímos la clase SoilMoistureSensor
#include "ControlProfile.h" // Include the ControlProfile header
#include "TaskScheduler.h" // Incluímos la clase TaskScheduler


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

const float TEMP_THRESHOLD = 25.0;  // Umbral de temperatura en °C
const float HUM_THRESHOLD = 40.0;   // Umbral de humedad en %

const float TEMP_HYSTERESIS = 2.0;  // Histéresis de temperatura en °C
const float HUM_HYSTERESIS = 2.0;   // Histéresis de humedad en %

const unsigned long TEMP_INTERVAL = 2000; // Intervalo para la verificación de temperatura en milisegundos
const unsigned long HUM_INTERVAL = 2000;  // Intervalo para la verificación de humedad en milisegundos


Sensor* dhtSensor; // Sensor DHT11
SoilMoistureSensor* soilSensor1;
SoilMoistureSensor* soilSensor2;
SoilMoistureSensor* soilSensor3;
Display* display; // Pantalla LCD
ActuatorController* actuatorController; // Controlador de actuadores
ControlProfile* controlProfile; // Perfil de control


unsigned long previousTempMillis = 0; // Tiempo anterior para la verificación de temperatura
unsigned long previousHumMillis = 0; // Tiempo anterior para la verificación de humedad

float currentTemperature = 0.0; // Variable para almacenar la temperatura actual
float currentHumidity = 0.0; // Variable para almacenar la humedad actual


bool fanOn = false; // Estado del ventilador
bool pump1On = false; // Estado de la bomba del circuito 1
bool pump2On = false; // Estado de la bomba del circuito 2
bool pump3On = false; // Estado de la bomba del circuito 3
bool sprayerOn = false; // Estado del aspersor de humedad

TaskScheduler dhtScheduler(TEMP_INTERVAL);

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
}

// Bucle principal, se ejecuta continuamente
void loop() {

  // Verificar la temperatura y la humedad, si shouldRun() es verdadero
  if (dhtScheduler.shouldRun()) {
    currentTemperature = checkTemperature();
    currentHumidity = checkHumidity();
  }

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
#include "DHTSensor.h" // Incluímos la clase DHTSensor
#include "LCDDisplay.h" // Incluímos la clase LCDDisplay
#include "ActuatorController.h" // Incluímos la clase ActuatorController

// Perfiles de configuración de pines para Arduino Uno y Arduino Mega
#if defined(ARDUINO_AVR_UNO)
  #define DHTPIN 13          // Pin donde está conectado el DHT11 -> pin 13 en uno
  #define FAN_PIN 6          // Pin para el motor del ventilador
  #define PUMP_PIN 7         // Pin para la bomba de agua
  #define GREEN_LED_PIN 11   // Pin para el LED verde
  #define RED_LED_PIN 12     // Pin para el LED rojo
  #define RW_PIN 9           // Pin RW de la pantalla LCD
  #define RS_PIN 10          
  #define E_PIN 8
  #define D4_PIN 5
  #define D5_PIN 4
  #define D6_PIN 3
  #define D7_PIN 2
#elif defined(ARDUINO_AVR_MEGA2560)
  #define DHTPIN 14          // Pin donde está conectado el DHT11 --> en mega usamos el puerto 14
  #define FAN_PIN 6          // Pin para el motor del ventilador
  #define PUMP_PIN 7         // Pin para la bomba de agua
  #define GREEN_LED_PIN 11   // Pin para el LED verde
  #define RED_LED_PIN 12     // Pin para el LED rojo
  #define RW_PIN 9           // Pin RW de la pantalla LCD
  #define RS_PIN 10          
  #define E_PIN 8
  #define D4_PIN 5
  #define D5_PIN 4
  #define D6_PIN 3
  #define D7_PIN 2
#else
  #error "Esta configuración está diseñada para Arduino Uno o Arduino Mega. Por favor, selecciona una de estas placas en el menú Herramientas > Placa."
#endif

#define DHTTYPE DHT11    // DHT 11 or DHT 22

const float TEMP_THRESHOLD = 25.0;  // Umbral de temperatura en °C
const float HUM_THRESHOLD = 40.0;   // Umbral de humedad en %

const float TEMP_HYSTERESIS = 0.0;  // Histéresis de temperatura en °C
const float HUM_HYSTERESIS = 0.0;   // Histéresis de humedad en %

const unsigned long TEMP_INTERVAL = 2000; // Intervalo para la verificación de temperatura en milisegundos
const unsigned long HUM_INTERVAL = 2000;  // Intervalo para la verificación de humedad en milisegundos


Sensor* sensor; // Sensor DHT11
Display* display; // Pantalla LCD
ActuatorController* actuatorController; // Controlador de actuadores
unsigned long previousTempMillis = 0; // Tiempo anterior para la verificación de temperatura
unsigned long previousHumMillis = 0; // Tiempo anterior para la verificación de humedad

bool fanOn = false; // Estado del ventilador
bool pumpOn = false; // Estado de la bomba

// Función de configuración, se ejecuta una vez al inicio
void setup() {
  Serial.begin(9600); // Inicializamos el puerto serie en 9600 baudios
  
  sensor = new DHTSensor(DHTPIN, DHTTYPE);  // Configuración del sensor DHT11 e instancia del objeto
  sensor->begin(); // Inicializamos el sensor
  
  display = new LCDDisplay(RS_PIN, RW_PIN, E_PIN, D4_PIN, D5_PIN, D6_PIN, D7_PIN); // Configuración de pines de la pantalla LCD

  display->begin(); // Inicializamos la pantalla
  
  actuatorController = new ActuatorController(FAN_PIN, PUMP_PIN, GREEN_LED_PIN, RED_LED_PIN);
  actuatorController->begin(); // Inicializamos el controlador de actuadores

}

// Bucle principal, se ejecuta continuamente
void loop() {
  unsigned long currentMillis = millis(); // Obtenemos el tiempo actual en milisegundos

  // Verificamos si ha pasado el intervalo de tiempo para la temperatura
  if (currentMillis - previousTempMillis >= TEMP_INTERVAL) {
    previousTempMillis = currentMillis;
    checkTemperature();
  }

  // Verificamos si ha pasado el intervalo de tiempo para la humedad
  if (currentMillis - previousHumMillis >= HUM_INTERVAL) {
    previousHumMillis = currentMillis;
    checkHumidity();
  }
}

// Función para verificar la temperatura
void checkTemperature() {
  float temperature = sensor->readTemperature(); // Leemos la temperatura del sensor

  // Verificamos si la lectura de la temperatura es inválida
  if (isnan(temperature)) {
    Serial.println("¡Error al leer la temperatura del sensor!");
    return;
  }

  // Mostramos la temperatura en el monitor serie
  Serial.print("Temperatura: ");
  Serial.print(temperature);
  Serial.println(" *C");

  display->showTemperature(temperature); // Mostramos la temperatura en la pantalla

  // Verificamos si el ventilador está encendido
  if (fanOn) {
    // Si la temperatura es menor o igual al umbral menos la histéresis, apagamos el ventilador
    if (temperature <= TEMP_THRESHOLD - TEMP_HYSTERESIS) {
      actuatorController->turnFanOff(); // Apagamos el ventilador
      fanOn = false;
    }
  } else {
    // Si la temperatura es mayor o igual al umbral, encendemos el ventilador
    if (temperature >= TEMP_THRESHOLD) {
      actuatorController->turnFanOn(); // Encendemos el ventilador
      fanOn = true;
    }
  }
}

// Función para verificar la humedad
void checkHumidity() {
  float humidity = sensor->readHumidity(); // Leemos la humedad del sensor

  // Verificamos si la lectura de la humedad es inválida
  if (isnan(humidity)) {
    Serial.println("¡Error al leer la humedad del sensor!");
    return;
  }

  // Mostramos la humedad en el monitor serie
  Serial.print("Humedad: ");
  Serial.print(humidity);
  Serial.println(" %");

  display->showHumidity(humidity); // Mostramos la humedad en la pantalla

  // Verificamos si la bomba está encendida
  if (pumpOn) {
    // Si la humedad es mayor o igual al umbral más la histéresis, apagamos la bomba
    if (humidity >= HUM_THRESHOLD + HUM_HYSTERESIS) {
      actuatorController->turnPumpOff(); // Apagamos la bomba
      Serial.print("Apagamos la bomba");
      pumpOn = false;
    }
  } else {
    // Si la humedad es menor o igual al umbral, encendemos la bomba
    if (humidity <= HUM_THRESHOLD) {
      actuatorController->turnPumpOn(); // Encendemos la bomba
      Serial.print("Encendemos la bomba");
      pumpOn = true;
    }
  }
}
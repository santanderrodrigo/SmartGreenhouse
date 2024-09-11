#include "DHTSensor.h" // Incluímos la clase DHTSensor
#include "LCDDisplay.h" // Incluímos la clase LCDDisplay
#include "ActuatorController.h" // Incluímos la clase ActuatorController

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

// Función de configuración, se ejecuta una vez al inicio
void setup() {
  Serial.begin(9600); // Inicializamos el puerto serie en 9600 baudios
  
  sensor = new DHTSensor(DHTPIN, DHTTYPE);  // Configuración del sensor DHT11 e instancia del objeto
  sensor->begin(); // Inicializamos el sensor
  
  // El pin RW no se usa, así que se conecta a GND, porque la pantalla permanece en modo escritura
  pinMode(RW_PIN, OUTPUT);   // Seteamos el pin RW como salida y lo ponemos en LOW
  display = new LCDDisplay(10, 8, 5, 4, 3, 2); // Configuración de pines de la pantalla
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
      pumpOn = false;
    }
  } else {
    // Si la humedad es menor o igual al umbral, encendemos la bomba
    if (humidity <= HUM_THRESHOLD) {
      actuatorController->turnPumpOn(); // Encendemos la bomba
      pumpOn = true;
    }
  }
}
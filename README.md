# 🌿 SmartGreenhouse Control System

Este proyecto es un sistema de control automatizado para un invernadero, diseñado para mantener las condiciones óptimas de **temperatura, humedad del aire y humedad del suelo** utilizando sensores y actuadores. Además, incluye funcionalidad de **comunicación Ethernet** para monitoreo y control remoto.

## 🌟 Características

- **Control automático de temperatura y humedad** del aire.
- **Control de la humedad del suelo** para mantener niveles adecuados.
- Gestión de **ventiladores, aspersores y bombas de agua**.
- Pantalla **LCD 2x16** para mostrar información en tiempo real.
- Comunicación **Ethernet** para monitoreo y control remoto desde cualquier lugar.

## 🛠️ Requisitos

- **Hardware:**
  - Arduino o plataforma compatible.
  - Sensor de temperatura y humedad (DHT11, DHT22, etc.).
  - Sensores de humedad del suelo (capacitivo).
  - Actuadores (ventiladores, aspersores, bombas de agua).
  - Pantalla LCD **2x16**.
  - Módulo Ethernet (**enc28j60**).
- **Librerías de Arduino:**
  - `DHT.h`
  - `LiquidCrystal.h`
  - `SPI.h`
  - `Ethernet.h`
  - `LinkedList.h`

## 🚀 Instalación y Configuración

1. Clona este repositorio en tu máquina local:
   ```bash
   git clone https://github.com/santanderrodrigo/SmartGreenhouse.git
   ```
2. Abre el archivo **GreenHouse.ino** en el **Arduino IDE**.
3. Asegúrate de tener instaladas las librerías necesarias (puedes instalarlas desde el **Gestor de Librerías** del IDE).
4. Conecta los sensores, actuadores y el módulo Ethernet a tu Arduino según el esquema de conexión.
5. Sube el código a tu Arduino utilizando el **Arduino IDE**.

## ⚙️ Esquema de Conexión

A continuación, se detalla cómo conectar los componentes al Arduino:

- **Sensor de temperatura y humedad (DHT11/DHT22)**: Pin digital `2`.
- **Sensor de humedad del suelo 1**: Pin analógico `A0`.
- **Sensor de humedad del suelo 2**: Pin analógico `A1`.
- **Sensor de humedad del suelo 3**: Pin analógico `A2`.
- **Ventilador**: Pin digital `3`.
- **Aspersor**: Pin digital `4`.
- **Bomba de agua 1**: Pin digital `5`.
- **Bomba de agua 2**: Pin digital `6`.
- **Bomba de agua 3**: Pin digital `7`.
- **Pantalla LCD**: Pines digitales `8, 9, 10, 11, 12, 13`.
- **Módulo Ethernet (enc28j60)**: Pines SPI (`10, 11, 12, 13`).

## 🖥️ Uso

El sistema se ejecuta continuamente en el bucle principal de Arduino, realizando las siguientes acciones:

- **controlTemperatureHumidity()**: Controla la temperatura y la humedad del aire, encendiendo o apagando el ventilador y el aspersor según sea necesario.
- **controlSoilMoisture()**: Controla la humedad del suelo, encendiendo o apagando las bombas de agua según sea necesario.
- **updateDisplay()**: Actualiza la pantalla LCD con la información actual de temperatura, humedad y estado de los actuadores.
- **EthernetController**: Gestiona la comunicación Ethernet para monitoreo y control remoto.

## 🤝 Contribuciones

Las contribuciones son bienvenidas. Si deseas contribuir, por favor sigue estos pasos:

1. Haz un **fork** del repositorio.
2. Crea una nueva rama:
   ```bash
   git checkout -b feature/nueva-funcionalidad
   ```
3. Realiza tus cambios y haz commit:
   ```bash
   git commit -am 'Añadir nueva funcionalidad'
   ```
4. Sube tus cambios a la rama:
   ```bash
   git push origin feature/nueva-funcionalidad
   ```
5. Abre un **Pull Request**.

## 📄 Licencia

Este proyecto está licenciado bajo la **Licencia MIT**. Consulta el archivo `LICENSE` para más detalles.

## 📬 Contacto

Para cualquier consulta o sugerencia, no dudes en contactarme

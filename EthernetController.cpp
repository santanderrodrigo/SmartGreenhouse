#include "EthernetController.h"
#include <SPI.h>

EthernetController::EthernetController(uint8_t* mac, IPAddress ip, uint8_t sckPin, uint8_t misoPin, uint8_t mosiPin, uint8_t csPin)
    : macAddress(mac), ipAddress(ip), sckPin(sckPin), misoPin(misoPin), mosiPin(mosiPin), csPin(csPin), server(80) {}

void EthernetController::begin() {
    // Configurar los pines SPI
    pinMode(sckPin, OUTPUT);
    pinMode(misoPin, INPUT);
    pinMode(mosiPin, OUTPUT);
    pinMode(csPin, OUTPUT);

    // Inicializar Ethernet
    Ethernet.begin(macAddress, ipAddress);
    Serial.println("Ethernet initialized");
}

void EthernetController::handleClient() {
    EthernetClient client = server.available();
    if (client) {
        Serial.println("New client connected");
        while (client.connected()) {
            if (client.available()) {
                char c = client.read();
                Serial.write(c);
                // Handle client request here
            }
        }
        client.stop();
        Serial.println("Client disconnected");
    }
}
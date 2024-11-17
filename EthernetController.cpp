#include "EthernetController.h"

EthernetController::EthernetController(int csPin)
    : _csPin(csPin), server(80) {}

void EthernetController::begin() {
    // Inicializar SPI
    SPI.begin();

    // Inicializar ENC28J60
    Ethernet.init(_csPin); // Inicializa el ENC28J60 con el pin CS

    // Configurar Ethernet con IP estática
    Ethernet.begin(_mac, _ip, _dns, _gateway, _subnet);

    server.begin();
}

void EthernetController::handleClient() {
    EthernetClient client = server.available();
    if (client) {
        // Procesar la solicitud del cliente
    }
}
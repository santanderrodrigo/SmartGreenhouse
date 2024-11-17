#ifndef ETHERNETCONTROLLER_H
#define ETHERNETCONTROLLER_H

#include <SPI.h>
#include <UIPEthernet.h> // Cambiado para usar UIPEthernet

class EthernetController {
public:
    EthernetController(int csPin);
    void begin();
    void handleClient();

private:
    byte _mac[6] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
    IPAddress _ip = IPAddress(192, 168, 1, 177);
    IPAddress _gateway = IPAddress(192, 168, 1, 1);
    IPAddress _subnet = IPAddress(255, 255, 255, 0);
    IPAddress _dns = IPAddress(8, 8, 8, 8);
    int _csPin;
    EthernetServer server;
};

#endif // ETHERNETCONTROLLER_H
#ifndef ETHERNETCONTROLLER_H
#define ETHERNETCONTROLLER_H

#include <UIPEthernet.h>

class EthernetController {
public:
    EthernetController(uint8_t* mac, IPAddress ip, uint8_t sckPin, uint8_t misoPin, uint8_t mosiPin, uint8_t csPin);
    void begin();
    void handleClient();

private:
    uint8_t* macAddress;
    IPAddress ipAddress;
    uint8_t sckPin;
    uint8_t misoPin;
    uint8_t mosiPin;
    uint8_t csPin;
    EthernetServer server;
};

#endif // ETHERNETCONTROLLER_H
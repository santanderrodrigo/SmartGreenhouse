#ifndef ETHERNETCONTROLLER_H
#define ETHERNETCONTROLLER_H

#define DISABLE_ETHERNET_CHECKSUM

#include <UIPEthernet.h>

class EthernetController {
public:
    EthernetController();
    void setup();
    void loop();
    void setParameters(float* temperature, float* humidity, bool* actuators, float* configParams);

private:
    byte mac[6] = { 0xDD, 0xDD, 0xDD, 0x00, 0x01, 0x05 };
    IPAddress ip = IPAddress(192, 168, 28, 47);
    EthernetServer server = EthernetServer(80);
    const int ledPin = 2;
    String EstadoLed = "OFF";

    float* currentTemperature;
    float* currentHumidity;
    bool* actuatorsState;
    float* configParameters;

    void homePage(EthernetClient &client);
};

#endif // ETHERNETCONTROLLER_H
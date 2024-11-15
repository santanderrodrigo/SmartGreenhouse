#ifndef ETHERNETCONTROLLER_H
#define ETHERNETCONTROLLER_H

#include <SPI.h>
#include <Ethernet.h>

class EthernetController {
public:
  EthernetController(byte* mac, IPAddress ip, int port);
  void begin();
  void handleClient();

private:
  EthernetServer server;
};

#endif
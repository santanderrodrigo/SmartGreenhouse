#ifndef ETHERNETCONTROLLER_H
#define ETHERNETCONTROLLER_H

#include <SPI.h>
#include <Ethernet.h>
#include <LinkedList.h>
#include "CommunicationController.h"

class EthernetController : public CommunicationController {
public:
  EthernetController(byte* mac, IPAddress ip, int port);
  void begin() override;
  void handleClient() override;
  void registerEndpoint(const String& endpoint, EndpointHandler handler) override;

private:
  EthernetServer server;
  LinkedList<String> endpoints;
  LinkedList<EndpointHandler> handlers;
  void serveClient();
};

#endif
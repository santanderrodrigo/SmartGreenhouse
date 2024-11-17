#ifndef COMMUNICATIONCONTROLLER_H
#define COMMUNICATIONCONTROLLER_H

#include <Ethernet.h>
#include <LinkedList.h>

typedef void (*EndpointHandler)(EthernetClient&);

class CommunicationController {
public:
  virtual void begin() = 0;
  virtual void handleClient() = 0;
  virtual void registerEndpoint(const String& endpoint, EndpointHandler handler) = 0;
};

#endif
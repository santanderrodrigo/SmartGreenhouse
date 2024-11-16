#include "EthernetController.h"

EthernetController::EthernetController(byte* mac, IPAddress ip, int port)
  : server(port) {
  Ethernet.begin(mac, ip);
}

void EthernetController::begin() {
  server.begin();
}

void EthernetController::handleClient() {
  serveClient();
}

void EthernetController::registerEndpoint(const String& endpoint, EndpointHandler handler) {
  endpoints.add(endpoint);
  handlers.add(handler);
}

void EthernetController::serveClient() {
  EthernetClient client = server.available();
  if (client) {
    if (client.available()) {
      String request = client.readStringUntil('\r');
      client.flush();

      // Parse the request to get the endpoint
      int start = request.indexOf(' ') + 1;
      int end = request.indexOf(' ', start);
      String endpoint = request.substring(start, end);

      // Find and call the handler for the endpoint
      for (int i = 0; i < endpoints.size(); i++) {
        if (endpoints.get(i) == endpoint) {
          handlers.get(i)(client);
          return;
        }
      }

      // Handle 404 Not Found
      client.println("HTTP/1.1 404 Not Found");
      client.println("Content-Type: text/plain");
      client.println("Connection: close");
      client.println();
      client.println("404 Not Found");
    }
    client.stop();
  }
}
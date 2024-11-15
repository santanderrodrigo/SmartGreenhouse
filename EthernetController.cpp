#include "EthernetController.h"

EthernetController::EthernetController(byte* mac, IPAddress ip, int port) 
  : server(port) {
  Ethernet.begin(mac, ip);
}

void EthernetController::begin() {
  server.begin();
  Serial.print("Servidor iniciado en la IP: ");
  Serial.println(Ethernet.localIP());
}

void EthernetController::handleClient() {
  EthernetClient client = server.available();
  if (client) {
    Serial.println("Nuevo cliente");
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println("Connection: close");
            client.println();
            client.println("<!DOCTYPE HTML>");
            client.println("<html>");
            client.println("<h1>Control de Invernadero</h1>");
            client.println("<p><a href=\"/FAN_ON\">Encender Ventilador</a></p>");
            client.println("<p><a href=\"/FAN_OFF\">Apagar Ventilador</a></p>");
            client.println("</html>");
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }

        if (currentLine.endsWith("GET /FAN_ON")) {
          digitalWrite(FAN_PIN, HIGH);
        }
        if (currentLine.endsWith("GET /FAN_OFF")) {
          digitalWrite(FAN_PIN, LOW);
        }
      }
    }
    client.stop();
    Serial.println("Cliente desconectado");
  }
}
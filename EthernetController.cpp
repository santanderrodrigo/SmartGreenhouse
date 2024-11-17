#include "EthernetController.h"

EthernetController::EthernetController() {
    // Constructor
}

void EthernetController::setup() {
    Serial.begin(9600);
    Serial.println("Test del Modulo ENC28J60 con UIPEthernet");

    // Inicializar la conexión Ethernet
    Ethernet.begin(mac, ip);

    if (Ethernet.localIP() == IPAddress(0, 0, 0, 0)) {
        Serial.println("Error al obtener la dirección IP");
    } else {
        Serial.print("Dirección IP asignada: ");
        Serial.println(Ethernet.localIP());
    }

    // Iniciar el servidor
    server.begin();
    Serial.println("Servidor HTTP iniciado");

    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW);
}

void EthernetController::setParameters(float* temperature, float* humidity, bool* actuators, float* configParams) {
    currentTemperature = temperature;
    currentHumidity = humidity;
    actuatorsState = actuators;
    configParameters = configParams;
}

void EthernetController::homePage(EthernetClient &client) {
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("Connection: close");
    client.println();

    client.println("<html><head><title>Smart GreenHouse</title><meta charset='UTF-8'>");
    client.println("<style>");
    client.println("body { font-family: Arial, sans-serif; text-align: center; }");
    client.println("h1 { color: #333; }");
    client.println("table { margin: 0 auto; border-collapse: collapse; }");
    client.println("td, th { border: 1px solid #ddd; padding: 8px; }");
    client.println("th { background-color: #f2f2f2; }");
    client.println("</style>");
    client.println("<meta http-equiv='refresh' content='10'>");
    client.println("</head><body>");
    client.println("<h1>Estado del Sistema</h1>");

    client.println("<table>");
    client.println("<tr><th>Parámetro</th><th>Valor</th></tr>");
    client.print("<tr><td>Temperatura</td><td>");
    client.print(*currentTemperature);
    client.println(" *C</td></tr>");

    client.print("<tr><td>Humedad</td><td>");
    client.print(*currentHumidity);
    client.println(" %</td></tr>");
    client.println("</table>");

    client.println("<h2>Estado de los Actuadores</h2>");
    client.println("<table>");
    client.println("<tr><th>Actuador</th><th>Estado</th></tr>");
    client.print("<tr><td>Ventilador</td><td>");
    client.print(actuatorsState[0] ? "ON" : "OFF");
    client.println("</td></tr>");

    client.print("<tr><td>Aspersor</td><td>");
    client.print(actuatorsState[1] ? "ON" : "OFF");
    client.println("</td></tr>");

    client.print("<tr><td>Bomba 1</td><td>");
    client.print(actuatorsState[2] ? "ON" : "OFF");
    client.println("</td></tr>");

    client.print("<tr><td>Bomba 2</td><td>");
    client.print(actuatorsState[3] ? "ON" : "OFF");
    client.println("</td></tr>");

    client.print("<tr><td>Bomba 3</td><td>");
    client.print(actuatorsState[4] ? "ON" : "OFF");
    client.println("</td></tr>");
    client.println("</table>");

    client.println("<h2>Parámetros de Configuración</h2>");
    client.println("<table>");
    client.println("<tr><th>Parámetro</th><th>Valor</th></tr>");
    client.print("<tr><td>Umbral de Temperatura</td><td>");
    client.print(configParameters[0]);
    client.println(" *C</td></tr>");

    client.print("<tr><td>Umbral de Humedad</td><td>");
    client.print(configParameters[1]);
    client.println(" %</td></tr>");

    client.print("<tr><td>Histeresis de Temperatura</td><td>");
    client.print(configParameters[2]);
    client.println(" *C</td></tr>");

    client.print("<tr><td>Histeresis de Humedad</td><td>");
    client.print(configParameters[3]);
    client.println(" %</td></tr>");

    client.print("<tr><td>Humedad del Suelo Mínima</td><td>");
    client.print(configParameters[4]);
    client.println(" %</td></tr>");

    client.print("<tr><td>Humedad del Suelo Máxima</td><td>");
    client.print(configParameters[5]);
    client.println(" %</td></tr>");

    client.print("<tr><td>Tiempo de Infiltración</td><td>");
    client.print(configParameters[6]);
    client.println(" segundos</td></tr>");

    client.print("<tr><td>Tiempo de Riego</td><td>");
    client.print(configParameters[7]);
    client.println(" segundos</td></tr>");
    client.println("</table>");

    client.println("</body></html>");
}

void EthernetController::loop() {
    EthernetClient client = server.available();

    if (client) {
        Serial.println("Cliente conectado");
        String request = client.readStringUntil('\r');
        Serial.println("Solicitud recibida:");
        Serial.println(request);
        client.flush();

        // Enviar la página web de respuesta
        homePage(client);

        // Cerrar la conexión con el cliente
        client.stop();
        Serial.println("Cliente desconectado");
    }
}
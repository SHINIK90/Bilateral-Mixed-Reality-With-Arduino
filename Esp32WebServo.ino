#include <WiFi.h>
#include <WebServer.h>

#define RXD2 16
#define TXD2 17

const char* ssid = "ESP32_AP";
const char* password = "12345678";

WebServer server(80);

void setup() {
  Serial.begin(115200);  // Debugging via Serial Monitor
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);   // Serial communication with Arduino Mega
  
  // Set up the access point
  WiFi.softAP(ssid, password);
  Serial.println("Access point started");
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());

  // Define routes
  server.on("/", handleRoot);
  server.on("/sendAngle", handleSendAngle);
  
  // Start the server
  server.begin();
  Serial.println("HTTP server started");
}

void handleRoot() {
  String html = "<html><body>"
                "<h1>ESP32 Servo Control</h1>"
                "<input type='range' min='0' max='180' value='90' id='angleSlider' onchange='sendAngle()'>"
                "<p>Angle: <span id='angleValue'>90</span>°</p>"
                "<script>"
                "function sendAngle() {"
                "  var slider = document.getElementById('angleSlider');"
                "  var angle = slider.value;"
                "  document.getElementById('angleValue').innerText = angle;"
                "  var xhr = new XMLHttpRequest();"
                "  xhr.open('GET', '/sendAngle?angle=' + angle, true);"
                "  xhr.send();"
                "}"
                "</script>"
                "</body></html>";
  server.send(200, "text/html", html);
}

void handleSendAngle() {
  if (server.hasArg("angle")) {
    String angle = server.arg("angle");
    Serial2.println(angle);  // Send the angle value to Arduino Mega via Serial2
    Serial.println(angle);
    server.send(200, "text/plain", "Angle received: " + angle);
  } else {
    server.send(400, "text/plain", "Bad Request: Angle value missing");
  }
}

void loop() {
  server.handleClient();
}

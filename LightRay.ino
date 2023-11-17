#include <ESP8266WiFi.h>

// WiFi credentials
const char *ssid = "LightRAy";
const char *password = "123456789";

// Create a WiFi server on port 80
WiFiServer server(80);

// Variables to store the state of outputs, luminosity data, and manual mode
String header;
String output26State = "off";
String output27State = "off";
int luminData = 0;
bool manualMode = false;

// Define pin assignments for outputs and LDR (Light Dependent Resistor)
const int output26 = D4;
const int output27 = D1;
const int ldr = A0;

void setup() {
  // Initialize serial communication
  Serial.begin(115200);

  // Set pin modes for outputs and LDR
  pinMode(output26, OUTPUT);
  pinMode(output27, OUTPUT);
  pinMode(ldr, INPUT);

  // Set initial state for outputs
  digitalWrite(output26, LOW);
  digitalWrite(output27, LOW);

  // Create an Access Point with specified credentials
  Serial.println("Setting up Access Point...");
  WiFi.softAP(ssid, password);

  // Get and print the Access Point IP address
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  // Start the server
  server.begin();
}

void loop() {
  // Read luminosity data from the LDR
  luminData = analogRead(ldr);
  Serial.println("Light intensity =" + String(luminData));
  delay(1000);

  // If not in manual mode, control outputs based on luminosity level
  if (!manualMode) {
    if (luminData < 5) {
      output26State = "on";
      output27State = "on";
      digitalWrite(output26, HIGH);
      digitalWrite(output27, HIGH);
    } else {
      output26State = "off";
      output27State = "off";
      digitalWrite(output26, LOW);
      digitalWrite(output27, LOW);
    }
  }

  // Check for incoming client requests
  WiFiClient client = server.available();

  if (client) {
    Serial.println("New Client.");
    String currentLine = "";

    // Read and process the client request
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        header += c;
        if (c == '\n') {
          if (currentLine.length() == 0) {
            handleClient(client);
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }

    // Clear header and close the client connection
    header = "";
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}

// Handle different client requests
void handleClient(WiFiClient client) {
  if (header.indexOf("GET /manual") >= 0) {
    // Toggle manual mode
    manualMode = !manualMode;
    Serial.println("Manual Mode: " + String(manualMode ? "ON" : "OFF"));
    sendHeader(client);
    client.println("HTTP/1.1 303 See Other");
    client.println("Location: /");
    client.println();
  } else if (header.indexOf("GET /26/on") >= 0) {
    // Turn on output 26
    output26State = "on";
    digitalWrite(output26, HIGH);
    sendHeader(client);
    sendWebPage(client);
  } else if (header.indexOf("GET /26/off") >= 0) {
    // Turn off output 26
    output26State = "off";
    digitalWrite(output26, LOW);
    sendHeader(client);
    sendWebPage(client);
  } else if (header.indexOf("GET /27/on") >= 0) {
    // Turn on output 27
    output27State = "on";
    digitalWrite(output27, HIGH);
    sendHeader(client);
    sendWebPage(client);
  } else if (header.indexOf("GET /27/off") >= 0) {
    // Turn off output 27
    output27State = "off";
    digitalWrite(output27, LOW);
    sendHeader(client);
    sendWebPage(client);
  } else {
    // Default case, send header and web page
    sendHeader(client);
    sendWebPage(client);
  }

  // Reset currentLine for the next iteration
  String currentLine = "";
}

// Send HTTP header
void sendHeader(WiFiClient client) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:text/html");
  client.println("Connection: close");
  client.println();
}

// Send the HTML web page to the client
void sendWebPage(WiFiClient client) {
  client.println("<!DOCTYPE html><html>");
  client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
  client.println("<link rel=\"icon\" href=\"data:,\">");
  client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
  client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
  client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
  client.println(".button2 {background-color: #555555;}</style></head>");

  client.println("<body><h1>ESP8266 Web Server</h1>");
  client.println("<p>GPIO 26 - State " + output26State + "</p>");
  if (output26State == "off") {
    client.println("<p><a href=\"/26/on\"><button class=\"button\">ON</button></a></p>");
  } else {
    client.println("<p><a href=\"/26/off\"><button class=\"button button2\">OFF</button></a></p>");
  }

  client.println("<p>GPIO 27 - State " + output27State + "</p>");
  if (output27State == "off") {
    client.println("<p><a href=\"/27/on\"><button class=\"button\">ON</button></a></p>");
  } else {
    client.println("<p><a href=\"/27/off\"><button class=\"button button2\">OFF</button></a></p>");
  }

  client.println("<p>Luminosity: " + String(luminData) + "</p>");
  client.println("<p>Manual Override: <button class=\"button\" onclick=\"toggleManual()\">Toggle</button></p>");
  client.println("<script>");
  client.println("function toggleManual() {");
  client.println("  var xhttp = new XMLHttpRequest();");
  client.println("  xhttp.open('GET', '/manual', true);");
  client.println("  xhttp.send();");
  client.println("}</script>");

  client.println("</body></html>");
}

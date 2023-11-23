#include <ESP8266WiFi.h>

// WiFi credentials
const char *ssid = "LightRAy";
const char *password = "123456789";

// Create a WiFi server on port 80
WiFiServer server(80);

// Variables to store the state of SwitchD4
String SwitchD4State = "off";

// Define pin assignment for SwitchD4 (GPIO 26)
const int SwitchD4 = D4;

void setup() {
  // Initialize serial communication
  Serial.begin(115200);

  // Set pin mode for SwitchD4
  pinMode(SwitchD4, OUTPUT);

  // Set initial state for SwitchD4
  digitalWrite(SwitchD4, LOW);

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
  // Check for incoming client requests for SwitchD4
  WiFiClient client = server.available();

  if (client) {
    Serial.println("New Client.");
    String currentLine = "";

    // Read and process the client request for SwitchD4
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        currentLine += c;
        if (c == '\n') {
          // If the current line is blank, it's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 1) {
            // HTTP headers
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // Process the request for SwitchD4
            if (currentLine.indexOf("GET /SwitchD4/on") >= 0) {
              Serial.println("SwitchD4 on");
              SwitchD4State = "on";
              digitalWrite(SwitchD4, HIGH);
            } else if (currentLine.indexOf("GET /SwitchD4/off") >= 0) {
              Serial.println("SwitchD4 off");
              SwitchD4State = "off";
              digitalWrite(SwitchD4, LOW);
            }

            // Display the HTML web page for SwitchD4 with styling
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // Styling for buttons
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");
            client.println("<body><h1>ESP8266 Web Server</h1>");
            client.println("<p>SwitchD4 - State " + SwitchD4State + "</p>");
            if (SwitchD4State == "off") {
              client.println("<p><a href=\"/SwitchD4/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/SwitchD4/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
            client.println("</body></html>");

            // Break out of the while loop
            break;
          } else { // If not a blank line, clear currentLine
            currentLine = "";
          }
        }
      }
    }

    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}


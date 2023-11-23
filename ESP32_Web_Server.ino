#include <ESP8266WiFi.h>

// Replace with your network credentials
const char* ssid = "Name";
const char* password = "123456789";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String D4OutputState = "off";

// Assign output variables to GPIO pins
const int D4Output = D1;

void setup() {
  Serial.begin(115200);
  // Initialize the output variables as outputs
  pinMode(D4Output, OUTPUT);
  // Set output to LOW
  digitalWrite(D4Output, LOW);

  // Connect to Wi-Fi network with SSID and password
  Serial1.print("Setting up Access Point...");
  // Remove the password parameter if you want the Access Point to be open
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  server.begin();
}

void loop() {
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out on the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out to the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g., HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // turns the GPIO on and off
            if (header.indexOf("GET /D4/on") >= 0) {
              Serial.println("D4 Output on");
              D4OutputState = "on";
              digitalWrite(D4Output, HIGH);
            } else if (header.indexOf("GET /D4/off") >= 0) {
              Serial.println("D4 Output off");
              D4OutputState = "off";
              digitalWrite(D4Output, LOW);
            }

            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons with rounded corners and drop shadow
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer; border-radius: 12px; box-shadow: 0 8px 16px rgba(0,0,0,0.2);}");
            client.println(".button2 {background-color: #555555;}</style></head>");

            // Web Page Heading
            client.println("<body><h1>Single Switch Web Server</h1>");

            // Display current state and ON/OFF buttons for D4 Output
            client.println("<p>D4 Output - State " + D4OutputState + "</p>");
            // If the D4OutputState is off, it displays the ON button       
            if (D4OutputState == "off") {
              client.println("<p><a href=\"/D4/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/D4/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
            client.println("</body></html>");

            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}



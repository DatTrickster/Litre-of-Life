# Litre-of-Life V2

SSID and password for the Access Point (AP) are set as global variables (ssid and password). The AP is created using WiFi.softAP(ssid, password), allowing devices to connect to the ESP8266 as a WiFi hotspot. Web Server Setup:

The code sets up a web server on port 80 using WiFiServer server(80). The server listens for incoming client connections using WiFiClient client = server.available(). GPIO Configuration:

Two GPIO pins (D4 and D1) are assigned to control the state of the outputs. These GPIO pins are set as outputs using pinMode(output26, OUTPUT) and pinMode(output27, OUTPUT).

# New stuff added 
added new Ldr function an set up master overide button to switch manuel mode on/off

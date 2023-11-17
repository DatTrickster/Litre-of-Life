# Litre-of-Life V2

The SSID and password for the access point (AP) are set as global variables (ssid and password). The AP is created using WiFi.softAP (ssid, password), allowing devices to connect to the ESP8266 as a WiFi hotspot. Web Server Setup:

The code sets up a web server on port 80 using the WiFiServer server (80). The server listens for incoming client connections using WiFiClient client = server.available(). GPIO Configuration:

Two GPIO pins (D4 and D1) are assigned to control the state of the outputs. These GPIO pins are set as outputs using pinMode (output26, OUTPUT) and pinMode (output27, OUTPUT).

# New stuff added 
added new Ldr function and set up master override button to switch manual mode on/off
Using pin A0 now for Ldr 

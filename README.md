# Litre-of-Life V2 CODE

The SSID and password for the access point (AP) are set as global variables (ssid and password). The AP is created using WiFi.softAP (ssid, password), allowing devices to connect to the ESP8266 as a WiFi hotspot. Web Server Setup:

The code sets up a web server on port 80 using the WiFiServer server (80). The server listens for incoming client connections using WiFiClient client = server.available(). GPIO Configuration:

Two GPIO pins (D4 and D1) are assigned to control the state of the outputs. These GPIO pins are set as outputs using pinMode (output26, OUTPUT) and pinMode (output27, OUTPUT).

# New stuff added in V2 code

###-added new Ldr function and set up master override button to switch manual mode on/off
###-Using pin A0 now for Ldr 
###-UI improvements

# ESP8266 Web Server Documentation
# Overview

This document provides an overview and explanation of the code for an ESP8266-based web server. The server controls two GPIO outputs and monitors the light intensity using a Light Dependent Resistor (LDR). Users can interact with the server through a web interface to manually control the outputs and observe the luminosity.

Components Used
ESP8266 WiFi module
Light Dependent Resistor (LDR)
Resistors and wiring for the circuit
Code Structure
The code is written in C++ and uses the Arduino framework. It is divided into several sections:

Library Inclusion:

The necessary libraries, including ESP8266WiFi.h, are included at the beginning of the code.
WiFi Configuration:

WiFi credentials (SSID and password) are defined for creating an Access Point.
Pin Configuration:

Pins for GPIO outputs (D4 and D1) and the LDR (A0) are defined.
Setup Function:

Serial communication is initialized.
Pin modes are set for outputs and inputs.
Access Point is set up with the defined credentials.
The server is started on port 80.
Loop Function:

Luminosity data is read from the LDR.
Automatic mode adjusts outputs based on luminosity (if not in manual mode).
Server checks for incoming client requests.
Serial output displays light intensity.
Handle Client Function:

Processes different HTTP requests:
Toggle manual mode ("/manual")
Control GPIO 26 ("/26/on" and "/26/off")
Control GPIO 27 ("/27/on" and "/27/off")
Send Header Function:

Sends HTTP response headers to clients.
Send Web Page Function:

Constructs and sends the HTML web page to clients.
Includes buttons for manual control, output states, and luminosity.
Web Interface
The web interface provides the following features:

Manual Mode Toggle: Allows users to switch between automatic and manual control.
Output Control: Buttons to turn GPIO 26 and GPIO 27 on or off.
Luminosity Display: Shows the current light intensity from the LDR.
Usage
Upload the code to the ESP8266 using the Arduino IDE.
Connect the circuit components as per the defined pin configurations.
Power on the ESP8266.
Connect to the Access Point ("LightRAy") using the specified password.
Open a web browser and navigate to the IP address displayed in the serial monitor.
Use the web interface to control outputs and monitor luminosity.
Notes
Adjustments to luminosity thresholds in the automatic mode can be made based on the specific requirements.
Ensure that the ESP8266 board and libraries are properly installed in the Arduino IDE.

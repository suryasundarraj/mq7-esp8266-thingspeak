# mq7-esp8266-thingspeak

This repo is used to get the CO gas level from the atmosphere using MQ-7 Gas Sensor and to upload to the Thingspeak.com to visiualize the data in the Grapical Form to Analyze easily.

### Installing the Arduino ESP8266 Core with the Boards Manager ###

- Download and Install Arduino 1.6.5 from [https://www.arduino.cc/en/Main/Software]
- Start Arduino and open Preferences window.
- Enter ```http://arduino.esp8266.com/stable/package_esp8266com_index.json``` into *Additional Board Manager URLs* field. You can add multiple URLs, separating them with commas.
- Open Boards Manager from Tools > Board menu and install *esp8266* platform (and don't forget to select your ESP8266 board from Tools > Board menu after installation).
- 

### Uploading the Program to the ESP8266 using the Arduino IDE

Step 1: Get this Git Repo to your desktop using,

                git clone https://github.com/suryasundarraj/mq7-esp8266-thingspeak.git

Step 2: Open the coSensor.ino in Arduino IDE

Step 3: Select the NodeMCU 1.0(ESP8266 V12-E), 80Mhz,115200

Step 4: Select the USB Port from Tools - > Port

Step 5: Edit the SSID and PASSWORD to configure to your router

Step 6: Edit the Thignspeak-Key to your unique key provided by thingspeak.com

Step 7: Upload the Code to the ESP8266 (Note: Connect the GPIO 0 to GND to Pull the ESP8266 to Flashing Mode)

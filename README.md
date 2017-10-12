# homeIoT
This project demonstrate the concept of home automation where different loads can be controlled remotely.

The main idea is to provide a plug and play solution to end user, which can be placed inside the already installed power or distribution board.
Each board will be integrated with ESP8266 board and this code will allow the user not only control the distribution and electric boards digitally but also analoge control are also in the hands of user. 
Board can be controlled over WiFi, WLAN even without any connectivity as this board also act as a http server.

ESP8266 module can also control the working through voice recognition because it as a Wemo switch and recognize by Amazon echo dot present in same network.
Main language used for this project is C and source code starts from .ino file.

Protocols used for this projects are HTTP and MQTT.
Libraries used for this project are ESP8266WiFi, AsyncMqttClient, ESP8266mDNS, fauxmoESP, ArduinoOTA.

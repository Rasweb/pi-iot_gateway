# ESP32 MQTT Client

## Table of Contents
- [Overview](#overview)
- [Features](#features)
- [Getting Started](#getting-started)
- [Build Steps](#build-steps)
- [In Action](#in-action)
- [Built With](#built-with)

## Overview
The ESP32 MQTT Client is designed to connect to the Raspberry Pi MQTT broker, allowing for seamless communication and data exchange. This client can publish sensor data and subscribe to topics for receiving commands.

## Features
- Connects to the Raspberry Pi MQTT broker.
- Publishes sensor data to specified topics.
- Subscribes to topics for receiving commands.
- Supports Wi-Fi connectivity for remote access.

## Getting Started
To get started with the ESP32 MQTT Client, follow the instructions below to set up your development environment and upload the code to your ESP32 device.

## Build Steps
1. Set Up the ESP32 Development Environment:
   - Set up the esp32 or use another client.
   - I used esp32-c6 espressif idf framework using c. 

2. Clone the Repository:
   ```bash
   git clone https://github.com/Rasweb/pi-iot_gateway.git
   cd pi-iot_gateway/esp-mqtt-client
    ```
   
 3. Configure the code
    - Create a config.h file for credentials inside the main folder:
      * MQTT_USERNAME
      * MQTT_PASSWORD
      * BROKER_IP
    - Create a config.h file for credentials inside the components/wifi/include folder:
      * WIFI_NAME - SSID
      * WIFI_PASSWORD

## In Action
Once the code is uploaded, the ESP32 will connect to the specified WI-FI network and MQTT broker.
You can monitor the serial output to see published messages and received commands.

## Built With
- ESP32-C6
- Espressif idf
  *  nvs_flash
  *  esp_netif
- C
- MQTT
  *  mqtt_client
- WiFi
  * esp_wifi
   

# Pi IoT Gateway
Personal IoT project to learn more about: python, embedded c, mqtt and raspberry pi.

## Table of Contents
- [Overview](#overview)
- [Project Structure](#project-structure)
  - [Raspberry Pi Broker](#raspberry-pi-broker)
  - [ESP32 MQTT Client](#esp32-mqtt-client)
- [Getting Started](#getting-started)
- [Built With](#built-with)

## Overview
The Pi IoT Gateway project facilitates communication between IoT devices using a Raspberry Pi as an MQTT broker and ESP32 clients. This setup allows for efficient data collection, processing, and visualization in IoT applications.

## Project Structure
This repository contains two main components:

### Raspberry Pi Broker
- **Folder**: `/dashboard`
- **Description**: Contains the code and configuration files for setting up the Raspberry Pi as an MQTT broker and a dashboard using Python Flask and Mosquitto.

### ESP32 MQTT Client
- **Folder**: `/esp-mqtt-client`
- **Description**: Contains the code for the ESP32 clients that connect to the Raspberry Pi broker, allowing for data publishing.

## Getting Started
To get started with the project, follow the instructions in the respective folders for the Raspberry Pi broker and ESP32 client.

### Raspberry Pi Broker
1. Follow the instructions in the [Raspberry Pi Broker README](dashboard/README.md) to set up the broker.

### ESP32 Client
1. Follow the instructions in the [ESP32 MQTT Client README](esp-mqtt-client/README.md) to set up the client.

## Built With
- Raspberry Pi 5 - 4GB
- Python
- Flask
- MQTT (Mosquitto)
- ESP32-C6 (espressif idf)
- Paho-MQTT (python)
- PubSubClient library (mqtt for ESP32)
- Embedded C

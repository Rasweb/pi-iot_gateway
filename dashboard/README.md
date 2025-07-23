## Table of contents
- [Overview](#overview)
  - [Dashboard and MQTT](#dashboard-and-mqtt)
  - [Built With](#built-with)
  - [Build Steps](#build-steps)
  - [In Action](#in-action)

## Overview
This README is connected to the Raspberry Pi 5 dashboard using Python Flask and MQTT Mosquitto.

### Dashboard and MQTT
- The dashboard interacts with MQTT to facilitate real-time data exchange.
- Key features include:
  * Real-time Data Visualization: Monitor sensor readings and device statuses.
  * User Authentication: Secure access to the dashboard using MQTT authentication. 

### Built With
- Python
- Raspberry pi 5 - 4GB
  *  Raspberry Pi OS Lite - 64bit (command-line operating system)
  *  SSH (for headless connection)
- Flask
- MQTT
  * Phao MQTT client (MQTT for Python)
  * Mosquitto (MQTT broker package)
- Enivorment variables
  * dotenv
  * os
- Additional Libraries
  * psutil (for device information)
  * datetime (to calculate uptime)

### Build Steps
1. Setup the raspberry pi (I used ssh):
   - Ensure your Raspberry Pi is connected to the network and accessible via SSH.

2. Install MQTT on Raspberry Pi:
   - Install Mosquitto:
     ```bash
     sudo apt install mosquitto mosquitto-clients
     ```
   - Enable the MQTT broker:
     ```bash
     sudo systemctl enable mosquitto
     ```
     - Check the MQTT broker status:
     ```bash
     sudo systemctl status mosquitto
     ```
     - Restart the MQTT broker if necessary:
     ```bash
     sudo systemctl restart mosquitto
     ```

3. Testing MQTT on Raspberry Pi:
   - Subscriber: Listen to the "test/message" topic:
     ```bash
     mosquitto_sub -v -t test/message
     ```
   - Publisher: Send a message to the "test/message" topic:
     ```bash
     mosquitto_pub -t test/message -m 'Hello World!'
     ```

4. Remote Access and Authentication for MQTT:
   - Create a password file and set a password:
   ```bash
    sudo mosquitto_passwd -c /etc/mosquitto/passwd <password>
   ```
   - Enforce authentication and allow remote connections:
   ```bash
   sudo nano /etc/mosquitto/mosquitto.conf
   ```
   Add the following lines at the bottom:
   ```bash
   listener 1883
   allow_anonymous false
   password_file /etc/mosquitto/passwd
   ```

5. Set Up the Python Dashboard and Flask on the Pi:
   - Choose a package manager (pip is shown below):
   ```bash
   sudo apt install python3-pip
   ```
   - Create a directory for your project:
   ```bash
   mkdir directory_name && cd directory_name
   ```
   - Create a new Python virtual environment:
   ```bash
   python3 -m venv flask_env
   ```
   - Activate the virtual environment:
   ```bash
   source flask_env/bin/activate
   ```
   - Install Flask:
   ```bash
   pip install flask
   ```
     
### In Action
Once the setup is complete, you can access the dashboard at `http://<your-pi-ip>:<port>`. The dashboard will display real-time data from your MQTT topics, consult the python file for correct topic names. 



# protocol_example_connect

This component implements the most common connection methods for ESP32 boards. It should be used mainly in examples of ESP-IDF to demonstrate functionality of network protocols and other libraries, that need the connection step as a prerequisite.

MODIFIED README and folder from the: [Original folder](https://pages.github.com/).

## How to use this component

Choose the interface (WiFi) to connect to the network and configure the interface.

### WiFi

Choose WiFi connection method (for chipsets that support it) and configure basic WiFi connection properties:
* WiFi SSID
* WiFI password
* Maximum connection retry (connection would be aborted if it doesn't succeed after specified number of retries)
* WiFi scan method (including RSSI and authorization mode threshold)

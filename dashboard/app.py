from flask import Flask, jsonify, render_template
import paho.mqtt.client as mqtt
from dotenv import load_dotenv
import os

load_dotenv()  # take environment variables
mqtt_user = os.getenv("MQTT_USER")
mqtt_pass = os.getenv("MQTT_PASS")

# Initiates the flask application
app = Flask(__name__)
test_data = {}

# TODO - Track connected devices, status and more using a dictionary
# TODO - Get more mqtt info, logging and events
# TODO - Look up psutil for raspberry pi system info and display that
# TODO - Add post routes to recive data from other devices
# TODO - Add a basic authentication for some routes
# TODO - Add error handling


# TODO - Routes to implement (not for frontend)
#   - /api/sensors - return sensor data as JSON 

# TODO - subscribe to more topics
# From paho mqtt with required params
def on_connect(client, userdata, flags, rc):
    print("Connected to MQTT broker")
    client.subscribe("test/#") # subscribe to all test topics  

# From paho mqtt with required params
def on_message(client, userdata, msg):
    # Refers to a variable outside the function
    global test_data
    topic = msg.topic
    payload = msg.payload.decode()
    print(f"Received message on {topic}: {payload}") 
    # dictonary
    test_data[topic] = payload

mqtt_client = mqtt.Client()
# Set username and password for mqtt authentication
mqtt_client.username_pw_set(mqtt_user, mqtt_pass)
mqtt_client.on_connect = on_connect
mqtt_client.on_message = on_message
# mqtt broker
mqtt_client.connect("localhost", 1883, 60)
# mqtt loop
mqtt_client.loop_start()

# Homepage
@app.route('/')
def home():
    # TODO - Send variable info with page render
    # - return render_template('index.html', bob="hello")
    # display bob in html
    # - <p>{{ bob }}</p>

    return render_template('index.html')

# Info route, renders html
@app.route('/info')
def info():
    return render_template('info.html')

@app.route('/test')
def testd():
    # Return data in JSON
    return jsonify(test_data)

# Activates flask appliaction in debug mode
# Captures any changes in the application and reloads it
if __name__ == '__main__':
    app.run(debug=True)
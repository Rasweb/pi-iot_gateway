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


def on_connect(client, userdata, flags, rc):
    print("Connected to MQTT broker")
    client.subscribe("test/temp")  

def on_message(client, userdata, msg):
    # Refers to a variable outside the function
    global test_data
    topic = msg.topic
    payload = msg.payload.decode()
    print(f"Received message on {topic}: {payload}") 
    # dictonary
    test_data[topic] = payload

mqtt_client = mqtt.Client()
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
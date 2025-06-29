from flask import Flask, jsonify, render_template
import paho.mqtt.client as mqtt
from dotenv import load_dotenv
import os
import psutil

load_dotenv()  # take environment variables
mqtt_user = os.getenv("MQTT_USER")
mqtt_pass = os.getenv("MQTT_PASS")

# Initiates the flask application
app = Flask(__name__)

# TODO - subscribe to more topics
# From paho mqtt with required params
def on_connect(client, userdata, flags, reason_code, properties):
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

mqtt_client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)
# Set username and password for mqtt authentication
mqtt_client.username_pw_set(mqtt_user, mqtt_pass)
mqtt_client.on_connect = on_connect
mqtt_client.on_message = on_message
# mqtt broker
mqtt_client.connect("localhost", 1883, 60)
# mqtt loop
mqtt_client.loop_start()

test_data = {}

def system_info():
    memory = psutil.virtual_memory()
    # Divide from Bytes -> KB -> MB
    available = round(memory.available/1024.0/1024.0,1)
    total = round(memory.total/1024.0/1024.0,1)
    # print(str(available) + "MB free /" + str(total) + "MB total ( " + str(memory.percent) + "% )")
    memory_info = str(available) + "MB free /" + str(total) + "MB total ( " + str(memory.percent) + "% )"

    disk = psutil.disk_usage('/')
    # Divide from Bytes -> KB -> MB -> GB
    free = round(disk.free/1024.0/1024.0/1024.0,1)
    total = round(disk.total/1024.0/1024.0/1024.0,1)
    # print(str(free) + "GB free /" + str(total) + "GB total ( " + str(disk.percent) + "% )")
    disk_info = str(free) + "GB free /" + str(total) + "GB total ( " + str(disk.percent) + "% )"
 
    # Memory Information
    memory = psutil.virtual_memory()
    memory_info = {
        'Total Memory': f"{memory.total / (1024 ** 2):.2f} MB",
        'Available Memory': f"{memory.available / (1024 ** 2):.2f} MB",
        'Used Memory': f"{memory.used / (1024 ** 2):.2f} MB",
        'Memory Usage': f"{memory.percent}%"
    }

    # Disk Information
    disk = psutil.disk_usage('/')
    
    #  CPU Information
    cpu_temp = psutil.sensors_temperatures().get("cpu_thermal", [])
    for entry in cpu_temp:
        # Create a new dict with key/value pairs
        cpu_info = dict(Usage = {psutil.cpu_percent(1)},
                        Current = {entry.current},
                        High = entry.high, 
                        Critical = {entry.critical})

    disk_info =  {
        'Total Disk Space': f"{disk.total / (1024 ** 3):.2f} GB",
        'Used Disk Space': f"{disk.used / (1024 ** 3):.2f} GB",
        'Free Disk Space': f"{disk.free / (1024 ** 3):.2f} GB",
        'Disk Usage': f"{disk.percent}%"
    }

    info = {
        'Memory': memory_info,
        'Disk': disk_info,
        'CPU': cpu_info
    }

    return info

# Print mqtt information usign the mqtt_client variable
def mqtt_info():
    print(f"d {mqtt_client.transport}")

# TODO - Track connected devices, status and more using a dictionary
# TODO - Get more mqtt info, logging and events
# TODO - Add post routes to recive data from other devices
# TODO - Add a basic authentication for some routes
# TODO - Add error handling


# TODO - Routes to implement (not for frontend)
#   - /api/sensors - return sensor data as JSON 



# Homepage
@app.route('/')
def home():
    info = system_info()
    mqtt_info()
    return render_template('index.html', system_info = info)

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
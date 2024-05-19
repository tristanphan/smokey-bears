import logging
import sys
from datetime import datetime, date
from typing import Tuple, List

import waitress
from flask import Flask, render_template, request

log = logging.getLogger("werkzeug")
log.setLevel(logging.ERROR)

Temperature = float
Humidity = float

app = Flask(__name__)
data: List[Tuple[date, Temperature, Humidity]] = []
date_formatter = "%m/%d %H:%M:%S"  # previously "%H:%M:%S"


@app.route("/")
def portal():
    return render_template("index.html")


@app.route("/update", methods=["GET"])
def update():
    new_temperature = float(request.args["temperature"])
    new_humidity = float(request.args["humidity"])
    d = datetime.now()
    data.append((d, new_temperature, new_humidity))
    print(f"Updated temperature = {new_temperature} and humidity = {new_humidity} at {d.strftime(date_formatter)}")
    return f"Set temperature = {new_temperature}, humidity = {new_humidity} at {d.strftime(date_formatter)}", 200


@app.route("/retrieve", methods=["GET"])
def retrieve():
    return {
        "timestamps": [d.strftime(date_formatter) for d, _, _ in data],
        "temperature_values": [temp for _, temp, _ in data],
        "humidity_values": [hum for _, _, hum in data],
    }, 200


@app.route("/clear", methods=["GET"])
def clear():
    data.clear()
    print("Cleared all data")
    return "Cleared all data", 200


if __name__ == "__main__":
    print(f"Starting server on port {int(sys.argv[1]) if len(sys.argv) > 1 else 5000}...")
    waitress.serve(
        app,
        host="0.0.0.0",
        port=int(sys.argv[1]) if len(sys.argv) > 1 else 5000,
    )

import logging
import sys
from datetime import datetime, date
from typing import Tuple, List

import waitress
from flask import Flask, render_template, request

log = logging.getLogger("werkzeug")
log.setLevel(logging.ERROR)

Temperature = float
GasConcentration = float

app = Flask(__name__)
data: List[Tuple[date, Temperature, GasConcentration]] = []
date_formatter = "%m/%d %H:%M:%S"  # previously "%H:%M:%S"


@app.route("/")
def portal():
    return render_template("index.html")


@app.route("/update", methods=["GET"])
def update():
    try:
        new_temperature = float(request.args["temperature"])
        new_gas_concentration = float(request.args["gas_concentration"])
    except ValueError:
        return "Invalid input, expected float", 400
    d = datetime.now()
    data.append((d, new_temperature, new_gas_concentration))
    print(f"Updated temperature = {new_temperature} and gas concentration = {new_gas_concentration} at"
          f" {d.strftime(date_formatter)}")
    return (f"Set temperature = {new_temperature}, gas concentration = {new_gas_concentration} at"
            f" {d.strftime(date_formatter)}"), 200


@app.route("/retrieve", methods=["GET"])
def retrieve():
    return {
        "timestamps": [d.strftime(date_formatter) for d, _, _ in data],
        "temperature_values": [temp for _, temp, _ in data],
        "gas_concentration_values": [hum for _, _, hum in data],
    }, 200


@app.route("/clear", methods=["GET"])
def clear():
    data.clear()
    print("Cleared all data")
    return "Cleared all data", 200


# noinspection HttpUrlsUsage
def main():
    port = int(sys.argv[1]) if len(sys.argv) > 1 else 5000
    host = "0.0.0.0"

    print(f"Starting server at http://{host}:{port}/")

    # Print instructions
    print(f"Endpoints:")
    print(f"    1. http://{host}:{port}/")
    print(f"    2. http://{host}:{port}/update?temperature=0&gas_concentration=0")
    print(f"    3. http://{host}:{port}/retrieve")
    print(f"    4. http://{host}:{port}/clear")

    waitress.serve(
        app,
        host=host,
        port=int(sys.argv[1]) if len(sys.argv) > 1 else 5000,
    )


if __name__ == "__main__":
    main()

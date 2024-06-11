import csv
import logging
import os
import re
import smtplib
import sys
from datetime import datetime, date
from email.message import EmailMessage
from pathlib import Path
from typing import Tuple, List, Optional

import waitress
from flask import Flask, render_template, request

log = logging.getLogger("werkzeug")
log.setLevel(logging.ERROR)

Temperature = float
GasLevel = float

app = Flask(__name__)
data: List[Tuple[date, Temperature, GasLevel]] = []
date_formatter = "%m/%d %H:%M:%S"
emails_path = Path("email_recipients.csv")
email_regex = re.compile(r"\b[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Z|a-z]{2,7}\b")

# Alarm mechanism is explained in [update_alarm_status_flags
alarm_status = False
manual_off = False
temperature_threshold = 40
gas_level_threshold = 70
email_recipients = []

# Sender email
sender_email = os.environ["EMAIL_USERNAME"]
sender_password = os.environ["EMAIL_PASSWORD"]


@app.route("/")
def portal():
    return render_template(
        "index.html",
        default_temperature_threshold=temperature_threshold,
        default_gas_level_threshold=gas_level_threshold,
    )


@app.route("/update", methods=["GET"])
def update():
    try:
        new_temperature = float(request.args["temperature"])
        new_gas_level = float(request.args["gas_level"])
    except (ValueError, KeyError):
        return {
            "message": "Invalid input, expected float",
        }, 400
    d = datetime.now()
    data.append((d, new_temperature, new_gas_level))
    update_alarm_status_flags_by_data()
    print(f"Updated temperature = {new_temperature} and gas level = {new_gas_level} at"
          f" {d.strftime(date_formatter)}")
    return {
        "message": (f"Set temperature = {new_temperature}, gas level = {new_gas_level} at"
                    f" {d.strftime(date_formatter)}"),
        "current_temperature": new_temperature,
        "current_gas_level": new_gas_level,
        "alarm_status": alarm_status,
    }, 200


@app.route("/retrieve", methods=["GET"])
def retrieve():
    return {
        "message": "Success",
        "timestamps": [d.strftime(date_formatter) for d, _, _ in data],
        "temperature_values": [temp for _, temp, _ in data],
        "gas_level_values": [hum for _, _, hum in data],
        "alarm_status": alarm_status,
    }, 200


@app.route("/clear", methods=["GET"])
def clear():
    data.clear()
    print("Cleared all data")
    return {
        "message": "Cleared all data",
    }, 200


@app.route("/set_alarm", methods=["GET"])
def set_alarm():
    global alarm_status, manual_off

    if ("status" not in request.args) or (request.args["status"].lower() not in ["on", "off"]):
        return {
            "message": "Invalid input, expected 'on' or 'off'",
        }, 400
    if request.args["status"].lower() == "on":
        set_alarm_status(True, "Manually activated by user")
    elif request.args["status"].lower() == "off":
        set_alarm_status(False)
        manual_off = True
        print("Manual override off enabled")
        update_alarm_status_flags_by_data()  # Can immediately unset [manual_off] if values are below threshold

    print("Set alarm status to", "on" if alarm_status else "off")
    return {
        "message": "Set alarm status to " + ("on" if alarm_status else "off"),
    }, 200


@app.route("/set_thresholds", methods=["GET"])
def set_thresholds():
    global temperature_threshold, gas_level_threshold
    if "temperature" not in request.args and "gas_level" not in request.args:
        return {
            "message": "Invalid input, expected 'temperature' or 'gas_level'",
        }, 400

    try:
        if "temperature" in request.args:
            temperature_threshold = float(request.args["temperature"])
        if "gas_level" in request.args:
            gas_level_threshold = float(request.args["gas_level"])
    except (ValueError, KeyError):
        return {
            "message": "Invalid input, expected float",
        }, 400

    print(f"Set temperature threshold to {temperature_threshold} and gas level threshold to {gas_level_threshold}")
    return {
        "message": f"Set temperature threshold to {temperature_threshold} and "
                   f"gas level threshold to {gas_level_threshold}",
    }, 200


def update_alarm_status_flags_by_data():
    # NOTE
    # The idea behind the alarm is that once the alarm is tripped (either threshold or manually),
    # it will stay on. If the alarm is manually turned off, it will reset if the values are now
    # below the threshold. If the alarm was turned off while the values were above the threshold,
    # [manual_off] will be set, so the alarm will stay off until the value dips below thresholds
    # and then goes above again.

    global alarm_status, manual_off

    # Get ideal alarm status
    new_alarm_status = False
    if len(data) > 0:
        _, temp, hum = data[-1]
        new_alarm_status = temp > temperature_threshold or hum > gas_level_threshold

    if new_alarm_status:
        # We are above threshold
        if manual_off:
            pass  # Ignore, since the user has already turned off the alarm
        else:
            set_alarm_status(True, f"Sensor exceeded threshold")
    else:
        # We are above threshold
        if manual_off:
            manual_off = False
            print("Manual override off disabled")
            set_alarm_status(False)
        else:
            pass  # Keep alarm on if it was on


def set_alarm_status(status: bool, cause: Optional[str] = None):
    global alarm_status
    if alarm_status != status and status is True:
        send_email(cause)
    alarm_status = status


def send_email(cause: Optional[str] = None):
    if len(email_recipients) == 0:
        print("No email recipients to send to")
        return

    body = f"<b>Cause of alarm:</b> {cause if cause is not None else 'Unknown'}<br/>"
    if len(data) > 0:
        body += (f"<b>Latest temperature:</b> {data[-1][1]}ºC / {temperature_threshold}ºC<br/>"
                 f"<b>Latest gas level</b>: {data[-1][2]}% / {gas_level_threshold}%<br/>")
    else:
        body += "<b>No sensor data available</b>"

    # Create message
    msg = EmailMessage()
    msg.set_content(body, subtype="html")
    msg["Subject"] = "[Urgent] Smoke Detector Activated"
    msg["From"] = sender_email
    msg["To"] = ", ".join(email_recipients)
    with smtplib.SMTP_SSL('smtp.gmail.com', 465) as smtp_server:
        smtp_server.login(sender_email, sender_password)
        smtp_server.send_message(msg)
    print("Email sent to " + ", ".join(email_recipients) +
          " with cause: " + (cause if cause is not None else "Unknown"))


# noinspection HttpUrlsUsage
def main():
    port = int(sys.argv[1]) if len(sys.argv) > 1 else 5000
    host = "0.0.0.0"

    print(f"Starting server at http://{host}:{port}/")

    # Print instructions
    print(f"Endpoints:")
    print(f"    1. http://{host}:{port}/")
    print(f"    2. http://{host}:{port}/update?temperature=0&gas_level=0")
    print(f"    3. http://{host}:{port}/retrieve")
    print(f"    4. http://{host}:{port}/clear")
    print(f"    5. http://{host}:{port}/set_alarm?status=on")
    print(f"    6. http://{host}:{port}/set_thresholds?temperature=30&gas_level=70")

    if not emails_path.exists():
        emails_path.write_text("")

    # Read email file
    with emails_path.open("r") as file:
        reader = csv.reader(file)
        for row in reader:
            if len(row) == 0:
                continue
            if email_regex.match(row[0]) is None:
                raise ValueError("Invalid email: " + row[0])
            email_recipients.append(row[0])

    waitress.serve(
        app,
        host=host,
        port=int(sys.argv[1]) if len(sys.argv) > 1 else 5000,
    )


if __name__ == "__main__":
    main()

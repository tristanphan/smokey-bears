# Server Runbook

The server consists of a REST API that supports IoT devices. It provides an interface for devices to send sensor
readings and manually activate the alarm. It also provides a web interface for users to see the sensor levels and alarm
status, as well as to manually activate the alarm.

## Table of Contents

- [Requirements Setup](#requirements-setup)
- [Running the Server](#running-the-server)
- [Email Alerts](#email-alerts)
    - [SMTP Login](#smtp-login)
    - [Recipients List](#recipients-list)

## Requirements Setup

The intended Python version of the server is 3.11.7. You can set up the requirements in a virtual environment as
follows:

```bash
cd `server/``

# Create a virtual environment
python3 -m venv venv

# If Linux/Unix/macOS
source venv/bin/activate
# If Windows
venv\Scripts\activate

# Install the requirements
pip install -r requirements.txt
```

## Running the Server

```bash
# Make sure you are in the server directory
# and the virtual environment is activated.
cd `server/`
# If Linux/Unix/macOS
source venv/bin/activate
# If Windows
venv\Scripts\activate

# Run the server
python server.py <port_number>
# <port_number> is optional, the default is 5000 if unspecified
```

The console will print the address of the server and its endpoints. You can access the web interface
at `http://<host>:<port>/` (by default `http://localhost:5000/`). You can test the other endpoints to see what they do.

## Email Alerts

The server can send email alerts when the alarm is activated. To do so, you will need to log in to an SMTP server and
specify the recipients.

### SMTP Login

Set up environment variables for the SMTP server login credentials by running the following commands (or setting the
variables in an environment file):

```bash
export EMAIL_USERNAME=senderaddress@myhost.com
export EMAIL_PASSWORD=mysupersecretpassword
```

### Recipients List

1. Create a file in the root directory of the server (`server/`) called `email_recipients.csv`
2. In this file, list the email addresses of the recipients, one per line

Each email address on the list will be recipient of alarm emails whenever the alarm gets triggered. The email will
contain the sensor readings at the time of the alarm, as well as the reason for the alarm.

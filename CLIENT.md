# Client Runbook

The client is an ESP32 device that reads data from its sensors and sends it to a server over HTTP. The device also comes
equipped with a remotely-activated alarm system, which includes a buzzer and a gas solenoid valve.

## Table of Contents

- [Getting Started](#getting-started)
- [Setting up WiFi](#setting-up-wifi)
- [Running the Client](#running-the-client)

## Getting Started

To proceed, you will need to install PlatformIO from [their website](https://platformio.org/install).

Make sure the program is installed and added to the path

```bash
platformio --version
```

## Setting up WiFi

The WiFi SSID and password are stored on the internal NVS of the ESP32.

Edit the file at `wifi_credentials_loader/src/main.cpp` to find and set the following lines:

```cpp
const char *ssid = "YOUR_SSID"
const char *password = "YOUR_PASSWORD"
```

Then, write these values to the ESP32 by running the following commands:

```bash
cd wifi_credentials_loader/

# Upload the code to the ESP32
# Make sure the device is connected to the computer
pio run -t upload

# Monitor the serial output to check if the update `was successful
pio device monitor
```

## Running the Client

You will need to update the server address in the client code. Edit the file at `client/include/config.h` to find and
set the following line:

```cpp
static const char *HOSTNAME = "myserver.com";
```

Then, upload the code to the ESP32 by running the following commands:

```bash
cd client/

# Upload the code to the ESP32
# Make sure the device is connected to the computer
pio run -t upload
  
# Monitor the serial output to check if the update was successful
pio device monitor
```

Now, the client code should be running. Confirm that it successfully connects to the internet, and check the server to
see if it is receiving data from the client.
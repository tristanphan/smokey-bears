# Smoke Detector with Automatic Gas Valve Shutoff

This project is a proof of concept for a smoke detector that can automatically shut off a gas valve when smoke is
detected. The project is divided into two parts: the client and the server. The client is an ESP32 that is connected to
a temperature sensor, gas/smoke sensor, button, Piezo buzzer, and gas valve. The server hosts a website that can be used
to monitor the client and control the alarm.

## Runbooks

If you are interested in using this project, please refer to the following guides:

- [Client Runbook](CLIENT.md)
- [Server Runbook](SERVER.md)
# Home Assistant, MQTT, and Node-RED Setup

This guide assumes Home Assistant is already installed and working.

For this project, Home Assistant acts as the central automation platform. It receives the AI detection event from the Ubiquiti camera integration, Node-RED applies the automation logic, and MQTT is used to trigger the ESP32-based announcer.

## 1. Install the MQTT broker

The first step is installing the Mosquitto MQTT broker add-on in Home Assistant.



Once installed, edit the broker configuration. YAML is the easiest way to do this for a simple setup like this.

Use the following configuration as a starting point and replace the username and password with your own values:

```yaml
logins:
  - username: [create an mqtt username]
    password: [create an mqtt password]
require_certificate: false
certfile: fullchain.pem
keyfile: privkey.pem
customize:
  active: false
  folder: mosquitto
```


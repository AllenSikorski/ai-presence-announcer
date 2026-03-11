# Home Assistant, MQTT, and Node-RED Setup

This guide assumes Home Assistant is already installed and working.

For this project, Home Assistant acts as the central automation platform. It receives the AI detection event from the Ubiquiti camera integration, Node-RED applies the automation logic, and MQTT is used to trigger the ESP32-based announcer.

## 1. Install the MQTT broker

The first step is installing the Mosquitto MQTT broker add-on in Home Assistant.

<img width="975" height="463" alt="image" src="https://github.com/user-attachments/assets/95c7987b-0eb5-4625-a834-2ddbd9d8dadc" />

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
## MQTT notes

- The default MQTT port is `1883`
- The MQTT broker IP should be the IP address of the Home Assistant instance
- That IP should be static, not assigned by DHCP
- The MQTT username and password created here must match the credentials used in the ESP32 firmware

<img width="975" height="730" alt="image" src="https://github.com/user-attachments/assets/d19aef7f-2df3-4a05-b545-54c579210bb2" />


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
## MQTT Setup

- The default MQTT port is `1883`
- The MQTT broker IP should be the IP address of the Home Assistant instance
- That IP should be static, not assigned by DHCP
- The MQTT username and password created here must match the credentials used in the ESP32 firmware

<img width="975" height="730" alt="image" src="https://github.com/user-attachments/assets/d19aef7f-2df3-4a05-b545-54c579210bb2" />


## Install the Node-RED add-on

After MQTT is working, install the Node-RED add-on in Home Assistant.

<img width="913" height="436" alt="image" src="https://github.com/user-attachments/assets/e8e6414f-ba38-449c-8f46-72f3d4e33d7a" />

Node-RED is used in this project to tie everything together:

- camera detection events from Home Assistant
- time and logic conditions
- MQTT publish and subscribe actions
- optional phone call or SMS notifications through Twilio

This makes it easy to expand the project later without constantly changing the ESP32 firmware.

## How the flow works

The Node-RED flow is simple and event-driven.

At a high level:

1. The Ubiquiti camera detects a vehicle or person
2. Home Assistant exposes that event as an entity state
3. Node-RED watches that entity for a change to `on`
4. A time range check is applied if needed
5. If the condition passes, Node-RED turns on a Home Assistant helper
6. That helper change triggers the MQTT publish node
7. The ESP32 receives the MQTT message and plays the announcement
8. After a short delay, the helper is turned back off so the system can trigger again cleanly

A physical button on the ESP32 can also publish an MQTT message back into Home Assistant for testing or for additional automations.

```text
sitelocation/announcement/button
```

### MQTT publish node

The flow sends a play command to the ESP32 on a topic like:

```text
sitelocation/announcement/play
```
Use topic names that make sense for your own environment.

## Home Assistant helper

This project uses a Home Assistant helper as a clean internal trigger point between the camera event and the MQTT announcement action.

First, add the helper to your Home Assistant configuration.

In `configuration.yaml`, create an `input_boolean` entry for the announcer trigger.

```yaml
input_boolean:
  announcement_trigger:
    name: AI Presence Announcer Trigger
    icon: mdi:bell-ring
```

After saving the configuration, restart Home Assistant so the new helper is loaded.

Once Home Assistant comes back up, the helper will be available as:

```text
input_boolean.announcement_trigger
```

This helper is used by Node-RED as a simple internal trigger flag.




When the detection conditions are met, Node-RED turns the helper on. That change can then be used to trigger the MQTT publish node that tells the ESP32 announcer to play. After a short delay, the helper is turned back off so the flow is ready for the next event.

This gives you a clean event pulse and makes it easier to add additional actions later.




## Node-RED flow: detection to trigger helper

This first part of the Node-RED flow handles the initial detection logic.

When the Ubiquiti camera detects a vehicle, Home Assistant exposes that detection as an entity state. Node-RED watches that entity and waits for it to change to `on`.

Once the detection is active, the flow passes through a time range check. In this example, announcements are only allowed between `8:00` and `16:30`.

If the detection happens during the allowed time window, Node-RED turns on the Home Assistant helper:

```text
input_boolean.announcement_trigger
```


That helper acts as the internal handoff point for the rest of the automation. The second half of the flow will watch for that helper to turn on and then publish the MQTT play command to the ESP32 announcer.

<img width="717" height="215" alt="image" src="https://github.com/user-attachments/assets/789d9f35-42ef-4ca4-b45f-aaae78b1d4e3" />


<table>
  <tr>
    <td align="center">
      <img width="500" alt="Node-RED flow overview" src="https://github.com/user-attachments/assets/598d404a-54ca-4411-818c-348f7975b23f" />
    </td>
    <td align="center">
      <img width="500" alt="Vehicle detection state node" src="https://github.com/user-attachments/assets/ebe1406c-288c-4edb-9d90-0280e3eb8d2c" />
    </td>
  </tr>
  <tr>
    <td align="center">
      <img width="500" alt="Action node data" src="https://github.com/user-attachments/assets/ccb47df1-b0a3-465d-a805-409f35b7725d" />
    </td>
    <td align="center">
      <img width="500" alt="Time range node" src="https://github.com/user-attachments/assets/8dc72ee8-9604-4664-9f16-9280db1b1956" />
    </td>
  </tr>
</table>

Use the following code under Data in the action Node: 
```text
{   "entity_id": "input_boolean.announcement_trigger" }
```



## Node-RED flow: helper to announcement playback

This second part of the Node-RED flow handles the actual announcement trigger.

<img width="881" height="227" alt="image" src="https://github.com/user-attachments/assets/6b3786be-e282-43cd-b9c8-1569db0c1ef1" />

Once the Home Assistant helper turns on, Node-RED watches for that state change and uses it as the signal to publish the MQTT play command to the ESP32 announcer.

The MQTT output node publishes to the announcement topic:

```text
sitelocation/announcement/play
```

When the ESP32 receives that MQTT message, it plays the local MP3 file through the connected PA speaker system.

At the same time, the flow also passes through a short delay. After that delay, a Home Assistant action node turns the helper back off.

This resets the system so it is ready for the next detection event.

<table>
  <tr>
    <td align="center">
      <img width="500" alt="Node-RED helper to playback flow overview" src="https://github.com/user-attachments/assets/f8ed6121-0346-432a-89ff-4cba8f1c9305" />
    </td>
    <td align="center">
      <img width="500" alt="MQTT publish node for announcement playback" src="https://github.com/user-attachments/assets/44a33cd0-fb24-4ef9-afe6-28255df93b98" />
    </td>
  </tr>
  <tr>
    <td align="center">
      <img width="500" alt="Delay node" src="https://github.com/user-attachments/assets/9f398942-25d8-48f9-8743-06d8e5f143d8" />
    </td>
    <td align="center">
      <img width="500" alt="Helper turn off action node" src="https://github.com/user-attachments/assets/3214e0ec-0276-4b16-8cf1-8c429ee2bbe7" />
    </td>
  </tr>
</table>

Use the following code under Data in the action Node: 
```text
{   "entity_id": "input_boolean.announcement_trigger" }
```














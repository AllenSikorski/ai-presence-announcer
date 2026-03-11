# ESP32 / Arduino and DFPlayer Setup

This guide covers the hardware wiring, SD card preparation, firmware upload, and basic testing for the ESP32-based announcer.

The ESP32 is responsible for connecting to Wi-Fi, subscribing to the MQTT play topic, and telling the DFPlayer Mini to play the local MP3 file when a message is received. It can also publish a button event back to Home Assistant for testing or additional automations.

## Wiring

Build the announcer hardware using the following wiring:

### ESP32 to DFPlayer

- D5 -> Button
- GND -> Button
- TX2 -> RX
- RX2 -> TX
- 3V3 -> VCC
- GND -> GND

### DFPlayer speaker output

- ADKEY1 -> black speaker wire
- SPK_1 -> red speaker wire

This reflects the working build used in this project.

![AI Presence Announcer (3)](https://github.com/user-attachments/assets/6dd0a5ed-d0d2-4918-b9c6-724a420479a4)


## Prepare the microSD card

Before powering up the DFPlayer Mini, prepare the microSD card.

- Format the microSD card as `FAT32`
- Place the audio file in the root of the card
- Name the audio file exactly `0001.mp3`

Once the card is prepared, insert it into the DFPlayer Mini before testing.

The firmware is written to play `0001.mp3` when the MQTT play topic is received or when the local button is pressed.

## Flash the firmware

Once the hardware is wired and the microSD card is inserted, plug the ESP32 into the PC.

Open the firmware file from this project:

- [ai_presence_announcer.ino](../firmware/ai_presence_announcer.ino)

<img width="544" height="324" alt="image" src="https://github.com/user-attachments/assets/b770bd43-d457-4744-9fe6-7f10ca556367" />


In Arduino IDE:

- Select **Tools**
- Select **Board**
- Choose **ESP32 Dev Module**
- Select the correct COM port for the ESP32

Before uploading, edit the firmware so it matches your environment.

## What must be edited in the `.ino` file

Before flashing, update the following values in the firmware:

### Wi-Fi credentials

Update:

- Wi-Fi SSID
- Wi-Fi password

### Static IP configuration

Update:

- local IP address
- gateway
- subnet
- primary DNS
- secondary DNS

The ESP32 should use a valid static IP on your network.

### MQTT broker configuration

Update:

- MQTT broker IP
- MQTT username
- MQTT password
- play topic
- button topic

The MQTT broker IP should match the IP address of the Home Assistant system running Mosquitto.

The topic names should match the topics used in your Home Assistant and Node-RED setup.

After editing the values, upload the sketch to the ESP32.

## Confirm network connectivity

Once the firmware has been uploaded, the ESP32 should connect to the network using the static IP you configured.

Ping the ESP32 to verify it is reachable on the network.

In this example, the ESP32 IP is:

```text
192.168.0.88
```
If the ping responds, that confirms the device is online and reachable.

<img width="816" height="463" alt="image" src="https://github.com/user-attachments/assets/17e35a08-8806-415a-97a9-04aa71170ef6" />


## Test local audio playback

After confirming the ESP32 is on the network:

1. Connect the speaker wires to the receiver or amplifier
2. Press the physical button connected to the ESP32
3. Confirm that the chime or announcement plays

This confirms:

- the ESP32 is running
- the DFPlayer Mini is responding
- the microSD card is inserted and readable
- the audio file is named correctly
- the speaker path is working

Do not continue until local button playback works.

## Test MQTT topic publishing

Before moving forward, confirm that button presses are publishing to MQTT correctly.

In Home Assistant, go to:

- **Settings**
- **Devices & services**
- **MQTT**
- **Configuration gear**

Under **Listen to a topic**, enter:

```text
sitelocation/announcement/#
```
Edit this to match the topic prefix used in your firmware. The # acts as a wildcard so you can see all related topic traffic.

<img width="958" height="522" alt="image" src="https://github.com/user-attachments/assets/37c00b94-0c6a-4ae5-b223-ea76ad3676e7" />


Then:

1. Start listening to the topic
2. Press the physical button on the ESP32
3. Confirm that the button press is published and appears in Home Assistant

This verifies that the ESP32 can publish events back into your Home Assistant / MQTT environment.

Test that button presses publish correctly before moving forward.

### PUT SCREENSHOT OF MQTT TOPIC TESTING HERE

## What the firmware does

At a high level, the firmware does the following:

1. Initializes the DFPlayer Mini
2. Connects the ESP32 to Wi-Fi
3. Connects to the MQTT broker
4. Subscribes to the announcement play topic
5. Waits for either:
   - an MQTT play message
   - a local button press
6. Plays the MP3 file through the connected speaker system
7. Publishes a button event back to MQTT when the physical button is pressed

This keeps the ESP32 side simple and lets Home Assistant and Node-RED handle the higher-level automation logic.

## Troubleshooting basics

If the system does not work as expected, check the following first:

- the microSD card is formatted as `FAT32`
- the audio file is named exactly `0001.mp3`
- the microSD card is inserted fully into the DFPlayer Mini
- the ESP32 has the correct static IP settings
- the ESP32 can be pinged on the network
- the MQTT broker IP, username, and password are correct
- the MQTT topic names match the Home Assistant and Node-RED configuration
- the ESP32 board type in Arduino IDE is set to **ESP32 Dev Module**
- the correct COM port is selected before flashing

## Previous step

Before this section, complete:

- [Home Assistant, MQTT, and Node-RED Setup](home-assistant-setup.md)

## Next step

After the ESP32 announcer is built, flashed, and tested, return to the main project overview:

- [Project README](../README.md)


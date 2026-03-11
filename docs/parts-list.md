# Parts List

This is the core hardware used for the AI Presence Announcer build.

## Required parts

### [microSD Card](https://www.amazon.com/dp/B08J4HJ98L)
Used to store the local audio file played by the DFPlayer Mini.

### [ESP32 Development Board](https://www.amazon.com/dp/B08D5ZD528)
Main controller for the project. Connects to Wi-Fi, listens for MQTT messages, and triggers audio playback.

### [DFPlayer Mini](https://www.amazon.com/dp/B01D1D0E7Q)
MP3 playback module used to play the local chime or voice announcement from the microSD card.

### [Lever Connectors](https://www.amazon.com/dp/B01EV70C78)
Used to make the wiring cleaner and easier to service.

### 2-Wire Push Button
Used as a local physical trigger for testing or manual announcements.

Any standard 2-wire momentary push button should work.

## Notes

- Format the microSD card as **FAT32**
- Name the audio file on the card exactly **`0001.mp3`**
- The push button is optional if the system will only be triggered through automation

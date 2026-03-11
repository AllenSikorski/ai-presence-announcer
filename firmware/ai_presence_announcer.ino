#include <Arduino.h>
#include <HardwareSerial.h>
#include "DFPlayerMini_Fast.h"
#include <WiFi.h>           // Wi-Fi library for ESP32
#include <PubSubClient.h>   // MQTT library

// Wi-Fi credentials
const char* ssid = "your_wifi_ssid";
const char* password = "your_wifi_password";

// Static IP configuration
IPAddress local_IP(192, 168, 0, 88); // Replace with your desired static IP
IPAddress gateway(192, 168, 0, 1);  // Replace with your router's gateway
IPAddress subnet(255, 255, 255, 0); // Subnet mask
IPAddress primaryDNS(8, 8, 8, 8);  // Primary DNS
IPAddress secondaryDNS(1, 1, 1, 1);    // Secondary DNS

// MQTT Broker configuration
const char* mqtt_server = "192.168.0.3"; // Replace with your broker's IP (This will be the same as Home Assistant's IP Address)
const char* mqtt_user = "your_mqtt_username";
const char* mqtt_password = "your_mqtt_password";
const char* topic_play = "sitelocation/announcement/play"; //update these topics with a topic that makes sense to your use case
const char* topic_button = "sitelocation/announcement/button"; //This is the test button connected to the phyical arduino

// HardwareSerial for MP3 module
HardwareSerial mySerial(2); // TX2 = GPIO17, RX2 = GPIO16
DFPlayerMini_Fast mp3;

// Define button pin
const int buttonPin = 5; // GPIO5 (D5)

// Variable to track MP3 initialization status
bool mp3Initialized = false;

// Wi-Fi and MQTT clients
WiFiClient espClient;
PubSubClient client(espClient);

// Function to handle incoming MQTT messages
void callback(char* topic, byte* payload, unsigned int length) {
  // Convert payload to string
  String message = "";
  for (unsigned int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  Serial.print("Received message on topic: ");
  Serial.print(topic);
  Serial.print(" -> ");
  Serial.println(message);

  // Check if the message is for playing the Announcer
  if (String(topic) == topic_play) {
    Serial.println("Playing announcer from MQTT command...");
    if (mp3Initialized) {
      mp3.play(1); // Play 0001.mp3
      Serial.println("Play command sent.");
    } else {
      Serial.println("MP3 module is NOT initialized!");
    }
  }
}

// Connect to MQTT broker
void reconnect() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT broker...");
    if (client.connect("ESP32-Announcer", mqtt_user, mqtt_password)) {
      Serial.println("connected!");
      client.subscribe(topic_play); // Subscribe to play topic
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" retrying in 5 seconds...");
      delay(5000);
    }
  }
}

void setup() {
  // Initialize serial communication for debugging
  Serial.begin(115200);

  // Initialize MP3 module
  mySerial.begin(9600, SERIAL_8N1, 16, 17); // TX2 and RX2
  Serial.println("Initializing MP3 module...");
  if (mp3.begin(mySerial)) {
    Serial.println("MP3 module initialized successfully!");
    mp3.volume(20);  // Set volume (0-30, adjust as needed)
    mp3Initialized = true; // Track initialization status
  } else {
    Serial.println("Failed to initialize MP3 module!");
    mp3Initialized = false;
  }

  // Configure the button pin as an input with an internal pull-up resistor
  pinMode(buttonPin, INPUT_PULLUP);

  // Connect to Wi-Fi with static IP
  Serial.println("Connecting to Wi-Fi...");
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("Failed to configure static IP!");
  }
  WiFi.begin(ssid, password);

  // Wait for Wi-Fi connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Configure MQTT
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  // Connect to MQTT broker
  reconnect();
}

void loop() {
  // Ensure MQTT connection
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Read the button state
  int buttonState = digitalRead(buttonPin);

  // Check if the button is pressed
  if (buttonState == LOW) { // Button is pressed
    Serial.println("Button pressed! Playing announcer...");
    if (mp3Initialized) {
      mp3.play(1); // Play 0001.mp3
      Serial.println("Play command sent.");
    } else {
      Serial.println("MP3 module is NOT initialized!");
    }

    // Publish button press event to MQTT
    client.publish(topic_button, "Button Pressed");

    delay(500); // Debounce delay
  }
}

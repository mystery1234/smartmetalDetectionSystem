#include <WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

// WiFi credentials
const char* ssid = "Moto"; //ssid
const char* password = "123456789"; //password

// Adafruit IO credentials
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "ADAFRUIT USERNAME"
#define AIO_KEY         "ADAFRUIT KEY"

// Hardware pins
#define METAL_SENSOR_PIN 23
#define IR_SENSOR        22
#define LED_GREEN        14
#define LED_RED          12
#define BUZZER           15
#define RELAY            13

WiFiClient client;

// MQTT client
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

// MQTT feeds
Adafruit_MQTT_Publish metalFeed = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/factory.metal");
Adafruit_MQTT_Publish countFeed = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/factory.object-count");
Adafruit_MQTT_Subscribe relayControl = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/factory.relay-control");

// Variables
int count = 0;
bool laserState = HIGH;
bool manualRelayMode = false;  // Flag to track manual or auto mode

void connectToWiFi() {
  Serial.print("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void MQTT_connect() {
  int8_t ret;
  if (mqtt.connected()) return;

  Serial.print("Connecting to Adafruit IO... ");
  while ((ret = mqtt.connect()) != 0) {
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying in 5 seconds...");
    mqtt.disconnect();
    delay(5000);
  }
  Serial.println("Connected to Adafruit IO!");
}

void setup() {
  Serial.begin(115200);

  pinMode(METAL_SENSOR_PIN, INPUT);
  pinMode(IR_SENSOR, INPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(RELAY, OUTPUT);

  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_RED, LOW);
  digitalWrite(BUZZER, LOW);
  digitalWrite(RELAY, LOW);

  connectToWiFi();
  mqtt.subscribe(&relayControl);
}

void loop() {
  MQTT_connect();
  mqtt.processPackets(1000);

  static unsigned long lastPing = 0;
  if (millis() - lastPing > 5000) {
    mqtt.ping();
    lastPing = millis();
  }

  // Handle incoming relay control messages
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(100))) {
    if (subscription == &relayControl) {
      String value = (char *)relayControl.lastread;
      value.trim();
      Serial.print("Relay control command: ");
      Serial.println(value);

      if (value.equalsIgnoreCase("ON")) {
        digitalWrite(RELAY, HIGH);
        manualRelayMode = true;
        Serial.println("Relay manually turned ON");
      } else if (value.equalsIgnoreCase("OFF")) {
        digitalWrite(RELAY, LOW);
        manualRelayMode = true;
        Serial.println("Relay manually turned OFF");
      } else if (value.equalsIgnoreCase("AUTO")) {
        manualRelayMode = false;
        Serial.println("Switched to automatic relay control");
      } else {
        Serial.println("Unknown relay command received");
      }
    }
  }

  // Object detection
  bool objectDetected = digitalRead(IR_SENSOR) == LOW;
  if (laserState == HIGH && objectDetected) {
    Serial.println("Object detected");
    count++;
    Serial.print("Total count: ");
    Serial.println(count);
    laserState = LOW;
    if (countFeed.publish((int32_t)count)) {
      Serial.println("Count published!");
    } else {
      Serial.println("Failed to publish count");
    }
  } else if (!objectDetected) {
    laserState = HIGH;
  }

  // Metal detection (only in AUTO mode)
  bool metalDetected = digitalRead(METAL_SENSOR_PIN) == LOW;
  if (!manualRelayMode) {
    if (metalDetected) {
      Serial.println("No metal detected");
      digitalWrite(LED_RED, LOW);
      digitalWrite(LED_GREEN, HIGH);
      digitalWrite(BUZZER, LOW);
      digitalWrite(RELAY, HIGH);
      metalFeed.publish("0");
    } else {
      Serial.println("Metal detected");
      digitalWrite(LED_RED, HIGH);
      digitalWrite(LED_GREEN, LOW);
      digitalWrite(BUZZER, HIGH);
      digitalWrite(RELAY, LOW);
      metalFeed.publish("1");
    }
  }

  delay(2000);
}
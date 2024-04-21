
#include <WiFi.h>
#include <WiFiClient.h>
WiFiClient client;

const char* ssid = "Panda"; // Your WiFi SSID
const char* password = "PulsePioneers1!"; // Your WiFi password
const char* serverAddress = "172.20.10.5"; // IP address of the TCP server
const uint16_t serverPort = 5001; // TCP server port
const int OUTPUT_PIN = 34; // AD8232 output connected to GPIO 26 (ADC0) on the Pi Pico W

const int sampleRate = 250; // Sample rate in Hz
const int bufferSize = 7500; // 60 seconds of data at 250 Hz
float buffer[bufferSize]; // Circular buffer to hold data
int bufferIndex = 0; // Current index in the buffer
bool activationFlag = false; // Flag to indicate activation
unsigned long activationTime = 0; // Time of activation
const long postActivationDuration = 15000; // 30 seconds after activation
unsigned long sampleTime = millis(); // Initialize sample time

// Button setup
const int buttonPin = 12; // The pin number for the button, using GP18

void setup() {
  pinMode(OUTPUT_PIN, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  WiFi.begin(ssid, password);
  Serial.begin(115200);
  blinkLED(2, 250); // Blink twice to indicate setup start

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    blinkLED(1, 500); // Slow blink to indicate trying to connect to WiFi
  }

  blinkLED(3, 100); // Blink 3 times quickly to indicate WiFi connected

  // Initialize the button pin as an input with an internal pull-up resistor
  pinMode(buttonPin, INPUT_PULLUP);
}


void loop() {
  static unsigned long lastSendTime = 0; // Last time data was sent
  const long sendDataInterval = 100; // Interval to send data, e.g., every 100ms

  int sample = analogRead(OUTPUT_PIN); // Read the EKG data from AD8232
  buffer[bufferIndex] = sample; // Store the read sample in the buffer
  bufferIndex = (bufferIndex + 1) % bufferSize;

  // Check for button press for event marking
  checkForEvent(); // This will encapsulate the button press logic

  // Stream data continuously
  if (millis() - lastSendTime >= sendDataInterval) {
    sendData(sample); // Send current sample or a chunk of data
    lastSendTime = millis();
  }

  delay(1000 / sampleRate); // Maintain sample rate to collect data
}

void checkForEvent() {
  if (digitalRead(buttonPin) == LOW && !activationFlag) {
    activationFlag = true;
    activationTime = millis();
    blinkLED(1, 100); // Single blink to indicate button press detected
    delay(100); // Debounce delay
  }
}

void sendData(int sample) {
  // Logic to connect and send current sample or a batch of samples to the server
  // Adjust this to manage connection and ensure efficient data transfer
  if (!client.connected()) {
    client.connect(serverAddress, serverPort);
  }
  client.write((const uint8_t*)&sample, sizeof(sample));

  // Optionally, handle client.stop() wisely to keep the connection open for streaming
}

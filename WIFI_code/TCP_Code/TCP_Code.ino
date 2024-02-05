#include <WiFi.h>
#include <WiFiClient.h>
#include <Arduino.h>

const char* ssid = "Panda"; // Your WiFi SSID
const char* password = "PulsePioneers1!";
const char* serverAddress = "172.20.10.5"; // IP address of the TCP server
const uint16_t serverPort = 5001; // TCP server port

WiFiClient client;

const int sampleRate = 250; // Sample rate in Hz
const int bufferSize = 15000; // 60 seconds of data at 250 Hz (250 samples/second * 60 seconds)
float buffer[bufferSize]; // Circular buffer to hold data
int bufferIndex = 0; // Current index in the buffer
bool activationFlag = false; // Flag to indicate activation
unsigned long activationTime = 0; // Time of activation
const long preActivationDuration = 30000; // 30 seconds before activation to keep
const long postActivationDuration = 30000; // 30 seconds after activation to collect
unsigned long sampleTime = millis(); // Initialize sample time

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("WiFi connected");
}

void loop() {
  // Check for serial command for activation
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    if (command == "start" && !activationFlag) {
      activationFlag = true;
      activationTime = millis();
      digitalWrite(LED_BUILTIN, HIGH); // Indicate activation
      Serial.println("Activation command received.");
    }
  }

  // Generate a pulse waveform as the data sample and store in the buffer
  float sample = generatePulseWaveform();
  buffer[bufferIndex] = sample;
  bufferIndex = (bufferIndex + 1) % bufferSize;

  // After activation, wait for additional 30 seconds before sending data
  if (activationFlag && (millis() - activationTime > postActivationDuration)) {
    sendBufferOverTCP(); // Send data over TCP
    activationFlag = false;
    digitalWrite(LED_BUILTIN, LOW); // Turn off LED after data sending
    Serial.println("Data sent over TCP.");
    bufferIndex = 0; // Reset buffer index for the next activation
  }

  delay(1000 / sampleRate); // Maintain sample rate
}

void sendBufferOverTCP() {
  if (!client.connect(serverAddress, serverPort)) {
    Serial.println("Connection to server failed");
    return;
  }

  Serial.println("Connected to server, sending data...");

  // Calculate the number of bytes to send (each float is 4 bytes)
  int numBytes = bufferSize * sizeof(float);

  // Send the buffer content as binary data
  byte* byteBuffer = (byte*)buffer; // Cast the float buffer to a byte pointer
  
  int bytesSent = 0; // Track the number of bytes sent
  while (bytesSent < numBytes) {
    // Send the data in chunks to avoid overflowing the device's send buffer
    int chunkSize = min(512, numBytes - bytesSent); // Adjust chunk size as needed
    client.write(&byteBuffer[bytesSent], chunkSize);
    bytesSent += chunkSize;
  }

  client.stop(); // Close the connection
  Serial.println("Data sent over TCP.");
}

float generatePulseWaveform() {
  // Simulate pulse waveform with a varying sine wave to mimic heartbeats
  unsigned long currentTime = millis();
  float timeSinceStart = (currentTime - sampleTime) / 1000.0; // Time in seconds
  float pulseFrequency = 1.0; // Pulse frequency in Hz, adjust for different heart rates
  // Generate waveform
  float waveform = sin(2 * PI * pulseFrequency * timeSinceStart);
  // Modulate waveform to simulate a pulse
  waveform = waveform * exp(-modf(timeSinceStart, &pulseFrequency) * 4); // Decay factor to simulate pulse shape
  return waveform;
}

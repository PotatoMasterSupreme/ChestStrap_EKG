#include <WiFi.h>
#include <WiFiUDP.h>
#include <Arduino.h>

const char* ssid = ""; // Your WiFi SSID
const char* password = "";
const char* serverAddress = "192.168.68.124"; // IP address of the UDP server
const uint16_t serverPort = 5001; // UDP server port
unsigned long sampleTime = millis(); // Initialize sample time

WiFiUDP udp;

const int sampleRate = 250; // Sample rate in Hz

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
  // Generate a pulse waveform as the data sample
  float sample = generatePulseWaveform();

  // Immediately send this data sample over UDP
  sendSampleOverUDP(sample);

  // Maintain sample rate
  delay(1000 / sampleRate);
}

void sendSampleOverUDP(float sample) {
  udp.beginPacket(serverAddress, serverPort);

  // Convert the float sample to a byte array
  byte* sampleBytes = (byte*)&sample;
  
  // Send the sample as binary data
  udp.write(sampleBytes, sizeof(sample));
  udp.endPacket();
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

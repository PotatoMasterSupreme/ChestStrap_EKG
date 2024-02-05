#include <Arduino.h>

const int sampleRate = 250; // Sample rate in Hz
const int bufferSize = 15000; // 30 seconds of data at 250 Hz
float buffer[bufferSize]; // Circular buffer to hold data
int bufferIndex = 0; // Current index in the buffer
bool activationFlag = false; // Flag to indicate activation
unsigned long activationTime = 0; // Time of activation
const long postActivationDuration = 30000; // 30 seconds after activation
unsigned long sampleTime = 0; // Track time for sample generation

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // Generate a pulse waveform as the data sample
  float sample = generatePulseWaveform();

  // Continuously store samples in the circular buffer
  buffer[bufferIndex] = sample;
  bufferIndex = (bufferIndex + 1) % bufferSize; // Wrap index if at end of buffer

  // Check for serial command for activation
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    if (command == "start") {
      activationFlag = true;
      activationTime = millis();
      digitalWrite(LED_BUILTIN, HIGH); // Indicate activation
    }
  }

  // After activation, collect for additional 30 seconds
  if (activationFlag && millis() - activationTime <= postActivationDuration) {
    // Keep collecting data
  } else if (activationFlag) {
    activationFlag = false;
    digitalWrite(LED_BUILTIN, LOW); // Turn off LED after data collection
    // Data collection complete, process or send data here
    sendBufferOverSerial();
  }

  // Maintain sample rate
  delay(1000 / sampleRate);
}

void sendBufferOverSerial() {
  // Example function to send the buffer over Serial
  for (int i = 0; i < bufferSize; i++) {
    Serial.println(buffer[(bufferIndex + i) % bufferSize], 6); // Adjust precision as needed
  }
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

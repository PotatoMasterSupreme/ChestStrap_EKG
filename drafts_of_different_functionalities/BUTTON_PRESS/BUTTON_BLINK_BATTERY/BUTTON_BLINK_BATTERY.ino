#include <WiFi.h>
#include <WiFiClient.h>

const char* ssid = "Mango"; // Your WiFi SSID
const char* password = "YashicaMat1!"; // Your WiFi password
const char* serverAddress = "192.168.68.118"; // IP address of the TCP server
const uint16_t serverPort = 5001; // TCP server port

WiFiClient client;

const int sampleRate = 250; // Sample rate in Hz
const int bufferSize = 15000; // 60 seconds of data at 250 Hz
float buffer[bufferSize]; // Circular buffer to hold data
int bufferIndex = 0; // Current index in the buffer
bool activationFlag = false; // Flag to indicate activation
unsigned long activationTime = 0; // Time of activation
const long postActivationDuration = 30000; // 30 seconds after activation
unsigned long sampleTime = millis(); // Initialize sample time

// Button setup
const int buttonPin = 18; // The pin number for the button, using GP18

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  WiFi.begin(ssid, password);

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
  // Check if the button has been pressed. Since we're using INPUT_PULLUP, button press is LOW
  if (digitalRead(buttonPin) == LOW && !activationFlag) {
    activationFlag = true;
    activationTime = millis();
    blinkLED(1, 100); // Single blink to indicate button press detected
    delay(100); // Debounce delay
  }
  
  float sample = generatePulseWaveform();
  buffer[bufferIndex] = sample;
  bufferIndex = (bufferIndex + 1) % bufferSize;

  // After activation, wait for additional 30 seconds before sending data
  if (activationFlag && (millis() - activationTime > postActivationDuration)) {
    if(sendBufferOverTCP()) { // If data sent successfully
      blinkLED(2, 100); // Blink twice quickly to indicate data sent
    } else {
      blinkLED(5, 200); // Blink 5 times slowly to indicate failure
    }
    activationFlag = false;
    bufferIndex = 0; // Reset buffer index for the next activation
  }

  delay(1000 / sampleRate); // Maintain sample rate to collect data
}

bool sendBufferOverTCP() {
  if (!client.connect(serverAddress, serverPort)) {
    return false;
  }

  client.write((const uint8_t*)buffer, bufferSize * sizeof(float));
  client.stop();
  return true;
}

void blinkLED(int count, int delayTime) {
  for(int i = 0; i < count; i++) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(delayTime);
    digitalWrite(LED_BUILTIN, LOW);
    delay(delayTime);
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

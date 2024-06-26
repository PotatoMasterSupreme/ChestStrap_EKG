#include <WiFi.h>
#include <WiFiClient.h>
WiFiClient client;

const char* ssid = "Mango"; // Your WiFi SSID
const char* password = "YashicaMat1!"; // Your WiFi password
const char* serverAddress = "192.168.68.103"; // IP address of the TCP server
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
  if (!SPIFFS.begin(true)) {
    blinkkLED(10,100);
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
}
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
  
  int sample = analogRead(OUTPUT_PIN);// Read the EKG data from AD8232
  buffer[bufferIndex] = sample; // Store the read sample in the buffer

  bufferIndex = (bufferIndex + 1) % bufferSize;

  // After activation, wait for additional 30 seconds before sending data
  if (activationFlag && (millis() - activationTime > postActivationDuration)) {
    if(sendBufferOverTCP()) { // If data sent successfully
      blinkLED(2, 100); // Blink twice quickly to indicate data sent
    } else {
      blinkLED(5, 200); // Blink 5 times slowly to indicate failure
    }
    saveData();
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


void saveData(float *buffer, int bufferSize) {
  File file = SPIFFS.open("/data.txt", FILE_WRITE);
  if (!file) {
    Serial.println("There was an error opening the file for writing");
    blinkLED(4,100);
    return;
  }
  for (int i = 0; i < bufferSize; i++) {
    file.println(buffer[i]);
  }
  file.close();
}

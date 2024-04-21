#include <WiFi.h>
#include <WiFiClient.h>
#include <SPI.h>
#include <SD.h>
#include <Preferences.h>
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

// Define the pin numbers for the SPI interface to the SD card
#define CS_PIN    5  // DATA 3/CS
#define MOSI_PIN  23 // CMD/PICO (Peripheral's SDI)
#define MISO_PIN  19 // DATA 0/POCI (Peripheral's SDO)
#define SCK_PIN   18 // CLK/SCK

Preferences preferences;
unsigned int fileIndex = 0; // File index counter for unique filenames



void setup() {
  Serial.begin(115200);
  while (!Serial); // Wait for the Serial monitor to open
  
  // Initialize SPI with the specified pins
  SPI.begin(SCK_PIN, MISO_PIN, MOSI_PIN, CS_PIN);
  preferences.begin("ekg", false); // Open in RW-mode
  fileIndex = preferences.getUInt("fileIndex", 0);
  // Attempt to initialize the SD card
  if (!SD.begin(CS_PIN)) {
    Serial.println("Card Mount Failed");
    return;
  }
  Serial.println("SD Card initialized successfully.");
  pinMode(LED_BUILTIN, OUTPUT);
  WiFi.begin(ssid, password);
  pinMode(OUTPUT_PIN, INPUT);
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
  
  int sample = analogRead(OUTPUT_PIN);// Read the EKG data from AD8232
  buffer[bufferIndex] = sample; // Store the read sample in the buffer

  bufferIndex = (bufferIndex + 1) % bufferSize;

  // After activation, wait for additional 30 seconds before sending data
  if (activationFlag && (millis() - activationTime > postActivationDuration)) {
    if(sendBufferOverTCP()) { // If data sent successfully
      blinkLED(2, 100); // Blink twice quickly to indicate data sent
    } else {
      blinkLED(5, 300); // Blink 5 times slowly to indicate failure
    }
    WriteBufferToSDCard(); // local backup 
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

void WriteBufferToSDCard() {
  // Ensure SD card is initialized only once in setup() and not here to avoid re-initialization issues
  // Generate a unique filename for the data
  String filename = "/ekgData_" + String(fileIndex) + ".csv";
  
  // Open the file for writing
  File file = SD.open(filename.c_str(), FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file for writing: " + filename);
    return; // Exit the function if file cannot be opened
  }

  // Write EKG readings to the file with minimal write operations
  String dataBuffer = ""; // Buffer to accumulate data
  const int BUFFER_LIMIT = 50; // Number of readings to accumulate before writing

  for (int i = 0; i < bufferIndex; i++) {
    // Append current reading to buffer, format as CSV
    dataBuffer += String(buffer[i]) + ",\n"; // Adding a comma for CSV formatting
    
    // Write and clear buffer after every BUFFER_LIMIT readings or at the last reading
    if ((i + 1) % BUFFER_LIMIT == 0 || i == bufferIndex - 1) {
      file.print(dataBuffer);
      dataBuffer = ""; // Clear the buffer
    }
  }
  
  file.close(); // Ensure the file is closed after writing
    Serial.println("EKG data written to SD card: " + filename);
    
    // Update the file index for the next data write operation and store it
    fileIndex++;
    preferences.putUInt("fileIndex", fileIndex);
}
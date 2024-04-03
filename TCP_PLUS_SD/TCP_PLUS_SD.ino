#include <WiFi.h>
#include <WiFiClient.h>
#include <Arduino.h>
#include <Preferences.h>
#include <SD.h> 
#include <SPI.h>


const char* ssid = "Mango"; // Your WiFi SSID
const char* password = "YashicaMat1!";
const char* serverAddress = "192.168.68.124"; // IP address of the TCP server
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
    WriteBufferToSDCard(); // store buffer on sd
    activationFlag = false;
    digitalWrite(LED_BUILTIN, LOW); // Turn off LED after data sending
    Serial.println("Data sent over TCP.");
    
    bufferIndex = 0; // Reset buffer index for the next activation
  }

  delay(1000 / sampleRate); // Maintain sample rate
}

bool sendBufferOverTCP() {
  if (!client.connect(serverAddress, serverPort)) {
    return false;
  }

  client.write((const uint8_t*)buffer, bufferSize * sizeof(float));
  client.stop();
  return true;
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

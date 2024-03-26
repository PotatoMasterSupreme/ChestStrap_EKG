#include <SPI.h>
#include <SD.h>

const uint8_t chipSelect = 5; // Adjust this to your SD card module's CS pin
const char* filename = "data.bin"; // File name for the data

float buffer[15000]; // Your data buffer

void setup() {
  Serial.begin(115200);
  while (!Serial); // Wait for the serial monitor

  // Fill the buffer with example data (for demonstration purposes)
  for (int i = 0; i < 15000; ++i) {
    buffer[i] = i * 0.1; // Example data
  }
  
  // Initialize SD card
  if (!SD.begin(chipSelect)) {
    Serial.println("SD card initialization failed! Is the card inserted properly?");
    return; // Stop here on failure
  }
  Serial.println("SD card initialized.");
  delay(1000);

  // Open the file for writing
  File file = SD.open(filename, FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file for writing. Check if the file is already open.");
    return; // Stop here on failure
  }

  // Write the buffer to the file in binary format
  size_t bytesWritten = file.write((const uint8_t*)buffer, sizeof(buffer));
  if (bytesWritten != sizeof(buffer)) {
    Serial.println("Error writing to file. Possible cause: not enough space or file system error.");
    // Optionally, you could handle the error more gracefully here
  } else {
    Serial.println("Buffer written to SD card successfully.");
  }

  // Close the file
  file.close();

  Serial.println("File closed successfully.");
}

void loop() {
  // Nothing to do here
}

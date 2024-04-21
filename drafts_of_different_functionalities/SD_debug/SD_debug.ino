#include <SPI.h>
#include <SD.h>

// Define the pin numbers for the SPI interface to the SD card
#define CS_PIN    5  // DATA 3/CS
#define MOSI_PIN  23 // CMD/PICO (Peripheral's SDI)
#define MISO_PIN  19 // DATA 0/POCI (Peripheral's SDO)
#define SCK_PIN   18 // CLK/SCK

// Define an array of numbers to write to the SD card
int numbers[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

void setup() {
  Serial.begin(115200);
  while (!Serial); // Wait for the Serial monitor to open
  
  // Initialize SPI with the specified pins
  SPI.begin(SCK_PIN, MISO_PIN, MOSI_PIN, CS_PIN);

  // Attempt to initialize the SD card
  if (!SD.begin(CS_PIN)) {
    Serial.println("Card Mount Failed");
    return;
  }
  Serial.println("SD Card initialized successfully.");

  // Open file for writing
  File file = SD.open("/numbers.txt", FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }
  
  // Write array elements to the file as text
  for (int i = 0; i < 10; i++) {
    file.println(numbers[i]);
  }
  
  file.close(); // Make sure to close the file when you're done
  Serial.println("Number array written to SD card.");
}

void loop() {
  // Nothing to do here
}

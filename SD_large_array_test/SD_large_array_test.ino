#include <SPI.h>
#include <SD.h>

// Define the pin numbers for the SPI interface to the SD card
#define CS_PIN    5  // DATA 3/CS
#define MOSI_PIN  23 // CMD/PICO (Peripheral's SDI)
#define MISO_PIN  19 // DATA 0/POCI (Peripheral's SDO)
#define SCK_PIN   18 // CLK/SCK

const int TOTAL_READINGS = 15000; // Total number of readings
int ekgReadings[TOTAL_READINGS]; // Array to store EKG readings

void setup() {
  Serial.begin(115200);
  while (!Serial); // Wait for Serial monitor to connect
  
  // Initialize SPI
  SPI.begin(SCK_PIN, MISO_PIN, MOSI_PIN, CS_PIN);

  // Initialize SD card
  if (!SD.begin(CS_PIN)) {
    Serial.println("Card Mount Failed");
    return;
  }
  Serial.println("SD Card initialized.");

  // Open file for writing
  File file = SD.open("/ekg_data.csv", FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file for writing.");
    return;
  }
  
  // Fill the ekgReadings array
  for (int i = 0; i < TOTAL_READINGS; i++) {
    ekgReadings[i] = i; // Simulate EKG readings
  }

  // Write EKG readings to the file with minimal write operations
  String dataBuffer = ""; // Buffer to accumulate data
  const int BUFFER_LIMIT = 50; // Number of readings to accumulate before writing

  for (int i = 0; i < TOTAL_READINGS; i++) {
    // Append current reading to buffer, format as CSV
    dataBuffer += String(ekgReadings[i]) + ",\n"; // Adding a comma for CSV formatting
    
    // Write and clear buffer after every BUFFER_LIMIT readings or at the last reading
    if ((i + 1) % BUFFER_LIMIT == 0 || i == TOTAL_READINGS - 1) {
      file.print(dataBuffer);
      dataBuffer = ""; // Clear the buffer
    }
  }
  
  file.close(); // Make sure to close the file
  Serial.println("EKG data written to SD card.");
}

void loop() {
  // Nothing here
}

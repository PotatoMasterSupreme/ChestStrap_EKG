#include <Arduino.h>
#include "LittleFS.h"

const int sampleRate = 250;  // Sample rate in Hz
const float frequency = 1.0; // Frequency of sine wave, more representative of heart rate
const int ledPin = LED_BUILTIN; // Built-in LED for indicating data write operation

unsigned long previousMillis = 0; 
const long interval = 60000; // interval at which to save data (1 minute)
int dataIndex = 0;

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);

  if (!LittleFS.begin()) {
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }
}

void loop() {
  unsigned long currentMillis = millis();

  // Check if a minute has passed
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // Indicate start of file write operation
    digitalWrite(ledPin, HIGH);

    // Save data to a file in LittleFS
    saveData();

    // Indicate end of file write operation
    digitalWrite(ledPin, LOW);

    // Reset data index for next cycle
    dataIndex = 0;
  } else if (dataIndex < sampleRate * 60) { // Collect data for 1 minute
    // Generate sine wave data
    float value = sin(2 * PI * frequency * dataIndex / sampleRate);
    writeDataPoint(value);
    dataIndex++;

    // Small delay to simulate real-time data collection
    delay(1000 / sampleRate);
  }
}

void writeDataPoint(float value) {
  File file = LittleFS.open("/data.csv", dataIndex == 0 ? "w" : "a");

  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }

  file.printf("%f\n", value);
  file.close();
}

void saveData() {
  // Implement any additional operations required when saving data
  // For now, this function is just a placeholder
}

// Define the pin connected to the output of the AD8232
const int AD8232OutputPin = A0; // Assuming the AD8232's output is connected to analog pin A0

void setup() {
  // Initialize Serial communication at a baud rate of 115200:
  Serial.begin(115200);
  // Setup the AD8232 output pin as an input
  pinMode(AD8232OutputPin, INPUT);
}

void loop() {
  // Read the analog value from the AD8232 output pin
  int sensorValue = analogRead(AD8232OutputPin);
  
  // Print the value to the Serial port
  Serial.println(sensorValue);
  
  // Wait a bit for the next reading. Adjust the delay to match the desired sampling rate.
  // For example, a delay of 4 milliseconds corresponds to a sampling rate of 250 samples/second (1/0.004)
  delay(4);
}

#include <FastLED.h>

// Define the constants
const int reedSwitchPin = A0; // Reed switch connected to A0 pin
const int ledPin = 6; // WS2812B LEDs connected to pin 6
const int numLeds = 120; // Total number of LEDs
const float maxDistance = 500.0; // Maximum distance in meters
const int updateInterval = 500; // Update interval in milliseconds

// Define the variables
volatile float distance = 0.0; // Distance covered
volatile float distance1 = 0.0; // Distance covered in 1 cycle

volatile float speed = 0.0; // Instantaneous speed
unsigned long lastUpdateTime = 0; // Time of last update

// Define the FastLED object
CRGB leds[numLeds];

void setup() {
  Serial.begin(9600); // Initialize the serial monitor
  pinMode(reedSwitchPin, INPUT_PULLUP); // Enable the internal pull-up resistor
  FastLED.addLeds<WS2812B, ledPin, GRB>(leds, numLeds); // Initialize the LEDs
}

void loop() {
  // Calculate the distance covered
  float circumference = 1.0; // Circumference of the tire in meters
  int count = 0; // Number of revolutions
  int sensorValue = analogRead(reedSwitchPin); // Read the sensor value
  if (sensorValue < 500) { // The reed switch is closed
    count++;
    while (analogRead(reedSwitchPin) < 500) {} // Wait for the switch to open
  }
  distance += circumference * count;
  distance1 = circumference * count;
  // Calculate the instantaneous speed
  unsigned long now = millis(); // Current time in milliseconds
  unsigned long timeElapsed = now - lastUpdateTime; // Time elapsed since last update
  if (timeElapsed >= updateInterval) {
    speed = distance1 / (timeElapsed / 1000.0);
    lastUpdateTime = now;
  }
 
  // Update the LEDs
  int numLitLeds = (int) (distance / maxDistance * numLeds);
  for (int i = 0; i < numLitLeds; i++) {
    leds[i] = CRGB::Green;
  }
  for (int i = numLitLeds; i < numLeds; i++) {
    leds[i] = CRGB::Black;
  }
  FastLED.show();
 
  // Print the speed and distance to the serial monitor
  //if (now % updateInterval == 0) {
    Serial.print("Speed = ");
    Serial.print(speed);
    Serial.print(" m/s, Distance = ");
    Serial.print(distance);
    Serial.println(" m");
  //}
}

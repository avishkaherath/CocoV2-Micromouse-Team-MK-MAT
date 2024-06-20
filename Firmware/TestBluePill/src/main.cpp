#include <Arduino.h>
// #include "ICMSensor.h"

// ICMSensor sensor;

// #define LED_PIN PC13

// void setup() {
//   sensor.setup();
//   // Initialize the digital pin as an output.
//   pinMode(LED_PIN, OUTPUT);
// }

// void loop() {
//     sensor.loop();
//   // Turn the LED on
//   digitalWrite(LED_PIN, LOW);   // Note: The LED is active low on the BluePill
//   delay(500);                  // Wait for half a second
  
//   // Turn the LED off
//   digitalWrite(LED_PIN, HIGH);
//   delay(500);                  // Wait for half a second
// }

#include <HardwareSerial.h>

HardwareSerial Serial2(PA3, PA2); // RX, TX

void setup() {
  // Initialize Serial2 at 115200 bps
  Serial2.begin(115200);
}

void loop() {
  // Print a message every second
  Serial2.println("Hello from UART2!");
  delay(1000); // Delay for 1000 milliseconds (1 second)
}
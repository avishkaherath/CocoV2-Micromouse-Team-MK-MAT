#include <Arduino.h>

// Define the pins connected to the encoder
const int encoderPinA = D1;  // D1 on ESP8266
const int encoderPinB = D2;  // D2 on ESP8266

volatile long encoderTicks = 0;  // Variable to store encoder ticks
volatile int lastEncoded = 0;   // Last encoded signal
long lastReportedTicks = 0;     // Last reported encoder ticks

// Interrupt Service Routine (ISR) for handling encoder changes
void IRAM_ATTR handleInterrupt() {
  int MSB = digitalRead(encoderPinA);  // Read the most significant bit
  int LSB = digitalRead(encoderPinB);  // Read the least significant bit

  int encoded = (MSB << 1) | LSB;  // Convert the 2 pin value to a single number
  int sum = (lastEncoded << 2) | encoded;  // Combine it with the previous encoded value

  // Determine the direction of the rotation
  if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) encoderTicks++;
  if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) encoderTicks--;

  lastEncoded = encoded;  // Store this value for next time
}

void setup() {
  Serial.begin(115200);  // Start serial communication at 115200 baud
  pinMode(encoderPinA, INPUT_PULLUP);  // Set encoder pins as inputs with pull-up resistors
  pinMode(encoderPinB, INPUT_PULLUP);

  // Attach interrupts to both encoder pins
  attachInterrupt(digitalPinToInterrupt(encoderPinA), handleInterrupt, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoderPinB), handleInterrupt, CHANGE);
}

void loop() {
  // Disable interrupts to read encoderTicks atomically
  noInterrupts();
  long currentTicks = encoderTicks;
  interrupts();

  // Check if the current encoder ticks have changed since last reported
  if (currentTicks != lastReportedTicks) {
    Serial.print("Encoder Ticks: ");
    Serial.println(currentTicks);
    lastReportedTicks = currentTicks;  // Update last reported to the current
  }
}
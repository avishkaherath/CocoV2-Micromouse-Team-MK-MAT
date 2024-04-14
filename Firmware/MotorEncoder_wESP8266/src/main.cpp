#include <Arduino.h>

// Define the pins connected to the encoder
const int encoderPinA = D1;  // D1 on ESP8266
const int encoderPinB = D2;  // D2 on ESP8266

volatile long encoderTicks = 0;  // Variable to store encoder ticks
volatile int lastEncoded = 0;   // Last encoded signal

// This function is called every time a change occurs on encoderPinA or encoderPinB
void IRAM_ATTR handleInterrupt() {
  int MSB = digitalRead(encoderPinA);  // MSB = most significant bit
  int LSB = digitalRead(encoderPinB);  // LSB = least significant bit

  int encoded = (MSB << 1) | LSB;      // converting the 2 pin value to single number
  int sum = (lastEncoded << 2) | encoded; // adding it to the previous encoded value

  if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) encoderTicks++;
  if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) encoderTicks--;

  lastEncoded = encoded; //store this value for next time
}

void setup() {
  Serial.begin(9600);    // Start serial communication at 9600 baud
  pinMode(encoderPinA, INPUT_PULLUP);  // Set encoder pins as inputs
  pinMode(encoderPinB, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(encoderPinA), handleInterrupt, CHANGE);  // Attach interrupt
  attachInterrupt(digitalPinToInterrupt(encoderPinB), handleInterrupt, CHANGE);  // Attach interrupt for encoderPinB as well
}

void loop() {
  // Print the number of encoder ticks
  Serial.print("Encoder Ticks: ");
  Serial.println(encoderTicks);

  delay(1000);  // Update every second
}
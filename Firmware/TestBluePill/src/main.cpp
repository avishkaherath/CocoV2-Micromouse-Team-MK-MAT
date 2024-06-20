#include <Arduino.h>
#include "ICM_20948.h" // Ensure you have included the right library

#define SERIAL_PORT Serial
#define WIRE_PORT Wire
#define AD0_VAL 0 // Set to 0 for 0x68 address

ICM_20948_I2C myICM; // Create an ICM_20948_I2C object since we are using I2C

void setup() {
  SERIAL_PORT.begin(115200); // Start the serial console
  while (!SERIAL_PORT);      // Wait for serial port to connect - necessary for Leonardo only
  SERIAL_PORT.println(F("Starting ICM-20948 I2C communication example"));

  WIRE_PORT.begin();
  WIRE_PORT.setClock(100000); // Use 400 kHz I2C speed

  // Initialize the ICM-20948
  myICM.begin(WIRE_PORT, AD0_VAL);

  if (myICM.status != ICM_20948_Stat_Ok) {
    SERIAL_PORT.print(F("ICM-20948 initialization error: "));
    SERIAL_PORT.println(myICM.statusString());
    while (1); // Hold here to check the error
  }

  SERIAL_PORT.println(F("ICM-20948 initialization successful!"));
}

void loop() {
  // Check for new data in the FIFO
  if (myICM.dataReady()) {
    myICM.getAGMT(); // Updates the sensor data: accel, gyro, mag, temp

    // SERIAL_PORT.print(F("Accel: "));
    // SERIAL_PORT.print(myICM.accX());
    // SERIAL_PORT.print(F(", "));
    // SERIAL_PORT.print(myICM.accY());
    // SERIAL_PORT.print(F(", "));
    // SERIAL_PORT.println(myICM.accZ());

    SERIAL_PORT.print(F("Gyro: "));
    SERIAL_PORT.print(myICM.gyrX());
    SERIAL_PORT.print(F(", "));
    SERIAL_PORT.print(myICM.gyrY());
    SERIAL_PORT.print(F(", "));
    SERIAL_PORT.println(myICM.gyrZ());

    // SERIAL_PORT.print(F("Mag: "));
    // SERIAL_PORT.print(myICM.magX());
    // SERIAL_PORT.print(F(", "));
    // SERIAL_PORT.print(myICM.magY());
    // SERIAL_PORT.print(F(", "));
    // SERIAL_PORT.println(myICM.magZ());

    // SERIAL_PORT.print(F("Temp: "));
    // SERIAL_PORT.println(myICM.temp());
    delay(100); // Wait for 10 milliseconds if no data is available
  }
}

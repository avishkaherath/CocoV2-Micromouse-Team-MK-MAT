#include <Arduino.h>
#include "motors.h"

// put function declarations here:
Motor leftM(A2, A3);
Motor rightM(A4, A5);

void setup() {
  // put your setup code here, to run once:
  leftM.Setup();
  rightM.Setup();
  Serial.begin(9600);
  leftM.forward(135);
  rightM.forward(135);
  delay(1000);
  // leftM.stop();
  // rightM.stop();
  // pinMode(A6,OUTPUT);
  // pinMode(A7,OUTPUT);
  // digitalWrite(A6,LOW);
  // digitalWrite(A7,HIGH);
}

void loop() {
  // for (int i = 255; i>0; i=i-10){
  //   leftM.forward(i);
  //  rightM.forward(i);
  //  Serial.println(i);
  //  delay(500);
  // }
}


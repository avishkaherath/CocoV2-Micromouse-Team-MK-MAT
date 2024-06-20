#include "motors.h"

Motor::Motor(int in1, int in2){
    IN1 = in1;
    IN2 = in2;
}

void Motor::Setup(){
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
}

void Motor::forward(int speed){
    digitalWrite(IN1, LOW);
    analogWrite(IN2,speed);
}

void Motor::backward(int speed){
    digitalWrite(IN2, LOW);
    analogWrite(IN1,speed);
}

void Motor::stop(){
    digitalWrite(IN2, HIGH);
    digitalWrite(IN1, HIGH);
}

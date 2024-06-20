#include <Arduino.h>

class Motor{
    public:
    int IN1;
    int IN2;
    Motor(int in1, int in2);
    void Setup();
    void forward(int speed);
    void backward(int speed);
    void stop();
};

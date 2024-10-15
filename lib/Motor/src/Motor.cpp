#include <Arduino.h>
#include <Motor.h>

Motor::Motor(int pinAA, int pinBB, float deathZone){
    pinA = pinAA;
    pinB = pinBB;
    rotation = CLOCKWISE;
    deathzone = deathZone;
    power = 0;
};

void Motor::setPower(void){
    setPower(power);
};

float Motor::getPower(void){
    return power;
};

void Motor::setPower(float value){
    float maped_power;
    if(value <= 0){
        power = 0;
        analogWrite(pinA, 0);
        analogWrite(pinB, 0);
    }
    else {
        if(value > 100) {value = 100;}
        power = value;
        maped_power = map(value, 0, 100, deathzone, 1024);
        writePins(maped_power);
    }
};

void Motor::writePins(float value){
    switch(rotation) {
        case CLOCKWISE:
            analogWrite(pinA, value);
            analogWrite(pinB, 0);
            break;
        case COUNTERCLOCKWISE:
            analogWrite(pinA, 0);
            analogWrite(pinB, value);
            break;
    }
};

void Motor::setRotation(Rotations nRotation){
    rotation = nRotation;
    float maped_power = map(power, 0, 100, deathzone, 1024);
    writePins(maped_power);
};

void Motor::invertRotation(void){
    Rotations temp;
    switch (rotation) {
        case CLOCKWISE:
            temp = COUNTERCLOCKWISE;
            break;
        case COUNTERCLOCKWISE:
            temp = CLOCKWISE;
            break;
        default:
            break;
    }
    rotation = temp;
    setRotation(rotation);
};

void Motor::brake(void){

    analogWrite(pinA, 1024);
    analogWrite(pinB, 1024);
};
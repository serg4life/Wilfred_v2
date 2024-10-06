#include <PID.h>

PIDController::PIDController(double Kp_, double Ki_, double Kd_, double a_){
    Kp = Kp_;    
    Ki = Ki_;
    Kd = Kd_;
    a = a_;

    Kr = sqrt(Ki*Kd);

    reference = NULL;
    saturation = 100;
    lastInput = 0;
    derivative = 0 ;
    integral = 0;
    lastError = 0;
    lastRef = 0;
    lastU = 0;
    es = 0;
};

void PIDController::updateInput(double newInput){
    input = newInput;
};

void PIDController::setReference(double newRef){
    reference = newRef;
};

void PIDController::setSaturation(double newSat){
    saturation = newSat;
};

double PIDController::getOutput(){
    return u;
};

void PIDController::compute(double deltaTime){
    if(reference == NULL){
        return;
    }
    double i_control;
    double error = reference - input;
    //derivative = (error - lastError) / deltaTime; NORMAL DERIVATIVE
    derivative = (1 - a) * ((reference - a * lastRef) - (input - a * lastInput)) / deltaTime;


    //Anti-Windup
    if(lastU > saturation){
        es = saturation - lastU;
        integral += (Ki * error + Kr * es) * deltaTime;
        i_control = integral;
    } else {
        integral += error * deltaTime;
        i_control = Ki * integral;
    }

    u = Kp * derivative + i_control;

    //LIMITAR LA TENSION
    if(u > saturation){
        u = saturation;
    } else if (u < -saturation) {  //CUIDADO QUE LA CLASE DE MOTORES ES DIFERENTE
        u = -saturation;
    }

    //ACTUALIZAR LOS VALORES PREVIOS
    lastError= error;
    lastInput = input;
    lastRef = reference;
    lastU = u;
};

float PIDController::checkAngle(float angle) {
    float new_angle;
    if (angle > 180){
        new_angle = -(360 - angle);
    } else {
        new_angle = angle;
    }
    return new_angle;
};


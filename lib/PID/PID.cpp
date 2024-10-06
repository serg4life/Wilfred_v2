#include <PID.h>

PID_Controller::PID_Controller(float Kp_par, float Ki_par, float Kd_par, float a_par, float ref_par, float start_meassure, float sat_par, bool filter_par) : filterObject(5, 0.005) {
    Kp = Kp_par;
    Ki = Ki_par;
    Kd = Kd_par;
    a = a_par;
    filter = filter_par;

    Kr = sqrt(Ki*Kd);

    ref = ref_par;
    sat = sat_par;
    meassure_prev = start_meassure;
    derivative = 0 ;
    integral = 0;
    error_prev = 0;
    ref_prev = 0;
    u_prev = 0;
    es = 0;
    b = 0;
    c = 0;
};

float PID_Controller::update(float meassure, float dt){
    float u;
    float i_control;
    float error = ref - meassure;
    //derivative = (error - error_prev) / dt; NORMAL DERIVATIVE
    derivative = (1 - a) * (c * (ref - a * ref_prev) - (meassure - a * meassure_prev)) / dt;

    //Anti-Windup
    if(u_prev > sat){
        es = sat - u_prev;
        integral += (Ki * error + Kr * es) * dt;
        i_control = integral;
    } else {
        integral += error * dt;
        i_control = Ki * integral;
    }

    //Calcular la señal de control
    if(filter){
        derivative = filterObject.getValue(derivative);
    }
    u = Kp * derivative + i_control;

    //LIMITAR LA TENSION
    if(u > sat){
        u = sat;
    } else if (u < -sat) {  //CUIDADO QUE LA CLASE DE MOTORES ES DIFERENTE
        u = -sat;
    }

    //ACTUALIZAR LOS VALORES PREVIOS
    error_prev = error;
    meassure_prev = meassure;
    ref_prev = ref;
    u_prev = u;
};

float PID_Controller::checkAngle(float angle) {
    float new_angle;
    if (angle > 180){
        new_angle = -(360 - angle);
    } else {
        new_angle = angle;
    }
    return new_angle;
};

PID_Filter::PID_Filter(float fc, float T){
    v0_prev = 0;
    vi_prev = 0;
    a = exp(-T*2*M_PI*fc);
};

float PID_Filter::getValue(float data){
    float v0 = v0_prev * a + vi_prev * (1 - a);
    v0_prev = v0;
    vi_prev = data;
    return v0;
};

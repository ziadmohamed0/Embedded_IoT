#include "pid.h"

PID::PID(float p, float i, float d) :
            kp(p),
            ki(i),
            kd(d),
            error(0),
            integral(0),
            derivative(0),
            prevError(0) {}

float PID::updatePID(float setPoint, float measurment, float dt) {
    this->error = setPoint - measurment;

    this->integral += error * dt;

    this->derivative  = (error - this->prevError) / dt;

    float output = ((this->kp * error) +
                    (this->ki * this->integral) +
                    (this->kd * derivative));

    this->prevError = error;
    
    return output;
}
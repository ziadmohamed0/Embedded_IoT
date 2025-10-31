#ifndef _PID_H_
#define _PID_H_

#include "common.h"

class PID {
    public:
        PID(float p, float i, float d);
        float updatePID(float setPoint, float measurment, float dt);
        ~PID() = default;
    private:
        float kp;
        float ki;
        float kd;
        float error;
        float integral;
        float derivative;
        float prevError;
};

#endif
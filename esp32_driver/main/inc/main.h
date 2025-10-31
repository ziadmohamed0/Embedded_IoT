#ifndef MAIN_H_
#define MAIN_H_

#include "led.h"
#include "button.h"
#include "dc_motor.h"
#include "TCRT5000.h"
#include "MPU6050.h"
#include "ultrasonic.h"
#include "servo.h"
#include "bts7960.h"
#include "bts7960_position.h"
#include "pot.h"
#include "pid.h"
#include "MQTT.h"
#include "NVS.h"
#include "WiFi.h"

extern const char *TAG;

// WiFi configurations
#define ssid                    "Mohamed Fathy"
#define password                "341978341978"

// MQTT configurations
#define mqtt_broker             "mqtt://192.168.100.25"
#define mqtt_port               1883

// topics of right hand
#define topic_right_shoulder1   "arm_right/shoulder1"
#define topic_right_shoulder2   "arm_right/shoulder2"
// #define topic_right_elbow       "arm_right/elbow"
// #define topic_right_wrist       "arm_right/wrist"

// topics of left hand
#define topic_left_shoulder1   "arm_left/shoulder1"
#define topic_left_shoulder2   "arm_left/shoulder2"
#define topic_left_elbow       "arm_left/elbow"
#define topic_left_wrist       "arm_left/wrist"

// topic of rover dc motors
#define topic_forward           "move/forward"
#define topic_backward          "move/backward"
#define topic_right             "move/right"
#define topic_left              "move/left"

// gripper topics
#define topic_gripper_close     "gripper/close"
#define topic_gripper_open      "gripper/open"

// prototyping functions
void setup_wifi();
void setup_mqtt();
void setup_HW();

#endif
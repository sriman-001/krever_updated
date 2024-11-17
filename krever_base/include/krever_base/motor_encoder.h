#ifdef __cplusplus
extern "C" {
#endif

#ifndef __MOTOR_ENCODER_H__
#define __MOTOR_ENCODER_H__

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <wiringPi.h>

// Define GPIO pins for the encoders (A and B channels)
#define LEFT_WHL_ENC_A 25   // Channel A of left encoder
#define LEFT_WHL_ENC_B 26   // Channel B of left encoder
#define RIGHT_WHL_ENC_A 27  // Channel A of right encoder
#define RIGHT_WHL_ENC_B 28  // Channel B of right encoder

// Define GPIO pins for the encoders (A and B channels)
#define LEFT_WHL_ENC_INT  LEFT_WHL_ENC_A    // Channel A of left encoder
#define LEFT_WHL_ENC_DIR LEFT_WHL_ENC_B    // Channel B of left encoder
#define RIGHT_WHL_ENC_INT RIGHT_WHL_ENC_A   // Channel A of right encoder
#define RIGHT_WHL_ENC_DIR RIGHT_WHL_ENC_B   // Channel B of right encoder

// Function declarations
void handler(int signo);
void left_wheel_pulse();  // Callback for left encoder
void right_wheel_pulse(); // Callback for right encoder
void set_motor_speeds(double left_wheel_command, double right_wheel_command);
void read_encoder_values(int *left_encoder_value, int *right_encoder_value);

// Variables to track encoder pulses and direction
extern int left_wheel_pulse_count;
extern int right_wheel_pulse_count;
extern int left_wheel_direction;  // 1: Forward, 0: Backward
extern int right_wheel_direction; // 1: Forward, 0: Backward

#endif 

#ifdef __cplusplus
}
#endif
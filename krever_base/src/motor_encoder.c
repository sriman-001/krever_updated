#include "krever_base/motor_encoder.h"
#include "krever_base/MotorDriver.h"
#include <math.h>

// Initialize pulse counters
int left_wheel_pulse_count = 0;
int right_wheel_pulse_count = 0;

// Initialize wheel directions
// 1 - forward, 0 - backward
int left_wheel_direction = 1;
int right_wheel_direction = 1;

// Read wheel encoder values
void read_encoder_values(int *left_encoder_value, int *right_encoder_value) {
    *left_encoder_value = left_wheel_pulse_count;
    *right_encoder_value = right_wheel_pulse_count;
}

// Left wheel callback function
void left_wheel_pulse() {
    // Determine direction based on A and B channels
    if (digitalRead(LEFT_WHL_ENC_A) == HIGH) {
        left_wheel_direction = (digitalRead(LEFT_WHL_ENC_B) == LOW) ? 1 : 0;
    } else {
        left_wheel_direction = (digitalRead(LEFT_WHL_ENC_B) == HIGH) ? 1 : 0;
    }

    // Increment or decrement pulse count based on direction
    if (left_wheel_direction == 1) {
        left_wheel_pulse_count++;
    } else {
        left_wheel_pulse_count--;
    }
}

// Right wheel callback function
void right_wheel_pulse() {
    // Determine direction based on A and B channels
    if (digitalRead(RIGHT_WHL_ENC_A) == HIGH) {
        right_wheel_direction = (digitalRead(RIGHT_WHL_ENC_B) == LOW) ? 1 : 0;
    } else {
        right_wheel_direction = (digitalRead(RIGHT_WHL_ENC_B) == HIGH) ? 1 : 0;
    }

    // Increment or decrement pulse count based on direction
    if (right_wheel_direction == 1) {
        right_wheel_pulse_count++;
    } else {
        right_wheel_pulse_count--;
    }
}

// Set motor speeds based on velocity commands
void set_motor_speeds(double left_wheel_command, double right_wheel_command) {
    DIR left_motor_direction;
    DIR right_motor_direction;

    // Calculate motor speed (convert to absolute values and scale)
    double left_motor_speed = fabs(left_wheel_command) * 1.65;
    double right_motor_speed = fabs(right_wheel_command) * 1.65;

    // Determine motor direction
    left_motor_direction = (left_wheel_command > 0) ? FORWARD : BACKWARD;
    right_motor_direction = (right_wheel_command > 0) ? FORWARD : BACKWARD;

    // Run motors with calculated speeds and directions
    Motor_Run(MOTORA, left_motor_direction, (int)left_motor_speed);
    Motor_Run(MOTORB, right_motor_direction, (int)right_motor_speed);
}

// Signal handler for cleanup or safe shutdown
void handler(int signo) {
    Motor_Stop(MOTORA);
    Motor_Stop(MOTORB);
    exit(0);
}
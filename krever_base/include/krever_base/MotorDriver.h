/*****************************************************************************
| File        :   MotorDriver.h
| Author      :   Custom
| Function    :   Drive L298N Motor Drivers
| Info        :
               L298N is a dual H-bridge motor driver used to control DC motors.
               Uses two input signals (IN1, IN2) and an enable pin (ENA, ENB) for speed and direction control.
*----------------
| This version:   V2.0
| Date        :   2024-11-15
| Info        :   Updated for L298N Motor Drivers
*
******************************************************************************/
#ifndef MOTOR_DRIVER_H__
#define MOTOR_DRIVER_H__

#include <wiringPi.h>
#include <softPwm.h>
#include <stdint.h>
#include <stdbool.h>

#include "krever_base/MotorDriver.h"

bool move_motor(int motor_id) {
    Motor_Run(motor_id, FORWARD, 50);
    delay(1000);  // Run motor for 1 second
    Motor_Stop(motor_id);
    return true;
}

int main() {
    Motor_Init();
    move_motor(0);  // Move Motor A
    move_motor(1);  // Move Motor B
    return 0;
}

// GPIO configuration for Motor A (Left Motor)
#define AIN1 17  // GPIO pin for Motor A IN1
#define AIN2 27  // GPIO pin for Motor A IN2
#define ENA  22  // GPIO pin for Motor A Enable (PWM)

// GPIO configuration for Motor B (Right Motor)
#define BIN1 23  // GPIO pin for Motor B IN1
#define BIN2 24  // GPIO pin for Motor B IN2
#define ENB  25  // GPIO pin for Motor B Enable (PWM)

// Motor IDs
#define MOTORA 0
#define MOTORB 1

// Direction Enum
typedef enum {
    BACKWARD = 0,
    FORWARD,
} DIR;

// Function Prototypes
void Motor_Init(void);
void Motor_Run(unsigned char motor, DIR dir, uint16_t speed);
void Motor_Stop(unsigned char motor);
unsigned char Motor_Direction(unsigned char motor);

#endif
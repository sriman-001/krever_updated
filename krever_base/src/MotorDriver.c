/*****************************************************************************
| File        :   MotorDriver.c
| Author      :   Custom
| Function    :   Drive L298N Motor Drivers
| Info        :
               Controls two DC motors using L298N motor drivers.
               Uses two input signals (IN1, IN2) and an enable pin (ENA, ENB) for speed and direction control.
*----------------
| This version:   V2.0
| Date        :   2024-11-15
| Info        :   Updated for L298N Motor Drivers
*
******************************************************************************/
#include "krever_base/MotorDriver.h"
#include <wiringPi.h>
#include <softPwm.h> // Used for software-based PWM if hardware PWM is not available

// Initialize motor state variables
unsigned char ain1_value = 0, ain2_value = 0;
unsigned char bin1_value = 0, bin2_value = 0;

/**
 * Motor Initialization
 *
 * Configures the GPIO pins and initializes software PWM for speed control.
 */
void Motor_Init(void) {
    // Initialize WiringPi library
    wiringPiSetupGpio();

    // Configure GPIO pins for Motor A
    pinMode(AIN1, OUTPUT);
    pinMode(AIN2, OUTPUT);
    pinMode(ENA, OUTPUT);
    softPwmCreate(ENA, 0, 100); // Create software PWM on ENA with range 0-100

    // Configure GPIO pins for Motor B
    pinMode(BIN1, OUTPUT);
    pinMode(BIN2, OUTPUT);
    pinMode(ENB, OUTPUT);
    softPwmCreate(ENB, 0, 100); // Create software PWM on ENB with range 0-100
}

/**
 * Run the Motor
 *
 * @param motor: Motor A (MOTORA) or Motor B (MOTORB)
 * @param dir: Direction (FORWARD or BACKWARD)
 * @param speed: Speed (0-100)
 */
void Motor_Run(unsigned char motor, DIR dir, unsigned short speed) {
    if (speed > 100) {
        speed = 100; // Cap speed to 100
    }

    if (motor == MOTORA) {
        // Set speed for Motor A
        softPwmWrite(ENA, speed);

        // Set direction for Motor A
        if (dir == FORWARD) {
            digitalWrite(AIN1, HIGH);
            digitalWrite(AIN2, LOW);
            ain1_value = 1;
            ain2_value = 0;
        } else {
            digitalWrite(AIN1, LOW);
            digitalWrite(AIN2, HIGH);
            ain1_value = 0;
            ain2_value = 1;
        }
    } else if (motor == MOTORB) {
        // Set speed for Motor B
        softPwmWrite(ENB, speed);

        // Set direction for Motor B
        if (dir == FORWARD) {
            digitalWrite(BIN1, HIGH);
            digitalWrite(BIN2, LOW);
            bin1_value = 1;
            bin2_value = 0;
        } else {
            digitalWrite(BIN1, LOW);
            digitalWrite(BIN2, HIGH);
            bin1_value = 0;
            bin2_value = 1;
        }
    }
}

/**
 * Stop the Motor
 *
 * @param motor: Motor A (MOTORA) or Motor B (MOTORB)
 */
void Motor_Stop(unsigned char motor) {
    if (motor == MOTORA) {
        softPwmWrite(ENA, 0); // Set speed to 0 to stop Motor A
    } else if (motor == MOTORB) {
        softPwmWrite(ENB, 0); // Set speed to 0 to stop Motor B
    }
}

/**
 * Get Motor Direction
 *
 * @param motor: Motor A (MOTORA) or Motor B (MOTORB)
 * @return 1 for FORWARD, 0 for BACKWARD
 */
unsigned char Motor_Direction(unsigned char motor) {
    if (motor == MOTORA) {
        if (ain1_value == 1 && ain2_value == 0) {
            return 1; // FORWARD
        } else if (ain1_value == 0 && ain2_value == 1) {
            return 0; // BACKWARD
        }
    } else if (motor == MOTORB) {
        if (bin1_value == 1 && bin2_value == 0) {
            return 1; // FORWARD
        } else if (bin1_value == 0 && bin2_value == 1) {
            return 0; // BACKWARD
        }
    }
    return 0; // Default to BACKWARD if undefined
}
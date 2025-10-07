#ifndef INC_MOTOR28BYJ_H_
#define INC_MOTOR28BYJ_H_

#include <stdint.h>
#include "main.h"

typedef struct {
    GPIO_TypeDef* IN_PORT[4];
    uint16_t IN_PIN[4];
    volatile uint32_t stepIdx;
    volatile uint32_t remainingHalfSteps;
} StepperMotor;

extern StepperMotor motor1;
extern StepperMotor motor2;

void startMotorTimer(void);
void stopMotorTimer(void);
void stopMotor(StepperMotor* motor);
void setMotorSteps(StepperMotor* motor, uint32_t steps);
void checkMotorHalfStep(void);

#endif /* INC_MOTOR28BYJ_H_ */

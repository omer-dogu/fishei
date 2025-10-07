#include "motor28byj.h"

extern volatile uint8_t tim2Elapsed;

StepperMotor motor1 = {
    .IN_PORT = {GPIOA, GPIOA, GPIOA, GPIOA},
    .IN_PIN  = {GPIO_PIN_1, GPIO_PIN_2, GPIO_PIN_3, GPIO_PIN_4},
    .stepIdx = 0,
    .remainingHalfSteps = 0,
};

StepperMotor motor2 = {
    .IN_PORT = {GPIOB, GPIOB, GPIOB, GPIOB},
    .IN_PIN  = {GPIO_PIN_7, GPIO_PIN_6, GPIO_PIN_5, GPIO_PIN_4},
    .stepIdx = 0,
    .remainingHalfSteps = 0,
};

int motorHalfStepTable[8][4] = {
    {1,0,0,0},
    {1,1,0,0},
    {0,1,0,0},
    {0,1,1,0},
    {0,0,1,0},
    {0,0,1,1},
    {0,0,0,1},
    {1,0,0,1}
};

void startMotorTimer(void)
{
    tim2Elapsed = 0;
    __HAL_TIM_SET_COUNTER(&htim2, 0);
    HAL_TIM_Base_Start_IT(&htim2);
}

void stopMotorTimer(void)
{
    HAL_TIM_Base_Stop_IT(&htim2);
    __HAL_TIM_SET_COUNTER(&htim2, 0);
}

void stopMotor(StepperMotor* motor)
{
    HAL_GPIO_WritePin(motor->IN_PORT[0], motor->IN_PIN[0], 0);
    HAL_GPIO_WritePin(motor->IN_PORT[1], motor->IN_PIN[1], 0);
    HAL_GPIO_WritePin(motor->IN_PORT[2], motor->IN_PIN[2], 0);
    HAL_GPIO_WritePin(motor->IN_PORT[3], motor->IN_PIN[3], 0);
}

void setMotorSteps(StepperMotor* motor, uint32_t steps)
{
    motor->remainingHalfSteps = steps * 8;
    motor->stepIdx = 0;
}

static int updateMotorHalfStep(StepperMotor* motor)
{
	HAL_GPIO_WritePin(motor->IN_PORT[0], motor->IN_PIN[0], motorHalfStepTable[motor->stepIdx][0]);
	HAL_GPIO_WritePin(motor->IN_PORT[1], motor->IN_PIN[1], motorHalfStepTable[motor->stepIdx][1]);
	HAL_GPIO_WritePin(motor->IN_PORT[2], motor->IN_PIN[2], motorHalfStepTable[motor->stepIdx][2]);
	HAL_GPIO_WritePin(motor->IN_PORT[3], motor->IN_PIN[3], motorHalfStepTable[motor->stepIdx][3]);

	motor->stepIdx = (motor->stepIdx + 1) % 8;
	motor->remainingHalfSteps--;

	if(motor->remainingHalfSteps == 0) {
		stopMotor(motor);
		return -1;
	}
	return 0;
}

void checkMotorHalfStep(void)
{
    if(tim2Elapsed) {
        tim2Elapsed = 0;

        if (updateMotorHalfStep(&motor1) && updateMotorHalfStep(&motor2))
        	stopMotorTimer();
    }
}

#include <stdio.h>
#include "main.h"
#include "oled.h"
#include "ds3231.h"
#include "motor28byj.h"
#include "fifo_conf.h"

#define RELAY1_PIN GPIO_PIN_12
#define RELAY1_PORT GPIOB
#define RELAY2_PIN GPIO_PIN_13
#define RELAY2_PORT GPIOB

uint8_t ledStatus = 0;
int feedCount = 0;

volatile uint8_t tim2Elapsed = 0;
volatile uint8_t printFlag = 0;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM1) {
    	DS3231_ReadTime_IT();
        printFlag = 1;
    }
    if(htim->Instance == TIM2) {
        tim2Elapsed = 1;
    }
}

void consoleInit(void)
{
    OLED_Start(0);
    setvbuf(stdout, NULL, _IONBF, 0);
}

void _putch(unsigned char c) { OLED_putch(c); }

size_t _write(int handle, char *buffer, int size)
{
    size_t nChars = 0;
    if (!buffer || (handle != _STDOUT && handle != _STDERR)) return 0;

    while(size--) {
        _putch(*buffer++);
        ++nChars;
    }
    return nChars;
}

void handleFeedingTask(void)
{
	static uint8_t feedEventTriggered;

    if(rtcTime.hour == 17 && rtcTime.min == 0 && rtcTime.sec == 0) {
        if(!feedEventTriggered) {
            setMotorSteps(&motor1, 512);
            setMotorSteps(&motor2, 512);
            startMotorTimer();
            feedCount++;
            feedEventTriggered = 1;
        }
    } else {
        feedEventTriggered = 0;
    }
}

void handleLightTask(void)
{
	static uint8_t ledOnEventTriggered;
	static uint8_t ledOffEventTriggered;

    if(rtcTime.hour == 9 && rtcTime.min == 0 && rtcTime.sec == 0) {
        if(!ledOnEventTriggered) {
        	ledStatus = 1;
            ledOnEventTriggered = 1;
        	HAL_GPIO_WritePin(RELAY1_PORT, RELAY1_PIN, 0);
        	HAL_GPIO_WritePin(RELAY2_PORT, RELAY2_PIN, 0);
        }
    } else {
        ledOnEventTriggered = 0;
    }

    if(rtcTime.hour == 16 && rtcTime.min == 0 && rtcTime.sec == 0) {
        if(!ledOffEventTriggered) {
        	ledStatus = 0;
            ledOffEventTriggered = 1;
        	HAL_GPIO_WritePin(RELAY1_PORT, RELAY1_PIN, 1);
        	HAL_GPIO_WritePin(RELAY2_PORT, RELAY2_PIN, 1);
        }
    } else {
        ledOffEventTriggered = 0;
    }
}

void printStatus(void)
{
	if (printFlag) {
		printFlag = 0;
		OLED_SetCursor(0, 0);
		printf("%02d:%02d:%02d\n\r", rtcTime.hour, rtcTime.min, rtcTime.sec);
		printf("Light: %s\n\r", ledStatus ? "On" : "Off");
		printf("Feed: %d", feedCount);
	}
}

void myMain(void)
{
	HAL_TIM_Base_Start_IT(&htim1);

	fifoInit(&oledTransmitFifo, oledTransmitBuf, OLED_BUF_SIZE);

	consoleInit();
	stopMotor(&motor1);
	stopMotor(&motor2);

	while(1)
	{
		handleFeedingTask();
		handleLightTask();
		checkMotorHalfStep();
		printStatus();
	}
}

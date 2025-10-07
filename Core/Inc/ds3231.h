#ifndef INC_DS3231_H_
#define INC_DS3231_H_

#include <stdint.h>

typedef struct {
    uint8_t sec;
    uint8_t min;
    uint8_t hour;
    uint8_t wday;
    uint8_t mday;
    uint8_t month;
    uint16_t year;
} RtcTime;

extern RtcTime rtcTime;
extern uint8_t rtcUpdateFlag;

void DS3231_ReadTime_IT(void);
HAL_StatusTypeDef DS3231_SetTime(const RtcTime *t);

#endif /* INC_DS3231_H_ */

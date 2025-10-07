#include "main.h"
#include "ds3231.h"

#define DS3231_ADDR 0x68

RtcTime rtcTime;
uint8_t rtcUpdateFlag;
uint8_t dsBuf[7];

static uint8_t bcd2bin(uint8_t v){ return v - 6 * (v >> 4); }
static uint8_t bin2bcd(uint8_t v){ return v + 6 * (v / 10); }

void DS3231_ReadTime_IT(void)
{
    HAL_I2C_Mem_Read_IT(&hi2c1, DS3231_ADDR << 1, 0x00, I2C_MEMADD_SIZE_8BIT, dsBuf, 7);
}

void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    if(hi2c != &hi2c1) return;

    rtcTime.sec   = bcd2bin(dsBuf[0] & 0x7F);
    rtcTime.min   = bcd2bin(dsBuf[1] & 0x7F);
    rtcTime.hour  = bcd2bin(dsBuf[2] & 0x3F);
    rtcTime.wday  = bcd2bin(dsBuf[3] & 0x07);
    rtcTime.mday  = bcd2bin(dsBuf[4] & 0x3F);
    rtcTime.month = bcd2bin(dsBuf[5] & 0x1F);
    rtcTime.year  = 2000 + bcd2bin(dsBuf[6]);

    rtcUpdateFlag = 1;
}

HAL_StatusTypeDef DS3231_SetTime(const RtcTime *t)
{
    uint8_t data[7];
    data[0] = bin2bcd(t->sec);
    data[1] = bin2bcd(t->min);
    data[2] = bin2bcd(t->hour);
    data[3] = bin2bcd(t->wday);
    data[4] = bin2bcd(t->mday);
    data[5] = bin2bcd(t->month);
    data[6] = bin2bcd((uint8_t)(t->year - 2000));

    return HAL_I2C_Mem_Write_IT(&hi2c1, DS3231_ADDR << 1, 0x00, I2C_MEMADD_SIZE_8BIT, data, 7);
}

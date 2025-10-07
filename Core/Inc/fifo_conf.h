#ifndef FIFO_CONF_H
#define FIFO_CONF_H

#include "fifo.h"

#define OLED_BUF_SIZE 2048

extern uint8_t oledTransmitBuf[OLED_BUF_SIZE];

extern fifo_t oledTransmitFifo;

#endif

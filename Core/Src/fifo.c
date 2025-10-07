#include "fifo.h"
#include <stddef.h>

int fifoInit(fifo_t* fifo, uint8_t* buf, uint32_t len)
{
    fifo->initFlag = 1;
    fifo->buf = buf;
    fifo->len = len;
    fifo->count = 0;
    fifo->head = 0;
    fifo->tail = 0;
    fifo->lines = 0;

    return 0;
}

int fifoWrite(fifo_t* fifo, uint8_t data)
{
    if (fifo->initFlag != 1 || fifo->count == fifo->len)
        return -1;

    fifo->buf[fifo->head++] = data;

    if (fifo->head == fifo->len)
        fifo->head = 0;

    fifo->count++;

    if (data == '\n')
        ++fifo->lines;

    return 0;
}

int fifoWriteMulti(fifo_t* fifo, const uint8_t* data, uint32_t size)
{
    if ((fifo->initFlag != 1) || (size == 0) || (size > (fifo->len - fifo->count)) || (fifo->count == fifo->len))
        return -1;

    for (int i = 0; i < size; ++i) {
        if (*data == '\n')
            ++fifo->lines;

        fifo->buf[fifo->head++] = *data++;

        if (fifo->head == fifo->len)
            fifo->head = 0;

        fifo->count++;
    }

    return 0;
}

int fifoRead(fifo_t* fifo, uint8_t* data)
{
    if (fifo->initFlag != 1 || fifo->count == 0)
        return -1;

    *data = fifo->buf[fifo->tail++];

    if (fifo->tail == fifo->len)
        fifo->tail = 0;

    fifo->count--;

    if (*data == '\n')
        --fifo->lines;

    return 0;
}

int fifoReadMulti(fifo_t* fifo, uint8_t* data, uint32_t size)
{
    if ((fifo->initFlag != 1) || (size == 0) || (size > fifo->count) || (fifo->count == 0))
        return -1;

    for (int i = 0; i < size; ++i) {
        uint8_t ch = fifo->buf[fifo->tail++];

        if (fifo->tail == fifo->len)
            fifo->tail = 0;

        *data++ = ch;
        fifo->count--;

        if (ch == '\n')
            --fifo->lines;
    }

    return 0;
}

int fifoTakeMulti(fifo_t* fifo, uint8_t* data, uint32_t size)
{
    if (fifo->initFlag != 1 || size == 0 || size > fifo->count)
        return -1;

    for (uint32_t i = 0; i < size; ++i) {
        uint32_t idx = fifo->tail + i;
        if (idx >= fifo->len) idx -= fifo->len;
        data[i] = fifo->buf[idx];
    }
    return 0;
}

void fifoClear(fifo_t* fifo)
{
    fifo->count = 0;
    fifo->head = 0;
    fifo->tail = 0;
    fifo->lines = 0;
}

int fifoGetLine(fifo_t* fifo, uint8_t* lBuf)
{
    uint8_t data;

    if (fifo->lines == 0)
        return -1;

    do {
        if (fifoRead(fifo, &data) == -1)
            return -1;

        *lBuf++ = data;
    } while (data != '\n');

    *lBuf = '\0';

    return 0;
}

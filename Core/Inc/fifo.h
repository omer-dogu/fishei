#ifndef FIFO_H
#define FIFO_H

#include <stddef.h>
#include <stdint.h>

typedef struct {
    uint32_t head;
    uint32_t tail;
    uint32_t count;
    uint32_t len;
    uint32_t initFlag;
    uint32_t lines;
    uint8_t* buf;
} fifo_t;

int fifoInit(fifo_t* fifo, uint8_t* buf, uint32_t len);
int fifoWrite(fifo_t* fifo, uint8_t data);
int fifoWriteMulti(fifo_t* fifo, const uint8_t* data, uint32_t size);
int fifoRead(fifo_t* fifo, uint8_t* data);
int fifoReadMulti(fifo_t* fifo, uint8_t* data, uint32_t size);
void fifoClear(fifo_t* fifo);
int fifoGetLine(fifo_t* fifo, uint8_t* lBuf);

int fifoTakeMulti(fifo_t* fifo, uint8_t* data, uint32_t size);

static inline uint32_t fifoGetCount(const fifo_t* fifo)
{
    return fifo->count;
}

static inline int fifoIsEmpty(const fifo_t* fifo)
{
    return fifo->count == 0;
}

#endif

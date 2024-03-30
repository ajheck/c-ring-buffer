#include "fifo.h"

Fifo_t* Fifo_Init(FifoIndex_t elementCount, FifoSize_t elementSize, uint8_t *buffer, size_t bufferSize)
{
    if(NULL == buffer)
    {
        return NULL;
    }

    size_t requiredSize = FIFO_COMPUTE_SIZE(elementCount, elementSize);
    if( requiredSize > bufferSize)
    {
        return NULL;
    }

    Fifo_t *fifo = (Fifo_t *)buffer;
    fifo->elemSize = elementSize;
    fifo->count = elementCount;
    fifo->reader = 0;
    fifo->writer = 0;

    return fifo;
}
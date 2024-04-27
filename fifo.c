#include "fifo.h"
#include <string.h>


// Distance from a to b with a wrap
#define wrapDistance(a, b, wrap) (((a) <= (b)) ? ((b) - (a)) : ((wrap) - (a) + (b)))

// Compute a + 1 with a wrap
#define wrapIncrement(a, wrap) (((a) + 1 < (wrap)) ? ((a) + 1) : 0)

// Cast arbitrary type to fifo pointer
#define pFifo(fifo) ((Fifo_t *)(fifo))

// If set, the fifo is full
#define STATUS_BIT_FULL_POS (0)
#define STATUS_BIT_FULL_MASK (1 << STATUS_BIT_FULL_POS)

#define isFull(fifo) (pFifo(fifo)->status & STATUS_BIT_FULL_MASK)
#define setFull(fifo) (pFifo(fifo)->status |= STATUS_BIT_FULL_MASK)
#define clearFull(fifo) (pFifo(fifo)->status &= ~STATUS_BIT_FULL_MASK)

// Compute the address of the start of a fifo's entry data
#define getDataPtr(fifo) ((void *)(fifo) + sizeof(Fifo_t))

// Compute the address of a fifo's present writing pointer
#define getWriterPtr(fifo) ((void *)(getDataPtr(fifo) + pFifo(fifo)->writer * pFifo(fifo)->entrySize))

// Compute the address of a fifo's present reading pointer
#define getReaderPtr(fifo) ((void *)(getDataPtr(fifo) + pFifo(fifo)->reader * pFifo(fifo)->entrySize))

// Compute the number of entries in a fifo
#define getEntryCount(fifo) ( (isFull(fifo)) ? pFifo(fifo)->maxEntryCount : wrapDistance(pFifo(fifo)->reader, pFifo(fifo)->writer, pFifo(fifo)->maxEntryCount))

Fifo_t *Fifo_Init(FifoIndex_t entryCount, FifoSize_t entrySize, uint8_t *buffer, size_t bufferSize)
{
    if (0 == entryCount || 0 == entrySize || NULL == buffer || 0 == bufferSize)
    {
        return NULL;
    }

    size_t requiredSize = FIFO_COMPUTE_SIZE(entryCount, entrySize);
    if (requiredSize > bufferSize)
    {
        return NULL;
    }

    Fifo_t *fifo = (Fifo_t *)buffer;
    fifo->entrySize = entrySize;
    fifo->maxEntryCount = entryCount;
    fifo->reader = 0;
    fifo->writer = 0;
    fifo->status = 0;

    return fifo;
}

int Fifo_EntrySize(Fifo_t *fifo)
{
    return (NULL == fifo) ? FIFO_RET_INVALID_ARG : fifo->entrySize;
}

int Fifo_EntryCount(Fifo_t *fifo)
{
    return (NULL == fifo) ? FIFO_RET_INVALID_ARG : getEntryCount(fifo);
}

int Fifo_Enqueue(Fifo_t *fifo, void *entry)
{
    if (NULL == fifo || NULL == entry)
    {
        return FIFO_RET_INVALID_ARG;
    }

    if(isFull(fifo))
    {
        // For now, fail to enqueue on full fifo
        // TODO: Add support for overflow detection when dequeuing
        // TODO: Add support for configuring overflow behavior (overwrite vs enqueue failure)
        return FIFO_RET_OVERFLOW;
    }

    memcpy(getWriterPtr(fifo), entry, fifo->entrySize);
    if( getEntryCount(fifo) == fifo->maxEntryCount - 1 )
    {
        // This enqueue caused the buffer to be filled; mark it as full, THEN increment the writer
        setFull(fifo);
    }
    
    fifo->writer = wrapIncrement(fifo->writer, fifo->maxEntryCount);

    return getEntryCount(fifo);
}

int Fifo_Dequeue(Fifo_t *fifo, void *entry)
{
    if (NULL == fifo)
    {
        return FIFO_RET_INVALID_ARG;
    }

    if (getEntryCount(fifo) == 0)
    {
        return FIFO_RET_NO_DATA;
    }

    if(entry != NULL)
    {
        memcpy(entry, getReaderPtr(fifo), fifo->entrySize);
    }
    fifo->reader = wrapIncrement(fifo->reader, fifo->maxEntryCount);
    // Only clear the full status once there is a free space
    clearFull(fifo);

    return getEntryCount(fifo);
}

void *Fifo_Peek(Fifo_t *fifo)
{
    return (fifo != NULL && getEntryCount(fifo) != 0) ? getReaderPtr(fifo) : NULL;
}
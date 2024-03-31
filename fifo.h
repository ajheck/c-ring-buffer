#ifndef C_RING_BUFFER_FIFO_H
#define C_RING_BUFFER_FIFO_H

#include <stdint.h>

typedef uint8_t FifoSize_t;

typedef uint8_t FifoIndex_t;

typedef struct
{
    FifoIndex_t writer;
    FifoIndex_t reader;
    FifoIndex_t maxEntryCount;
    FifoSize_t entrySize;
    uint8_t status;
} Fifo_t;

typedef enum {
    FIFO_RET_OK = 0,
    FIFO_RET_INVALID_ARG = -1,
    FIFO_RET_OVERFLOW = -2,
    FIFO_RET_NO_DATA = -3,
} FifoRet_t;

/**
 * @brief Macro to compute the required memory footprint of a fifo.
 * Useful for creating statically sized buffers.
 *
 * @param entryCount number of entries in the fifo
 * @param entrySize Size of each element in B
 * @return Size, in B, required to store the data and the fifo overhead
 */
#define FIFO_COMPUTE_SIZE(entryCount, entrySize) ((FifoIndex_t)(entryCount) * (FifoSize_t)(entrySize) + sizeof(Fifo_t))

/**
 * @brief Initialize a block of memory as a fifo.
 * Use @ref FIFO_COMPUTE_SIZE to determine the optimal buffer size.
 *
 * @param entryCount Number of entries in the fifo
 * @param entrySize Size of each element in B
 * @param buffer Memory for the fifo to use
 * @param bufferSize Size of buffer in B
 * @return Fifo_t* Pointer to the initialized fifo on success; NULL if buffer is NULL, any argument is 0, or bufferSize is too small.
 */
Fifo_t *Fifo_Init(FifoIndex_t entryCount, FifoSize_t entrySize, uint8_t *buffer, size_t bufferSize);

/**
 * @brief Get the element size of a fifo.
 *
 * @param fifo Fifo to check the element size of
 * @return int negative value on error; otherwise, the size of each element in the fifo.
 */
int Fifo_EntrySize(Fifo_t *fifo);

/**
 * @brief Get a count of how many entries are presently enqueued in the fifo.
 *
 * @param fifo Fifo to check
 * @return int negative number on error; otherwise, the count of enqueued entries
 */
int Fifo_EntryCount(Fifo_t *fifo);

/**
 * @brief Copy data into the fifo.
 *
 * @param fifo Fifo to enqueue data on
 * @param entry Entry to be copied
 * @return int negative value on failure to enqueue; otherwise, the count of enqueued entries after the enqueue is completed.
 */
int Fifo_Enqueue(Fifo_t *fifo, void *entry);

/**
 * @brief Read data from the fifo
 *
 * @param fifo Fifo to dequeue data from
 * @param entry Location to copy data to
 * @return int negative value on failure to dequeue; otherwise, the count of enqueued entries after the dequeue is completed.
 */
int Fifo_Dequeue(Fifo_t *fifo, void *entry);

#endif // include guard
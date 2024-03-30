#ifndef C_RING_BUFFER_FIFO_H
#define C_RING_BUFFER_FIFO_H

#include <stdint.h>

typedef uint8_t FifoSize_t;

typedef uint8_t FifoIndex_t;

typedef struct Fifo_s {
    FifoSize_t elemSize;
    FifoIndex_t writer;
    FifoIndex_t reader;
    FifoIndex_t count;
} Fifo_t;


/**
 * @brief Macro to compute the required memory footprint of a fifo.
 * Useful for creating statically sized buffers.
 * 
 * @param elementCount number of elements in the fifo
 * @param elementSize Size of each element in B
 * @return Size, in B, required to store the data and the fifo overhead
 */
#define FIFO_COMPUTE_SIZE(elementCount, elementSize) ((FifoIndex_t)(elementCount) * (FifoSize_t)(elementSize) + sizeof(Fifo_t))

/**
 * @brief Initialize a block of memory as a fifo.
 * Use @ref FIFO_COMPUTE_SIZE to determine the optimal buffer size.
 * 
 * @param elementCount Number of elements in the fifo
 * @param elementSize Size of each element in B
 * @param buffer Memory for the fifo to use
 * @param bufferSize Size of buffer in B
 * @return Fifo_t* Pointer to the initialized fifo on success; NULL if buffer is NULL, or bufferSize is too small.
 */
Fifo_t* Fifo_Init(FifoIndex_t elementCount, FifoSize_t elementSize, uint8_t *buffer, size_t bufferSize);

#endif // include guard
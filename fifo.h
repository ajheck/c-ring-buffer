#ifndef C_RING_BUFFER_FIFO_H
#define C_RING_BUFFER_FIFO_H

#include <stdint.h>

/**
 * @brief Size of an entry or block of memory in bytes.
 * Typedef'd to allow configuraiton on a per applicaton basis.
 * Shrinking the width of the typedef reduces metadata overhead.
 */
typedef uint8_t FifoSize_t;

/**
 * @brief Index into a fIfo's entry data.
 * To ensure concurrency, this must be atomic read/write for the target architecture.
 * Typedef'd to allow configuraiton on a per applicaton basis.
 * Shrinking the width of the typedef reduces metadata overhead.
 */
typedef uint8_t FifoIndex_t;

/**
 * @brief Fifo metadata. Declared here in the .h to support sizeof(Fifo_t) outside the library.
 * Members of this structure should not be accessed outside of the provided Fifo_* APIs.
 */
typedef struct
{
    FifoIndex_t writer;
    FifoIndex_t reader;
    FifoIndex_t maxEntryCount;
    FifoSize_t entrySize;
    uint8_t status;
} Fifo_t;

/**
 * @brief Return types for applicable Fifo_* APIs
 */
typedef enum {
    FIFO_RET_OK = 0,
    FIFO_RET_INVALID_ARG = -1,
    FIFO_RET_OVERFLOW = -2,
    FIFO_RET_NO_DATA = -3,
} FifoRet_t;

/**
 * @brief Macro to compute the required memory footprint of a fifo.
 * Useful for creating statically sized buffers, if arguments are constants.
 *
 * @param entryCount number of entries in the fifo
 * @param entrySize Size of each element in B
 * @return Size, in B, required to store the data and the fifo overhead
 */
#define FIFO_COMPUTE_SIZE(entryCount, entrySize) ((FifoIndex_t)(entryCount) * (FifoSize_t)(entrySize) + sizeof(Fifo_t))

/**
 * @brief Macro to peek and dequeue each entry in a fifo
 */
#define FIFO_PEEK_FOREACH(fifoPtr, peekPtr) for(int fifo_foreach_ret = 0; fifo_foreach_ret >= 0 && (peekPtr = Fifo_Peek(fifo)) != NULL; fifo_foreach_ret = Fifo_Dequeue(fifo, NULL))

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
 * @param entry Location to copy data to; if NULL, no copy is performed, but the entry is still dequeued
 * @return int negative value on failure to dequeue; otherwise, the count of enqueued entries after the dequeue is completed.
 */
int Fifo_Dequeue(Fifo_t *fifo, void *entry);

/**
 * @brief Peek the next entry in the fifo.  Useful for zero-copy usage of data in the fifo.
 * 
 * @param fifo Fifo to peek
 * @return void* pointer to peeked entry; NULL if error or no data to peek.
 */
void *Fifo_Peek(Fifo_t *fifo);

#endif // include guard
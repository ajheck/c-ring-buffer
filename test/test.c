
#include "minunit.h"
#include "fifo.h"

MU_TEST(test_Fifo_Init)
{
    FifoIndex_t count = 1;
    FifoSize_t size = 1;
    uint8_t data[FIFO_COMPUTE_SIZE(count, size)];

    mu_assert(NULL == Fifo_Init(0, size, data, sizeof(data)), "Fifo_Init() did not return NULL for 0 element size");
    mu_assert(NULL == Fifo_Init(count, 0, data, sizeof(data)), "Fifo_Init() did not return NULL for 0 count");
    mu_assert(NULL == Fifo_Init(count, size, data, 0), "Fifo_Init() did not return NULL for 0 buffer size");
    mu_assert(NULL == Fifo_Init(count, size, NULL, sizeof(data)), "Fifo_Init() did not return NULL when passed a NULL buffer");
    mu_assert(NULL == Fifo_Init(count, size, data, sizeof(data) - 1), "Fifo_Init() did not return NULL when passed a size which was too small");
    mu_assert((Fifo_t *)data == Fifo_Init(count, size, data, sizeof(data)), "Fifo_Init() did not return expected pointer to data");
}

MU_TEST(test_Fifo_Enqueue)
{
    FifoIndex_t count = 1;
    FifoSize_t size = 1;
    uint8_t data[FIFO_COMPUTE_SIZE(count, size)];
    uint8_t entry = 'a';
    Fifo_t *fifo = Fifo_Init(count, size, data, sizeof(data));

    // Argument checks
    mu_assert_int_eq(FIFO_RET_INVALID_ARG, Fifo_Enqueue(NULL, &entry));
    mu_assert_int_eq(FIFO_RET_INVALID_ARG, Fifo_Enqueue(fifo, NULL));

    // Corner case; FIFO of size 1
    mu_assert_int_eq(1, Fifo_Enqueue(fifo, &entry));
    mu_assert_int_eq(FIFO_RET_OVERFLOW, Fifo_Enqueue(fifo, &entry));

    // Nominal enqueue until full.
    FifoIndex_t biggerCount = 5;
    uint8_t biggerData[FIFO_COMPUTE_SIZE(biggerCount, size)];
    fifo = Fifo_Init(count, size, data, sizeof(biggerData));
    for(int i = 0; i < count; i ++)
    {
        mu_assert_int_eq((i + 1), Fifo_Enqueue(fifo, &entry));
    }
    mu_assert( FIFO_RET_OVERFLOW == Fifo_Enqueue(fifo, &entry), "Fifo_Enqueue() did not return expected error when overflowing after multiple enqueues");
}

MU_TEST(test_Fifo_Dequeue)
{
    FifoIndex_t count = 1;
    FifoSize_t size = 1;
    uint8_t data[FIFO_COMPUTE_SIZE(count, size)];
    uint8_t entry = 'a';
    Fifo_t *fifo = Fifo_Init(count, size, data, sizeof(data));

    // Argument checks
    mu_assert_int_eq(FIFO_RET_INVALID_ARG, Fifo_Dequeue(NULL, &entry));

    // Empty dequeue
    mu_assert_int_eq(FIFO_RET_NO_DATA, Fifo_Dequeue(fifo, &entry));

    // Corner case; FIFO of size 1
    uint8_t enqueuedValue = 1;
    entry = ~enqueuedValue;
    mu_assert_int_eq(1, Fifo_Enqueue(fifo, &enqueuedValue));
    mu_assert_int_eq(0, Fifo_Dequeue(fifo, &entry));
    mu_assert_int_eq(enqueuedValue, entry);
    mu_assert_int_eq(FIFO_RET_NO_DATA, Fifo_Dequeue(fifo, &entry));

    // Nominal dequeue until empty.
    FifoIndex_t biggerCount = 4;
    uint8_t biggerData[FIFO_COMPUTE_SIZE(biggerCount, size)];
    fifo = Fifo_Init(biggerCount, size, data, sizeof(biggerData));
    for(int i = 0; i < biggerCount; i ++)
    {
        enqueuedValue = i;
        mu_assert_int_eq((i + 1), Fifo_Enqueue(fifo, &enqueuedValue));
    }

    int expectedValue = 0;
    int ret = 0;
    for(ret = Fifo_Dequeue(fifo, &entry); ret >= 0; ret = Fifo_Dequeue(fifo, &entry))
    {
        mu_assert_int_eq(expectedValue, entry);
        expectedValue ++;
    }
    mu_assert_int_eq(FIFO_RET_NO_DATA, ret);

    // Dequeue with NULL still works
    for(int i = 0; i < biggerCount; i ++)
    {
        enqueuedValue = i;
        mu_assert_int_eq((i + 1), Fifo_Enqueue(fifo, &enqueuedValue));
    }
    
    for(ret = Fifo_Dequeue(fifo, NULL); ret >= 0; ret = Fifo_Dequeue(fifo, NULL))
    {
        mu_assert(ret >= 0, "Failed to dequeue with NULL buffer");
    }
    mu_assert_int_eq(FIFO_RET_NO_DATA, ret);
}

MU_TEST(test_Fifo_EntrySize)
{
    FifoIndex_t count = 1;
    FifoSize_t size = 1;
    uint8_t data[FIFO_COMPUTE_SIZE(count, size)];
    Fifo_t *fifo = Fifo_Init(count, size, data, sizeof(data));

    // Argument checks
    mu_assert_int_eq(FIFO_RET_INVALID_ARG, Fifo_EntrySize(NULL));

    // Nominal case
    mu_assert_int_eq(count, Fifo_EntrySize(fifo));
}

MU_TEST(test_Fifo_EntryCount)
{
    FifoIndex_t count = 10;
    FifoSize_t size = 1;
    uint8_t data[FIFO_COMPUTE_SIZE(count, size)];
    Fifo_t *fifo = Fifo_Init(count, size, data, sizeof(data));

    // Argument checks
    mu_assert_int_eq(FIFO_RET_INVALID_ARG, Fifo_EntryCount(NULL));

    // Nominal case 
    uint8_t entry = 1;
    int ret = 0;
    int expected = 1;
    while((ret = Fifo_Enqueue(fifo, &entry)) > 0)
    {
        mu_assert_int_eq(expected, Fifo_EntryCount(fifo));
        expected++;
    }
}


MU_TEST_SUITE(suite_Fifo_APIs)
{
    MU_RUN_TEST(test_Fifo_Init);
    MU_RUN_TEST(test_Fifo_Enqueue);
    MU_RUN_TEST(test_Fifo_Dequeue);
    MU_RUN_TEST(test_Fifo_EntrySize);
    MU_RUN_TEST(test_Fifo_EntryCount);
}

int main()
{
    MU_RUN_SUITE(suite_Fifo_APIs);
    MU_REPORT();
    return MU_EXIT_CODE;
}

#include "minunit.h"
#include "fifo.h"

MU_TEST(test_Fifo_Init){
    FifoIndex_t count = 1;
    FifoSize_t size = 1;
    uint8_t data[FIFO_COMPUTE_SIZE(count, size)];

    mu_assert(NULL == Fifo_Init(count, size, NULL, sizeof(data)), "Fifo_Init() did not return NULL when passed a NULL buffer");
    mu_assert(NULL == Fifo_Init(count, size, data, sizeof(data) - 1), "Fifo_Init() did not return NULL when passed a size which was too small");
    mu_assert((Fifo_t *)data == Fifo_Init(count, size, data, sizeof(data)), "Fifo_Init() did not return expected pointer to data");
}


MU_TEST_SUITE(suite_Fifo_APIs) {
    MU_RUN_TEST(test_Fifo_Init);
}

int main() {
	MU_RUN_SUITE(suite_Fifo_APIs);
	MU_REPORT();
	return MU_EXIT_CODE;
}
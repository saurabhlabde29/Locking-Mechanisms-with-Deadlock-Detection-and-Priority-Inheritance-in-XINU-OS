#include <xinu.h>

uint64 sum;
lock_t l;
uint32 index = 0;
uint32 naive_count = 0;
uint32 sync_count = 0;

uint32 serial_summation(uint32 *array, uint32 n);
uint32 naive_parallel_summation(uint32 *array, uint32 n, uint32 num_threads);
uint32 sync_parallel_summation(uint32 *array, uint32 n, uint32 num_threads);

uint32 parallel_sum_naive(uint32 *array, uint32 n);
uint32 parallel_sum_sync(uint32 *array, uint32 n);

void main_basic(void)
{
	uint32 n = 800000;   
	uint32 *array = (uint32*)getmem(sizeof(uint32)*n);
	uint32 num_threads = 40;
	uint64 result_serial = 0;
	uint64 result_naive_parallel = 0;
	uint64 result_sync_parallel = 0;
	uint32 i;
	for(i = 0; i<n; i++)
	{
		array[i] = 1;
	}
	init_lock(&l);
	
	sum = 0;
	result_serial = serial_summation(array, n);
	printf("Serial Summation Result = %d\n", result_serial);

	sum = 0;
	index = 0;
	result_naive_parallel = naive_parallel_summation(array, n, num_threads);
	printf("Naive Parallel Summation = %d\n", result_naive_parallel);

	sum = 0;
	index = 0;
	result_sync_parallel = sync_parallel_summation(array, n, num_threads);
	printf("Sync Parallel Summation Result = %d\n", result_sync_parallel);
	
	freemem((char*)array,sizeof(uint32)*n);
}

uint32 parallel_sum_naive(uint32 *array, uint32 n)
{
	while(index++<n)
 	{
		sum += *array;
		array++;
        }
	naive_count++;
	return 0;
}

uint32 parallel_sum_sync(uint32 *array, uint32 n)
{
	while(index++<n)
 	{
	 	lock(&l);
		sum += *array;
		array++;
		unlock(&l);
        }
	sync_count++;
	return 0;
}

uint32 serial_summation(uint32 *array, uint32 n)
{	
	uint32 i;
	for(i = 0; i<n; i++)
	{
		sum += array[i];
	}
	return sum;
}

uint32 naive_parallel_summation(uint32 *array, uint32 n, uint32 num_threads)
{
	uint32 i;
	for(i = 0; i<num_threads; i++)
	{
		resume(create(parallel_sum_naive, 1024, 15, "without_using_locks", 2, array, n));
	}
	while(naive_count < num_threads)
		sleepms(1000);
	return sum;
}

uint32 sync_parallel_summation(uint32 *array, uint32 n, uint32 num_threads)
{
	uint32 i;
	for(i = 0; i<num_threads; i++)
	{
		resume(create(parallel_sum_sync, 1024, 15, "with_using_locks", 2, array, n));
	}
	while(sync_count < num_threads)
		sleepms(1000);
	return sum;
}

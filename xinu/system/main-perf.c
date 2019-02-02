#include <xinu.h>

lock_t b;
sl_lock_t s;
uint32 spin_count = 0;
uint32 bwf_count = 0;

uint32 test_spin(uint32 num_threads);
uint32 test_bwf(uint32 num_threads);

uint32 spin_task();
uint32 bwf_task();

void main_perf(void)
{
	
	uint64 start_time, end_time, exec_spin, exec_bwf;
	
	init_lock(&b);
	sl_init(&s);
	
	uint32 num_threads = 50;
	
	start_time = ctr1000;
	test_spin(num_threads);
	end_time = ctr1000;
	exec_spin = end_time - start_time;
	printf("Time using SPINLOCK = %d\n", exec_spin);

	
	start_time = ctr1000;
	test_bwf(num_threads);
	
	end_time = ctr1000;
	exec_bwf = end_time - start_time;
	printf("Time using BUSY WAIT FREE LOCKS = %d\n", exec_bwf);
}

uint32 spin_task()
{
	uint32 i = 0;
	sl_lock(&s);
	while(i < 1000000)
 	{
 		i++;
        }
        sl_unlock(&s);
	spin_count++;
	return 0;
}

uint32 bwf_task()
{	
	uint32 i = 0;
	lock(&b);
	while(i < 1000000)
 	{
 		i++;
        }
	unlock(&b);
	bwf_count++;
	return 0;
}

uint32 test_spin(uint32 num_threads)
{
	uint32 i;
	for(i = 0; i<num_threads; i++)
	{
		resume(create(spin_task, 1024, 15, "using_spin_lock",0));
	}
	while(spin_count < num_threads)
		sleepms(10);
	return 0;
}

uint32 test_bwf(uint32 num_threads)
{
	uint32 i;
	for(i = 0; i<num_threads; i++)
	{
		resume(create(bwf_task, 1024, 15, "using_bwf_lock",0));
	}
	while(bwf_count < num_threads)
		sleepms(10);
	return 0;
}

#include <xinu.h>

lock_t lb;
lock_t lb1;
lock_t lb2;
lock_t pfml1;
lock_t pfml2;

pi_lock_t lp;
pi_lock_t lp1;
pi_lock_t lp2;
pi_lock_t lt1;
pi_lock_t lt2;
pi_lock_t lpm1;
pi_lock_t lpm2;

uint32 startTime_pi;
uint32 startTime_bwf;
uint32 endTime_pi;
uint32 endTime_bwf;
uint32 execTime_pi;
uint32 execTime_bwf;

void single_bwf_lock_action(lock_t *);
void single_pi_lock_action(pi_lock_t *);
void unbounded_function(void);
void double_bwf_lock_action(lock_t *, lock_t *);
void double_pi_lock_action(pi_lock_t *, pi_lock_t *);


// This is a model test case used in many locks. A process accquiring a single lock
void single_bwf_lock_action(lock_t *lo)
{
  uint32 i;
 intmask mask;
  lock(lo);
  mask = disable();
  kprintf("LOCK ENTRY :: Process %d -- Priority %d\n",getpid(),proctab[getpid()].prprio);
  restore(mask);
  i=0;
  while(i<10000000){
  	i++;
  }
 
  mask = disable();
 kprintf("LOCK EXIT :: Process %d -- Priority %d\n",getpid(),proctab[getpid()].prprio);
 restore(mask);
  unlock(lo);

}
// Performance measurement function to be executed by a single thread
void pfm_single_bwf_lock_action(lock_t *lo)
{
  uint32 i;
 intmask mask;
  lock(lo);
  mask = disable();
  kprintf("LOCK ENTRY :: Process %d -- Priority %d\n",getpid(),proctab[getpid()].prprio);
  restore(mask);
  i=0;
  while(i<10000000){
  	i++;
  }
 
  mask = disable();
 kprintf("LOCK EXIT :: Process %d -- Priority %d\n",getpid(),proctab[getpid()].prprio);
 restore(mask);
  unlock(lo);

}
// Performance measurement function to be run by the highest priority thread of no busy wait locks
void pfm_single_bwf_lock_action1(lock_t *lo)
{
startTime_bwf = ctr1000;
  uint32 i;
 intmask mask;
  lock(lo);
  mask = disable();
  kprintf("LOCK ENTRY :: Process %d -- Priority %d\n",getpid(),proctab[getpid()].prprio);
  restore(mask);
  i=0;
  while(i<10000000){
  	i++;
  }
 
  mask = disable();
 kprintf("LOCK EXIT :: Process %d -- Priority %d\n",getpid(),proctab[getpid()].prprio);
 restore(mask);
  unlock(lo);
 execTime_bwf = ctr1000 - startTime_bwf;
 kprintf("BWF LOCK - EXEC TIME OF HIGHEST PRIORITY PROCESS %d\n",execTime_bwf);
}



// This function models a single lock acquititon function by a process
void single_pi_lock_action(pi_lock_t *lo)
{
  uint32 i;
 intmask mask;
  pi_lock(lo);
  mask = disable();
 kprintf("LOCK ENTRY :: Process %d -- Priority %d\n",getpid(),proctab[getpid()].prprio);
 restore(mask);
  i=0;
  while(i<10000000){
  	i++;
  }
  mask = disable();
  kprintf("LOCK EXIT :: Process %d -- Priority %d\n",getpid(),proctab[getpid()].prprio);
  restore(mask);
  pi_unlock(lo);

}
// Performance measurement function of the single pi lock acquition process
void pfm_single_pi_lock_action(pi_lock_t *lo)
{
  uint32 i;
 intmask mask;
  pi_lock(lo);
  mask = disable();
 kprintf("LOCK ENTRY :: Process %d -- Priority %d\n",getpid(),proctab[getpid()].prprio);
 restore(mask);
  i=0;
  while(i<10000000){
  	i++;
  }
  mask = disable();
  kprintf("LOCK EXIT :: Process %d -- Priority %d\n",getpid(),proctab[getpid()].prprio);
  restore(mask);
  pi_unlock(lo);

}

// This performance measurement function is run by the highest priority thread of the pi lock case
void pfm_single_pi_lock_action1(pi_lock_t *lo)
{
 startTime_pi = ctr1000;
  uint32 i;
 intmask mask;
  pi_lock(lo);
  mask = disable();
 kprintf("LOCK ENTRY :: Process %d -- Priority %d\n",getpid(),proctab[getpid()].prprio);
 restore(mask);
  i=0;
  while(i<10000000){
  	i++;
  }
  mask = disable();
  kprintf("LOCK EXIT :: Process %d -- Priority %d\n",getpid(),proctab[getpid()].prprio);
  restore(mask);
  pi_unlock(lo);
  execTime_pi = ctr1000 -startTime_pi;
  kprintf("PI_LOCK EXEC TIME OF THE HIGHEST PRIORITY PROCESS %d\n",execTime_pi);

}



void unbounded_function()
{
  uint32 i;
  intmask mask;
  mask = disable();
  kprintf("LOCK ENTRY :: Process %d -- Priority %d\n",getpid(),proctab[getpid()].prprio);
  restore(mask);
  i=0;
  while(i<10000000){
  	i++;
  }
  mask = disable();
  kprintf("LOCK EXIT :: Process %d -- Priority %d\n",getpid(),proctab[getpid()].prprio);
  restore(mask);
}

// This function models the double bwf lock acquition function
void double_bwf_lock_action(lock_t *lock1, lock_t *lock2)
{
  uint32 i;
  intmask mask;
  lock(lock1);
  lock(lock2);
  mask = disable();
  kprintf("LOCK ENTRY :: Process %d -- Priority %d\n",getpid(),proctab[getpid()].prprio);
  restore(mask);
   i=0;
  while(i<1000000){
  	i++;
  }
  mask = disable();
  kprintf("LOCK EXIT :: Process %d -- Priority %d\n",getpid(),proctab[getpid()].prprio);
  restore(mask);
  unlock(lock2);
  unlock(lock1);

}

// This test models the double pi lock acquition function
void double_pi_lock_action(pi_lock_t *lock1, pi_lock_t *lock2)
{
  uint32 i;
  intmask mask;
  pi_lock(lock1);
  for(i = 0; i<100;i++);
  pi_lock(lock2);
  mask = disable();
  kprintf("LOCK ENTRY :: Process %d -- Priority %d\n",getpid(),proctab[getpid()].prprio);
 restore(mask);
   i=0;
  while(i<100000000){
  	i++;
  }
    mask = disable();
 kprintf("LOCK EXIT :: Process %d -- Priority %d\n",getpid(),proctab[getpid()].prprio);
 restore(mask);
  pi_unlock(lock2);
  pi_unlock(lock1);

}

/*
void pfm_double_pi_lock_action(pi_lock_t *lock1, pi_lock_t *lock2)
{
  startTime_pi = ctr1000;
  uint32 i;
  intmask mask;
  pi_lock(lock1);
  for(i = 0; i<100000;i++);
  pi_lock(lock2);
  mask = disable();
  kprintf("LOCK ENTRY :: Process %d -- Priority %d\n",getpid(),proctab[getpid()].prprio);
 restore(mask);
   i=0;
  while(i<100000000){
  	i++;
  }
    mask = disable();
 kprintf("LOCK EXIT :: Process %d -- Priority %d\n",getpid(),proctab[getpid()].prprio);
 restore(mask);
  pi_unlock(lock1);
  for(i=0;i<100000;i++);
  pi_unlock(lock2);
  
  execTime_pi = ctr1000 - startTime_pi;
  
  kprintf("EXEC Time of highest Priority process with pi lock : %d\n",execTime_pi);
}


void pfm_double_bwf_lock_action1(lock_t *lock1, lock_t *lock2)
{
  startTime_bwf = ctr1000;
  uint32 i;
  intmask mask;
  lock(lock1);
  for(i = 0; i<100000;i++);
  lock(lock2);
  mask = disable();
  kprintf("LOCK ENTRY :: Process %d -- Priority %d\n",getpid(),proctab[getpid()].prprio);
 restore(mask);
   i=0;
  while(i<100000000){
  	i++;
  }
    mask = disable();
 kprintf("LOCK EXIT :: Process %d -- Priority %d\n",getpid(),proctab[getpid()].prprio);
 restore(mask);
  unlock(lock1);
  for(i=0;i<100000;i++);
  unlock(lock2);
  
  execTime_bwf = ctr1000 - startTime_bwf;
  
  kprintf("EXEC Time of highest Priority process with bwf lock : %d\n",execTime_bwf);
}

*/


void main_priority_inversion()
{
  init_lock(&lb);
  init_lock(&lb1);
  init_lock(&lb2);
  init_lock(&pfml1);
 
  pi_init(&lp);
  pi_init(&lp1);
  pi_init(&lp2);
  pi_init(&lt1);
  pi_init(&lt2);
  pi_init(&lpm1);


  
  kprintf("----------------------------------------------------------------------------------------------------------------------------------------------------------\n");
  kprintf("TEST CASE 1 WITH PRIORITY INVERSION\n");
  kprintf("----------------------------------------------------------------------------------------------------------------------------------------------------------\n");
  resume(create(single_bwf_lock_action, 4096, 4, "TEST1_PROC1", 1, &lb));
  sleepms(2);
  resume(create(single_bwf_lock_action, 4096, 16, "TEST1_PROC2", 1, &lb));
  sleepms(1);
  resume(create(unbounded_function, 4096, 12, "TEST1_PROC3", 0));

  sleepms(1000);
  printf("\n\n\n");
  kprintf("----------------------------------------------------------------------------------------------------------------------------------------------------------\n");
  kprintf("TEST CASE 2 WITH PRIORITY INHERITANCE\n");
  kprintf("----------------------------------------------------------------------------------------------------------------------------------------------------------\n");

  resume(create(single_pi_lock_action, 4096, 4, "TEST2_PROC1", 1, &lp));
  sleepms(13);
  resume(create(single_pi_lock_action, 4096, 16, "TEST2_PROC2", 1, &lp));
 // sleepms(3);
  //resume(create(unbounded_function, 4096, 10, "Process 3_pi", 0));

  sleepms(1000);
  printf("\n\n\n");
 
   kprintf("----------------------------------------------------------------------------------------------------------------------------------------------------------\n");
  kprintf("TEST CASE 3 WITH PRIORITY INHERITANCE DEPICTING TRANSITIVE PROPERTY\n");
  kprintf("----------------------------------------------------------------------------------------------------------------------------------------------------------\n");

  
  
  resume(create(single_pi_lock_action,4096,2, "TEST3_PROC1",1, &lt1));
  sleepms(1);
  resume(create(double_pi_lock_action,4096,8, "TEST3_PROC2",2, &lt2,&lt1));
  sleepms(4);
  resume(create(single_pi_lock_action,4096,16, "TEST3_PROC3",1, &lt2));
 // resume(create(unbounded_function, 4096, 10, "Transitive4", 0));
  sleepms(1000);
  printf("\n\n\n");
  
  kprintf("----------------------------------------------------------------------------------------------------------------------------------------------------------\n");
  kprintf("TEST CASE 4 WITH PRIORITY INVERSION WITH CHAIN FORMING\n");
  kprintf("----------------------------------------------------------------------------------------------------------------------------------------------------------\n");

 
  resume(create(single_bwf_lock_action, 4096, 4, "TEST4_PROC1", 1, &lb1));
 // sleepms(1);
  resume(create(single_bwf_lock_action, 4096, 4, "TEST4_PROC2", 1, &lb2));
  sleepms(10);
  resume(create(double_bwf_lock_action, 4096, 16, "TEST4_PROC3", 2, &lb1, &lb2));

  sleepms(1000);
  printf("\n\n\n");
kprintf("----------------------------------------------------------------------------------------------------------------------------------------------------------\n");
  kprintf("TEST CASE 5 WITH PRIORITY INHERITANCE WITH CHAIN FORMING\n");
  kprintf("----------------------------------------------------------------------------------------------------------------------------------------------------------\n");
  
  
  resume(create(single_pi_lock_action, 4096, 5, "TEST5_PROC1", 1, &lp1));
 sleepms(3);
  resume(create(single_pi_lock_action, 4096, 6, "TEST5_PROC2", 1, &lp2));
  sleepms(1);
  resume(create(double_pi_lock_action, 4096, 10, "TEST5_PROC3", 2, &lp1, &lp2));

sleepms(1000);
  printf("\n\n\n");
kprintf("----------------------------------------------------------------------------------------------------------------------------------------------------------\n");
  kprintf("TEST CASE 6 WITH PRIORITY INHERITANCE PERFORMANCE MEASUREMENT\n");
  kprintf("----------------------------------------------------------------------------------------------------------------------------------------------------------\n");
// Test case 6 with priority inheritance with performance measurement

//resume(create(pfm_double_pi_lock_action, 4096, 4, "Lowest_prio", 2, &lpm1, &lpm2));
//sleepms(5);
//resume(create(pfm_single_pi_lock_action, 4096, 10, "Highest_prio", 1, &lpm1));
//sleepms(1);
//resume(create(pfm_single_pi_lock_action, 4096, 6, "Medium Prio", 1, &lpm2));
//resume(create(unbounded_function, 4096, 8, "Transitive4", 0));
resume(create(pfm_single_pi_lock_action, 4096, 4, "TEST6_PROC1", 1, &lpm1));
  sleepms(13);
  resume(create(pfm_single_pi_lock_action1, 4096, 16, "TEST6_PROC2", 1, &lpm1));
 // sleepms(3);
  resume(create(unbounded_function, 4096, 10, "TEST6_PROC3", 0));

sleepms(1000);

printf("\n\n\n");

//resume(create(pfm_double_bwf_lock_action, 4096, 4, "Lowest_prio", 2, &pfml1, &pfml2));
//sleepms(5);
//resume(create(pfm_single_bwf_lock_action, 4096, 10, "Highest_prio", 1, &pfml1));
//sleepms(1);
//resume(create(pfm_single_bwf_lock_action, 4096, 6, "Medium Prio", 1, &pfml2));
//resume(create(unbounded_function, 4096, 8, "Transitive4", 0));
resume(create(pfm_single_bwf_lock_action, 4096, 4, "TEST6_PROC4", 1, &pfml1));
  sleepms(2);
  resume(create(pfm_single_bwf_lock_action1, 4096, 16, "TEST6_PROC5", 1, &pfml1));
  //sleepms(1);
  resume(create(unbounded_function, 4096, 12, "TEST6_PROC6", 0));
sleepms(1000);

printf("\n\n\n");




}



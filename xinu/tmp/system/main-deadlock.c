#include<xinu.h>

al_lock_t l1;
al_lock_t l2;
al_lock_t l3;
al_lock_t l4;
al_lock_t l5;
al_lock_t l6;
al_lock_t l7;
al_lock_t l8;
al_lock_t l9;
al_lock_t l10;

void test_deadlock_1(void)
{
	al_lock(&l1);
	
	sleepms(1);
	
	al_lock(&l2);
	
	al_unlock(&l2);
	
	al_unlock(&l1);
}

void test_deadlock_2(void)
{
	al_lock(&l2);
	
	sleepms(1);
	
	al_lock(&l3);
	
	al_unlock(&l3);
	
	al_unlock(&l2);
}

void test_deadlock_3(void)
{
	al_lock(&l3);
	
	sleepms(1);
	
	al_lock(&l4);
	
	al_unlock(&l4);
	
	al_unlock(&l3);
}

void test_deadlock_4(void)
{
	al_lock(&l4);
	
	sleepms(1);
	
	al_lock(&l1);
	
	al_unlock(&l1);
	
	al_unlock(&l4);
}

void test_deadlock_5(void)
{
	al_lock(&l9);
	
	sleepms(1);
	
	al_lock(&l10);
	
	al_unlock(&l10);
	
	al_unlock(&l9);
}

void test_deadlock_6(void)
{
	al_lock(&l10);
	
	sleepms(1);
	
	al_lock(&l9);
	
	al_unlock(&l9);
	
	al_unlock(&l10);
}

void test_trylock_1(void)
{
	w:
	while(al_trylock(&l5) == FALSE);
	sleepms(1);
	if(al_trylock(&l6) == FALSE)
	{
		al_unlock(&l5);
		goto w;
	}
	
	al_unlock(&l6);
	
	al_unlock(&l5);
}

void test_trylock_2(void)
{
	x:
	while(al_trylock(&l6) == FALSE);
	sleepms(1);
	if(al_trylock(&l7) == FALSE)
	{
		al_unlock(&l6);
		goto x;
	}
	
	al_unlock(&l7);
	
	al_unlock(&l6);
}

void test_trylock_3(void)
{
	y:
	while(al_trylock(&l7) == FALSE);
	sleepms(1);
	if(al_trylock(&l8) == FALSE)
	{
		al_unlock(&l7);
		goto y;
	}
	
	al_unlock(&l8);
	
	al_unlock(&l7);
}

void test_trylock_4(void)
{
	z:
	while(al_trylock(&l8) == FALSE);
	sleepms(1);
	if(al_trylock(&l5) == FALSE)
	{
		al_unlock(&l8);
		goto z;
	}
	
	al_unlock(&l5);
	
	al_unlock(&l8);
}

void main_deadlock(void)
{
	kprintf("Deadlock func executed \n");
	al_init(&l1);
	al_init(&l2);
	al_init(&l3);
	al_init(&l4);
	al_init(&l9);
	al_init(&l10);
	
	resume(create(test_deadlock_1, 4096, 15, "test_deadlock_1", 0));
	resume(create(test_deadlock_2, 4096, 15, "test_deadlock_2", 0));
	resume(create(test_deadlock_3, 4096, 15, "test_deadlock_3", 0));
	resume(create(test_deadlock_4, 4096, 15, "test_deadlock_4", 0));
	resume(create(test_deadlock_5, 4096, 15, "test_deadlock_5", 0));
	resume(create(test_deadlock_6, 4096, 15, "test_deadlock_6", 0));
	sleepms(1000);	
}

void main_trylock(void)
{
	kprintf("Trylock func executed \n");
	al_init(&l5);
	al_init(&l6);
	al_init(&l7);
	al_init(&l8);
	
	resume(create(test_trylock_1, 4096, 15, "test_trylock_1", 0));
	resume(create(test_trylock_2, 4096, 15, "test_trylock_2", 0));
	resume(create(test_trylock_3, 4096, 15, "test_trylock_3", 0));
	resume(create(test_trylock_4, 4096, 15, "test_trylock_4", 0));
	sleepms(1000);	
}

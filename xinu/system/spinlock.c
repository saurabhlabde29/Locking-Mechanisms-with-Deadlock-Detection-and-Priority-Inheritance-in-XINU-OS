/* spinlock.c - This file contains the implementation of the spinlock */
#include <xinu.h>

void sl_init(sl_lock_t *l){
	l->flag = NOT_TAKEN;
}

void sl_lock(sl_lock_t *l){
	while(test_and_set(&l->flag,TAKEN)==TAKEN);
}

void sl_unlock(sl_lock_t *l){
	l->flag = NOT_TAKEN;
}

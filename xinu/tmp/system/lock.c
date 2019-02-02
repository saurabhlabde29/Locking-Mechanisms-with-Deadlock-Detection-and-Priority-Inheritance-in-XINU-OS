/*lock.c - This file contains the implementation for the lock without busy wait */

#include <xinu.h>

void park(void);
void set_park(void);
void unpark(pid32);

void init_lock(lock_t *l){
	l->flag=0;
	l->guard =0;
	l->wait_queue = newqueue();
}

void park(){
	intmask mask;
	mask = disable();
	struct procent *prptr = &proctab[currpid];
	if(prptr->set_park_flag == SET){
		prptr->set_park_flag = NOT_SET;
		restore(mask);
		suspend(currpid);
	}
	else
		restore(mask);
}

void set_park() {
	intmask mask;
	mask = disable();
	struct procent *prptr = &proctab[currpid];
	prptr->set_park_flag = SET;
	restore(mask);
}

void unpark(pid32 pid){
	ready(pid);
}

void lock(lock_t *l){
	while(test_and_set(&l->guard , TAKEN)==TAKEN);
	if(l->flag == NOT_SET){
		l->flag = SET;
		l->guard = NOT_TAKEN;
	
	}
	else {
		l->guard = NOT_TAKEN;
		enqueue(currpid, l->wait_queue);
		set_park();
		
		park();
	}

}

void unlock(lock_t *l){
	while(test_and_set(&l->guard , TAKEN)==TAKEN);
	l->guard = NOT_TAKEN;
	if(isempty(l->wait_queue))
		l->flag = NOT_SET;
		
	else {
		l->guard = NOT_TAKEN;
		unpark(dequeue(l->wait_queue));
		
	}

}

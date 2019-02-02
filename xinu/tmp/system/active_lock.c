/*active_lock.c - This file contains the implementation for the lock without busy wait  with deadlock detection*/

#include <xinu.h>
int c =0;
void al_park(void);
void al_set_park(void);
void al_unpark(pid32, al_lock_t *);
uint32 deadlock_detector(pid32);
void display_deadlock(void);
void clear_deadlock_flags(void);

void al_init(al_lock_t *l){
	l->lock_id = ++number_of_locks;
	l->flag=0;
	l->guard =0;
	l->wait_queue = newqueue();
	l->lock_holder = NONE;
}

void al_park(){
	struct procent *prptr = &proctab[currpid];
	if(prptr->set_park_flag == SET){
		prptr->set_park_flag = NOT_SET;
		suspend(currpid);
	}
}

void al_set_park() {
	struct procent *prptr = &proctab[currpid];
	prptr->set_park_flag = SET;
}

void al_unpark(pid32 pid, al_lock_t *l){
	l->lock_holder = pid;
	proctab[pid].has_lock = l->lock_id;
	locktab[l->lock_id] = pid;
	proctab[pid].waiting_for_lock = NONE;
	ready(pid);
}

void al_lock(al_lock_t *l){
	while(test_and_set(&l->guard , TAKEN)==TAKEN);
	//kprintf("Accquired the guard\n");
	if(l->flag == NOT_SET){
		l->flag = SET;
		//kprintf("PID %d has set the flag\n",currpid);
		l->guard = NOT_TAKEN;
		l->lock_holder = currpid;
		//kprintf("LOCKID: %d - Holder %d\n",l->lock_id,currpid);
		proctab[currpid].has_lock = l->lock_id;
		locktab[l->lock_id] = currpid;
	
	}
	else {
		proctab[currpid].waiting_for_lock = l->lock_id;
		//if(currpid == 9)
			//kprintf("PID : %d - wf: %d : H %d\n",currpid,l->lock_id,l->lock_holder);
		if(deadlock_detector(l->lock_holder)){
			//kprintf("Lock detected\n");
			display_deadlock();
			clear_deadlock_flags();
		}
		else
			clear_deadlock_flags();
		
		number_of_deadlocked_processes = 0;
		enqueue(currpid, l->wait_queue);
		al_set_park();
		l->guard = NOT_TAKEN;
		al_park();
	}

}

bool8 al_trylock(al_lock_t *l){
	while(test_and_set(&l->guard , TAKEN)==TAKEN);
	if(l->flag == NOT_SET){
		l->flag = SET;
		l->guard = NOT_TAKEN;
		l->lock_holder = currpid;
		proctab[currpid].has_lock = l->lock_id;
		locktab[l->lock_id] = currpid;
		return TRUE;
	}
	else
		l->guard = NOT_TAKEN;
		return FALSE;

}

void al_unlock(al_lock_t *l){
	while(test_and_set(&l->guard , TAKEN)==TAKEN);
	l->guard = NOT_TAKEN;
	l->lock_holder = NONE;
	proctab[currpid].has_lock = NONE;
	locktab[l->lock_id] = 0;
	//proctab[currpid].waiting_for_lock = 0;
	if(isempty(l->wait_queue))
		l->flag = NOT_SET;
		
	else 
		al_unpark(dequeue(l->wait_queue ),l);
		
	

}

uint32 deadlock_detector(pid32 lock_holder){
	//kprintf("Inside deadlock detector\n");
	struct procent *prptr = &proctab[lock_holder];
	//kprintf("Currpid : %d  Lockholder %d\n",currpid,lock_holder);
	lid32 waiting_for_lock_id = prptr->waiting_for_lock;
	pid32 next_lock_holder = locktab[waiting_for_lock_id];
	//if(c++ == 0)
		//kprintf("currpid %d - next lock holder %d\n",currpid, next_lock_holder);
	if(waiting_for_lock_id == 0) {
		//if(currpid == 9)
			//kprintf("currpid %d lockholder %d\n\n",currpid,lock_holder);
		return 0;
	}
	else if(next_lock_holder == currpid){
		proctab[lock_holder].part_of_deadlock = 1;
		proctab[next_lock_holder].part_of_deadlock = 1;
		number_of_deadlocked_processes += 2;
		return 1;
	}
	else{
		number_of_deadlocked_processes++;
		proctab[lock_holder].part_of_deadlock = 1;
		return deadlock_detector(next_lock_holder);
	}
}

void display_deadlock(void){
	intmask mask;
	mask = disable();
	kprintf("lock detected = ");
	int i ;
	int count_of_processes_printed = 0;
	for(i=0;i<NPROC;i++){
		if(proctab[i].part_of_deadlock == 1){
			count_of_processes_printed++;
			if(count_of_processes_printed != number_of_deadlocked_processes)
				kprintf("P%d-",i);
			else
				kprintf("P%d",i);
		}
			
	}
	kprintf("\n");
	restore(mask);

}

void clear_deadlock_flags(void){
	int i;
	for(i = 0; i<=number_of_locks;i++){
		proctab[locktab[i]].part_of_deadlock = 0;
	}

}

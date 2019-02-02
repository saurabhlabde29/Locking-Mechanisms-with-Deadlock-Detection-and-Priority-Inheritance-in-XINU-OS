/* This file contains the implementation of the lock with priority inheritance protocol*/

#include<xinu.h>

void pi_park(void);
void pi_set_park(void);
void pi_unpark(pid32, pi_lock_t *);
void priority_inheritance_protocol(pid32 , pid32);
void priority_release(void);

void pi_init(pi_lock_t *l){
	l->lock_id = ++number_of_locks_pi;
	//kprintf("Lock Id %d\n",l->lock_id);
	l->flag=NOT_SET;
	l->guard =NOT_TAKEN;
	l->wait_queue = newqueue();
	l->lock_holder = NONE;
}

void pi_park(){
	intmask mask;
	mask = disable();
	struct procent *prptr = &proctab[currpid];
	if(prptr->set_park_flag == SET){
		prptr->set_park_flag = NOT_SET;
		restore(mask);
		suspend(currpid);
	}
}

void pi_set_park() {
	intmask mask;
	mask = disable();
	struct procent *prptr = &proctab[currpid];
	prptr->set_park_flag = SET;
	restore(mask);
}
// FIXME Change this according to pilock
void pi_unpark(pid32 pid, pi_lock_t *l){
	intmask mask;
	mask = disable();
	//kprintf("%d\n",pid);
	//kprintf("FID %d\n",firstid(readylist));
	priority_release();
	l->lock_holder = pid;
	proctab[pid].has_lock = l->lock_id;
	locktab_pi[l->lock_id].pid = pid;
	proctab[pid].waiting_for_lock = NONE;
	proctab[pid].wait_queue_id = 0;
	restore(mask);
	resume(pid);
}


void pi_lock(pi_lock_t *l){
	while(test_and_set(&l->guard , TAKEN)==TAKEN);
	intmask mask;
	//mask = disable();
	//kprintf("Currpid : %d : Lock Id %d\n",currpid, l->lock_id);
	//restore(mask);
	if(l->flag == NOT_SET){
		//kprintf("Currpid : %d has lockid %d \n ",currpid,l->lock_id);
		l->flag = SET;
		l->guard = NOT_TAKEN;
		l->lock_holder = currpid;
		proctab[currpid].has_lock = l->lock_id;
		locktab_pi[l->lock_id].pid = currpid;
		locktab_pi[l->lock_id].wq_id = l->wait_queue;
	
	}
	else {
		//kprintf("Currpid : %d waiting for lockid %d\n ",currpid,l->lock_id);
		proctab[currpid].waiting_for_lock = l->lock_id;
		proctab[currpid].wait_queue_id = l->wait_queue;
		//mask = disable();
		priority_inheritance_protocol(l->lock_holder, currpid);
		//restore(mask);
		insert(currpid, l->wait_queue ,proctab[currpid].prprio);
		pi_set_park();
		l->guard = NOT_TAKEN;
		pi_park();
	}

}

void pi_unlock(pi_lock_t *l){
	while(test_and_set(&l->guard , TAKEN)==TAKEN);
	l->guard = NOT_TAKEN;
	l->lock_holder = NONE;
	proctab[currpid].has_lock = NONE;
	locktab_pi[l->lock_id].pid = 0;
	//priority_release();
	if(isempty(l->wait_queue))
		l->flag = NOT_SET;	
	else 
		pi_unpark(dequeue(l->wait_queue ),l);
		
}

void priority_inheritance_protocol(pid32 lock_holder, pid32 current_process){
	pri16 old_prprio = proctab[lock_holder].prprio;
	lid32 blocking_lock;
	pid32  blocking_lock_holder;
	intmask mask;
	mask = disable();
	if(proctab[current_process].prprio > proctab[lock_holder].prprio){
		proctab[lock_holder].prprio = proctab[current_process].prprio;
		//mask = disable();
		kprintf("Priority change = P%d::%d-%d\n",lock_holder, old_prprio, proctab[lock_holder].prprio);
		//restore(mask);
		if(proctab[lock_holder].prstate == PR_READY){
			//kprintf("Inserted in ready list\n");
			lock_holder = getitem(lock_holder);
			insert(lock_holder, readylist, proctab[lock_holder].prprio);
		}
		else if(proctab[lock_holder].prstate == PR_SUSP){
			lock_holder = getitem(lock_holder);
			insert(lock_holder,proctab[lock_holder].wait_queue_id,proctab[lock_holder].prprio);
		
		}
	}

	blocking_lock = proctab[lock_holder].waiting_for_lock;
	while(blocking_lock > 0){
		blocking_lock_holder = locktab_pi[blocking_lock].pid;
		old_prprio = proctab[blocking_lock_holder].prprio;
		if(proctab[current_process].prprio > proctab[blocking_lock_holder].prprio){
			proctab[blocking_lock_holder].prprio = proctab[current_process].prprio;
			//mask = disable();
			kprintf("Priority change = P%d::%d-%d\n",blocking_lock_holder, old_prprio, proctab[blocking_lock_holder].prprio);
			//restore(mask);
			if(proctab[blocking_lock_holder].prstate == PR_READY){
				getitem(blocking_lock_holder);
				insert(blocking_lock_holder , readylist , proctab[blocking_lock_holder].prprio);
			}
			else if (proctab[blocking_lock_holder].prstate == PR_SUSP){
				getitem(proctab[blocking_lock_holder].wait_queue_id);
				insert(blocking_lock_holder, proctab[blocking_lock_holder].wait_queue_id, proctab[blocking_lock_holder].prprio);
			}
		
			blocking_lock = proctab[blocking_lock_holder].waiting_for_lock;
		}
		else {
			blocking_lock = 0;
			restore(mask);
			return ;
		}
	}
	restore(mask);

}

void priority_release(void){

	pri16  highest_prprio;
	int i;
	int index = 0;
	lid32 mylocks[MAX_LOCKS];
	pri16 myprio[MAX_LOCKS];
	pri16 old_prio = proctab[currpid].prprio;
	if(proctab[currpid].prprio == proctab[currpid].original_prprio)
		return;
	else {
		//kprintf("Inside else\n");
		for(i=1;i<=number_of_locks_pi;i++){
			if(locktab_pi[i].pid == currpid){
				mylocks[index] = i;
				myprio[index] = firstkey(locktab_pi[i].wq_id);
				index++;
			}
		}
		highest_prprio = myprio[0];
		for(i=1;i<index;i++){
			if(highest_prprio < myprio[i])
				highest_prprio = myprio[i];
		}
		
		 if (index == 0){
		 	//old_prio = proctab[currpid].prprio;
			proctab[currpid].prprio = proctab[currpid].original_prprio;
			//kprintf("Index = 0\n");
			kprintf("Priority change = P%d::%d-%d\n",currpid,old_prio,proctab[currpid].prprio);
			return;
		}
		if(proctab[currpid].original_prprio >= highest_prprio){
			//kprintf("Enter loop 2\n");
			proctab[currpid].prprio = proctab[currpid].original_prprio;
			kprintf("Priority change = P%d::%d-%d\n",currpid,old_prio,proctab[currpid].prprio);
			return;
		}
		
		//kprintf("highest Prio %d\n",highest_prprio);
		proctab[currpid].prprio = highest_prprio;
		kprintf("Priority change = P%d::%d-%d\n",currpid,old_prio,proctab[currpid].prprio);
	}



}


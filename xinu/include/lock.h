/*lock.h - This is the header file for all the lock variants */

#define TAKEN 	1
#define NOT_TAKEN	0

#define SET	1
#define NOT_SET	0

#define MAX_LOCKS	100
#define NONE	0

typedef struct {

	uint32 flag;
	
} sl_lock_t;

typedef struct {
	uint32 flag;
	qid16 wait_queue;
	uint32 guard;

}lock_t;

typedef struct {
	lid32 lock_id;
	uint32 flag;
	qid16 wait_queue;
	uint32 guard;
	pid32 lock_holder;

}al_lock_t;

typedef struct {
	lid32  lock_id;
	uint32 flag;
	qid16 wait_queue;
	uint32 guard;
	pid32  lock_holder;
}pi_lock_t;

typedef struct {
	
	pid32 pid;
	qid16 wq_id;
}tab_t;

lid32 number_of_locks;
lid32 number_of_locks_pi;

uint32 number_of_deadlocked_processes;

pid32  locktab[MAX_LOCKS];

tab_t locktab_pi[MAX_LOCKS];


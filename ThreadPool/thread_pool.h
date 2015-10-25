#ifndef __RV_THREAD_POOL__
#define __RV_THREAD_POOL__

#include <pthread.h>

#define THREAD_POOL_MAX_THREADNUM 256

typedef void (*TASK_FUNC)(void * arg, int id);

struct task {
	int id;
	TASK_FUNC func;
	void * arg;
	struct task * next;
};

struct thread_pool {
	unsigned int thread_num, task_num;
	struct task * task_list, * task_list_tail;
	pthread_t threads[THREAD_POOL_MAX_THREADNUM];
	pthread_mutex_t queue_lock; 
    pthread_cond_t queue_ready;
    int running;
};

int  thread_pool_init(struct thread_pool * pool, unsigned int thread_num);
void thread_pool_destory(struct thread_pool * pool);
int  thread_pool_add_task(struct thread_pool * pool, TASK_FUNC func, void * arg, int id);
void thread_sleep(int ms);

#endif

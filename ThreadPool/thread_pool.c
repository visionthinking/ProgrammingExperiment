#include "thread_pool.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef WIN32
	#include <windows.h>
	void thread_sleep(int ms) {
	    Sleep(ms);
	}
#else
	#include <unistd.h>
	void thread_sleep(int ms) {
	    usleep(ms*1000);
	}
#endif

static
void * thread_pool_process(void * args){
	struct thread_pool * pool = (struct thread_pool * ) args;
	struct task * current_task = NULL;
	// printf("Thread 0x%x started.\n", pthread_self());
	while(1){
		pthread_mutex_lock(&pool->queue_lock);
		while(pool->task_num == 0 && pool->running){
			// printf("Thread 0x%x is waiting.\n", pthread_self());
			pthread_cond_wait(&pool->queue_ready, &pool->queue_lock);
		}
		if(!pool->running){
			pthread_mutex_unlock(&pool->queue_lock);
			// printf("Thread 0x%x is exiting.\n", pthread_self());
			pthread_exit(NULL);
		}
		current_task = pool->task_list->next;
		pool->task_list->next = current_task->next;
		pool->task_num -= 1;
		if(pool->task_num == 0){
			pool->task_list_tail = pool->task_list;
		}
		pthread_mutex_unlock(&pool->queue_lock);
		
		if(current_task && current_task->func){
			// printf("Thread 0x%x is working. [ task id: %d ]\n", pthread_self(), current_task->id);
			current_task->func(current_task->arg, current_task->id);
			// printf("Thread 0x%x finished.   [ task id: %d ]\n", pthread_self(), current_task->id);
			free(current_task);
			current_task = NULL;
		}
	}
}

static
void thread_pool_start(struct thread_pool * pool){
	int i;
	pthread_mutex_init(&pool->queue_lock, NULL);  
    pthread_cond_init(&pool->queue_ready, NULL);
    for(i = 0; i < pool->thread_num; i += 1){
    	if(pthread_create(&pool->threads[i], NULL, thread_pool_process, (void *)pool)){
			fprintf(stderr, "Creating thread error.\n");
			exit(-1);
		}
    }
}

int thread_pool_init(struct thread_pool * pool, unsigned int thread_num){
	pool->thread_num = thread_num < THREAD_POOL_MAX_THREADNUM ? thread_num : THREAD_POOL_MAX_THREADNUM;
	pool->task_list = (struct task*) malloc(sizeof(struct task));
	if(!pool->task_list){
		fprintf(stderr, "Creating thread pool error.\n");
		return -1;
	} 
	pool->task_list_tail = pool->task_list;
	memset(pool->task_list, 0, sizeof(struct task));
	pool->running = 1;
	pool->task_num = 0;
	thread_pool_start(pool);
	return 0;
}

void thread_pool_destory(struct thread_pool * pool){
	struct task * p = NULL;
	unsigned int i;
	if(!pool->running){
		return;
	}
	pool->running = 0;
	
	pthread_cond_broadcast(&pool->queue_ready);
	for (i = 0; i < pool->thread_num; i++){
    	pthread_join(pool->threads[i], NULL);
    }
	
	pthread_mutex_lock(&pool->queue_lock);
	if(pool->task_num > 0){
		//free task linked-list
		while(pool->task_list->next){
			p = pool->task_list->next;
			free(pool->task_list);
			pool->task_list = p;
		}
		pool->task_num = 0;
		pool->task_list = NULL;
		pool->task_list_tail = NULL;
	}
	pthread_mutex_unlock(&pool->queue_lock);
	
	pthread_mutex_destroy(&pool->queue_lock);
    pthread_cond_destroy(&pool->queue_ready);
}

int thread_pool_add_task(struct thread_pool * pool, TASK_FUNC func, void * arg, int id){
	struct task * new_task = (struct task*) malloc(sizeof(struct task));
	if(!new_task){
		return -1;
	}
	new_task->func = func;
	new_task->arg = arg;
	new_task->next = NULL;
	new_task->id = id;
	
	pthread_mutex_lock(&pool->queue_lock);
	pool->task_list_tail->next = new_task;
	pool->task_list_tail = new_task;
	pool->task_num += 1;
	pthread_mutex_unlock(&pool->queue_lock);
	pthread_cond_signal(&pool->queue_ready);
	return 0;
}



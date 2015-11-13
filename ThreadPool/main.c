#include <stdio.h>
#include <stdlib.h>

#include "thread_pool.h"
#include "message_queue.h"

void task(void * arg, int id){
	int i = 10000000;
	while(i--){
	}
	printf("task %d: finish.\n", id);
}

void thread_pool_example(){
	int i;
	struct thread_pool pool;
	thread_pool_init(&pool, 8);
	thread_sleep(100);
	for(i=0;i<20;i++){
		thread_pool_add_task(&pool, task, NULL, i);
	}
	thread_sleep(1000);
	thread_pool_destory(&pool);
}

void message_queue_example(){
	int i, x;
	struct message_queue q;
	int data[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	
	// initialize.
	message_queue_init(&q);
	
	// add messages.
	for(i=0;i<10;i++){
		message_queue_push(&q, &data[i]);
	}
	
	// get all of the messages.
	while(q.msg_num){
		x = *((int*)message_queue_pop(&q));
		printf("msg: %d\n", x);	
	}
	
	// destroy
	message_queue_destroy(&q);
}

int main(void){
	thread_pool_example();
	message_queue_example();
	return 0;	
}

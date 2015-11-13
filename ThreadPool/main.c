#include <stdio.h>
#include <stdlib.h>

#include "thread_pool.h"
#include "message_queue.h"

void task(void * arg, int id){
	int i = 100000000;
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
	thread_sleep(2000);
	thread_pool_destory(&pool);
}

void message_queue_example(){
	int i;
	struct message_queue q;
	struct message_pack msg;
	
	// initialize.
	message_queue_init(&q);
	
	// add 20 messages.
	for(i=0;i<20;i++){
		message_queue_push(&q, NULL, 0, i);
	}
	
	// get all of the messages.
	while(q.msg_num){
		message_queue_pop(&q, &msg);
		printf("msg: %d\n", msg.id);
	}
	
	// destroy
	message_queue_destroy(&q);
}

int main(void){
	thread_pool_example();
	message_queue_example();
	return 0;	
}

#include <stdio.h>
#include <stdlib.h>

#include "thread_pool.h"

void task(void * arg, int id){
	int i = 100000000;
	while(i--){
	}
	printf("%d: finish.\n", id);
}

int main(void){
	int i;
	struct thread_pool pool;
	thread_pool_init(&pool, 8);
	thread_sleep(100);
	for(i=0;i<100;i++){
		thread_pool_add_task(&pool, &task, NULL, i);
	}
	thread_sleep(5000);
	thread_pool_destory(&pool);
	return 0;	
}

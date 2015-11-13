#include "message_queue.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int message_queue_init(struct message_queue * q){
	q->msg_list = (struct message_pack *) malloc(sizeof(struct message_pack));
	if(!q->msg_list){
		fprintf(stderr, "Creating message queue error.\n");
		return -1;
	}
	q->msg_list_tail = q->msg_list;
	memset(q->msg_list, 0, sizeof(struct message_pack));
	q->msg_num = 0;
	q->is_running = 1;
	pthread_mutex_init(&q->queue_lock, NULL);
    pthread_cond_init(&q->queue_ready, NULL);
	return 0;
}

void message_queue_destroy(struct message_queue * q){
	struct message_pack * p = NULL;
	if(!q->is_running){
		return;
	}
	q->is_running = 0;
	pthread_cond_broadcast(&q->queue_ready);
	
	pthread_mutex_lock(&q->queue_lock);
	if(q->msg_num > 0){
		//free msg linked-list
		while(q->msg_list->next){
			p = q->msg_list->next;
			free(q->msg_list);
			q->msg_list = p;
		}
		q->msg_num = 0;
		q->msg_list = NULL;
		q->msg_list_tail = NULL;
	}
	pthread_mutex_unlock(&q->queue_lock);
	
	pthread_mutex_destroy(&q->queue_lock);
    pthread_cond_destroy(&q->queue_ready);
}

int message_queue_push(struct message_queue * q, void * data, unsigned int data_len, int id){
	struct message_pack * msg = (struct message_pack *) malloc(sizeof(struct message_pack));
	if(!msg){
		fprintf(stderr, "message_queue_push: Memory Error");
		return -1;	
	}
	msg->id = id;
	msg->data = data;
	msg->data_len = data_len;
	msg->next = NULL;
	
	pthread_mutex_lock(&q->queue_lock);
	q->msg_list_tail->next = msg;
	q->msg_list_tail = msg;
	q->msg_num += 1;
	pthread_mutex_unlock(&q->queue_lock);
	return 0;
}

void message_queue_pop(struct message_queue * q, struct message_pack * msg){
	struct message_pack * first = NULL;
	pthread_mutex_lock(&q->queue_lock);
	while(q->msg_num == 0 && q->is_running){
		pthread_cond_wait(&q->queue_ready, &q->queue_lock);
	}
	if(!q->is_running){
		pthread_mutex_unlock(&q->queue_lock);
		return;
	}
	first = q->msg_list->next;
	q->msg_list->next = first->next;
	q->msg_num -= 1;
	if(q->msg_num == 0){
		q->msg_list_tail = q->msg_list;
	}
	pthread_mutex_unlock(&q->queue_lock);
	
	first->next = NULL;
	memcpy(msg, first, sizeof(struct message_pack));
	free(first);
}







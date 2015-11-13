#ifndef __RV_MESSAGE_QUEUE__
#define __RV_MESSAGE_QUEUE__

#include <pthread.h>

struct message_pack {
	void * data;
	struct message_pack * next;
};

struct message_queue {
	struct message_pack * msg_list, * msg_list_tail;
	unsigned int msg_num;
	pthread_mutex_t queue_lock;
    pthread_cond_t queue_ready;
    int is_running;
};

int  message_queue_init(struct message_queue * q);
void message_queue_destroy(struct message_queue * q);
int  message_queue_push(struct message_queue * q, void * data);
void * message_queue_pop(struct message_queue * q);

#endif

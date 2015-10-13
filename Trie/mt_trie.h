#include "trie.h"
#include <pthread.h>

#ifndef __MT_TRIE_H__
#define __MT_TRIE_H__

#define NUM_THREADS	8

struct mt_trie {
	unsigned int threads_num;
	pthread_t threads[NUM_THREADS];
	struct trie trie_list[NUM_THREADS];
	char * common_words;
	ull words_size;
};

void mt_trie_init(char * build_file, unsigned int threads_num, uint max_nodes);
void mt_trie_build();
int mt_trie_query(char * str, uint len);

#endif

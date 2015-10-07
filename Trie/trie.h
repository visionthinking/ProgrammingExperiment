#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef __TRIE_H__
#define __TRIE_H__

typedef unsigned int uint;
typedef unsigned long long ull;

struct node {
	ull child[2];
	ull bit_start;
	ull bit_len;
	ull is_word_end;
};

struct trie {
	char INDEX_FILE_NAME[1024];
	size_t INDEX_FILE_SIZE_MB;
	uint MAX_NODES;
	
	char * words;
	struct node * t;
	ull tlen;
};

#define BITS_USED 8
/* 1 for success, 0 for failed */
int trie_init(struct trie * mytrie, char * index_file_name, size_t file_size_MB, uint max_nodes);
int trie_query(struct trie * mytrie, char * str);

#endif



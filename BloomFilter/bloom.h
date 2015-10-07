#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef __BLOOM_FILTER__
#define __BLOOM_FILTER__

typedef unsigned int (*HASH_FUNC)(char*, unsigned int);
typedef unsigned int u32;
typedef unsigned int uint;

struct bloom_filter {
	HASH_FUNC hash_func_list[30];
	int hash_func_list_len;
	u32 * data;
	uint bit_len;
};

int  bloom_filter_init(struct bloom_filter * bf, uint bit_len);
void bloom_filter_insert(struct bloom_filter * bf, char * str, uint len);
int  bloom_filter_query(struct bloom_filter * bf, char * str, uint len);

#endif

#include "mt_trie.h"
#include <pthread.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef WIN32
#include <windows.h>
void sleep_ms(int ms) {
    Sleep(ms);
}
#else
#include <unistd.h>
void sleep_ms(int ms) {
    usleep(ms*1000);
}
#endif

static struct mt_trie global_mt;
static volatile int running_threads = 0;
static pthread_mutex_t running_mutex = PTHREAD_MUTEX_INITIALIZER;

inline
uint hash_code(char c){
	return c;	
}

static 
long mt_trie_readfile(char ** words, char * filename){
	long file_size;
	size_t result;
	FILE * in = fopen(filename, "r");
	if(!in){
		fprintf(stdout, "file does not exist. [%s]\n", filename); 
		exit(-1);
	}
	fseek (in , 0 , SEEK_END);
	file_size = ftell (in);
	rewind (in);
	*words = (char*) malloc (sizeof(char)*(file_size + 1));
	(*words)[file_size] = 0;
	result = fread(*words, 1, file_size, in);
	if(result != file_size) {
		fprintf(stdout, "Reading file error.\n");
		exit(-1);
	}
	fclose(in);
	return file_size + 1;
}

void * mt_trie_run(void * threadid){
	unsigned int tid = (unsigned int) threadid;
	unsigned int threads_num = global_mt.threads_num;
	char * words = global_mt.common_words;
	ull wsize = global_mt.words_size;
	struct trie * mytrie = &global_mt.trie_list[tid];
	ull p, pstart, cnt = 0, hash;
	
	// trie insert
	p = 0;
	pstart = 0;
	while(p < wsize){
		if(words[p] == '\n' || words[p] == '\r' || words[p] == 0){
			if(p - pstart > 0 && hash_code(words[pstart]) % threads_num == tid){
				cnt += 1;
				trie_insert(mytrie, pstart, p - 1);
			}
			pstart = p + 1;
		}
		p += 1;
	}
	printf("insert count: %d\n", cnt);
	printf("Thread #%u finished.\n", tid);
	pthread_mutex_lock(&running_mutex);
	running_threads--;
	pthread_mutex_unlock(&running_mutex);
	pthread_exit(NULL);
}

void mt_trie_init(char * build_file, unsigned int threads_num, uint max_nodes){
	unsigned int i;
	// read build_file to common_words
	global_mt.words_size = mt_trie_readfile(&global_mt.common_words, build_file);
	global_mt.threads_num = threads_num;
	printf("Index file read size: %d Bytes\n", global_mt.words_size - 1);
	
	// build tries
	for(i=0;i<threads_num;i++){
		if(!trie_init(&global_mt.trie_list[i], build_file, 0, max_nodes / threads_num)){
			printf("Can not build trie.\n");
			exit(-1);
		}else{
			global_mt.trie_list[i].words = global_mt.common_words;	
		}
	}
}

void mt_trie_build(){
	unsigned int i;
	// start threads
	running_threads = global_mt.threads_num;
	for(i=0;i<global_mt.threads_num;i++){
		if(pthread_create(&global_mt.threads[i], NULL, mt_trie_run, (void *)i)){
			printf("Create thread error.\n");
			exit(-1);
		}
	}
	
	while(running_threads > 0){
		sleep_ms(50);
	}
	printf("Trie building finished.\n");
}

int mt_trie_query(char * str, uint len){
	uint hash = 0;
	if(len > 0){
		hash = hash_code(str[0]) % global_mt.threads_num;
		return trie_query(&global_mt.trie_list[hash], str);
	}
	return 0;
}





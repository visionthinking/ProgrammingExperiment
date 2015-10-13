#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define BUILD_FILE "emaillist.dat"
#define QUERY_FILE "checklist.dat"

#include "trie.h"
#include "mt_trie.h"

unsigned int string_trim_CR_LF(char * str){
	unsigned int len = strlen(str);
	while(str[len-1] == '\n' || str[len-1] == '\r'){
		str[len-1] = 0;
		len -= 1;	
	}
	return len;
}

void trie_query_from_file(struct trie * mytrie, char * query_file_name){
	ull len, cnt_yes = 0;
	char line[1024];
	FILE * in = fopen(query_file_name, "r");
	while(fgets(line, 1024, in)){
		len = string_trim_CR_LF(line);
		if(trie_query(mytrie, line)){
			cnt_yes += 1;
			//printf("yes\n");	
		}else{
			//printf("no\n");	 
		}
	}
	fclose(in);
	
	printf("count yes: %d\n", cnt_yes);
}

void single_thread_trie(int argc, char *argv[]){
	char * build_file = BUILD_FILE;
	char * query_file = QUERY_FILE;
	struct trie mytrie;
	clock_t start = clock();
	
	if(argc == 3){
		build_file = argv[1];
		query_file = argv[2];	
	}
	
	if(trie_init(&mytrie, build_file, 1000, 50000000)){
		trie_build_from_file(&mytrie);
		trie_query_from_file(&mytrie, query_file);
	}
	printf("time: %f secs\n",  (double)(clock() - start) / CLOCKS_PER_SEC);	
}

void mt_query_from_file(char * query_file_name){
	uint len, cnt_yes = 0;
	char line[1024];
	FILE * in = fopen(query_file_name, "r");
	while(fgets(line, 1024, in)){
		len = string_trim_CR_LF(line);
		if(mt_trie_query(line, len)){
			cnt_yes += 1;
			//printf("yes\n");	
		}else{
			//printf("no\n");	 
		}
	}
	fclose(in);
	
	printf("count yes: %d\n", cnt_yes);
}

void multithreads_trie(int argc, char *argv[]){
	char * build_file = BUILD_FILE;
	char * query_file = QUERY_FILE;
	clock_t start = clock();
	
	if(argc == 3){
		build_file = argv[1];
		query_file = argv[2];	
	}
	
	mt_trie_init(build_file, 8, 60000000);
	mt_trie_build();
	mt_query_from_file(query_file);
	printf("time: %f secs\n",  (double)(clock() - start) / CLOCKS_PER_SEC);	
}

int main(int argc, char *argv[]){
	//single_thread_trie(argc, argv);
	multithreads_trie(argc, argv);
	return 0;
}



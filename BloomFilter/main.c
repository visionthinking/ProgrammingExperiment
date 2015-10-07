#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "bloom.h"

unsigned int string_trim_CR_LF(char * str){
	unsigned int len = strlen(str);
	while(str[len-1] == '\n' || str[len-1] == '\r'){
		str[len-1] = 0;
		len -= 1;	
	}
	return len;
}

void build_index(struct bloom_filter * bf, char * build_file_name){
	unsigned int len, cnt = 0;
	char line[1024];
	
	FILE * in = fopen(build_file_name, "r");
	while(fgets(line, 1024, in)){
		if(++cnt % 1000000 == 0) printf("count: %d\n", cnt);
		len = string_trim_CR_LF(line);
		if(line[len-1] == '\n'){
			len -= 1;	
		}
		bloom_filter_insert(bf, line, len);
	}
	fclose(in);
}

void query_from_file(struct bloom_filter * bf, char * query_file_name){
	unsigned int len, cnt = 0;
	char line[1024];
	
	FILE * in = fopen(query_file_name, "r");
	while(fgets(line, 1024, in)){
		len = string_trim_CR_LF(line);
		if(bloom_filter_query(bf, line, len)){
			cnt += 1;	
		}
	}
	fclose(in);
	
	printf("count yes: %d\n", cnt);
}

int main(int argc, char ** argv){
	char * build_file = "emaillist.dat";
	char * query_file = "checklist.dat";
	struct bloom_filter bf;
	
	if(argc == 3){
		build_file = argv[1];
		query_file = argv[2];	
	}
	
	clock_t start = clock();
	bloom_filter_init(&bf, 300000000);
	build_index(&bf, build_file);
	query_from_file(&bf, query_file);
	printf("time: %f secs\n",  (double)(clock() - start) / CLOCKS_PER_SEC);
	return 0;
}

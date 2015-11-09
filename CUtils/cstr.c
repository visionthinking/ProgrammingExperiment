#include "cstr.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static
int cstr_expand(struct cstr * s, uint to_len){
	char * new_block = NULL;
	if(to_len <= s->len){
		return 0;
	}
	new_block = malloc(sizeof(char) * (to_len+1));
	if(new_block){
		strncpy(new_block, s->_, s->capacity);
		free(s->_);
		s->_ = new_block;
		s->capacity = to_len;
		return 0;
	}
	return -1;
}

void cstr_init(struct cstr * s){
	s->len = 0;
	s->capacity = CSTR_INIT_CAPACITY;
	s->_ = malloc(sizeof(char) * (s->capacity+1));
	if(!s->_){
		fprintf(stderr, "cstr_init: Memory Error!");
		s->capacity = 0;
		return;
	}
	s->_[0] = '\0';
}

void cstr_append(struct cstr * s, char * str){
	uint len = strlen(str), expanded_size;
	if(len > s->capacity - s->len){
		expanded_size = s->capacity * 2;
		while(len > expanded_size - s->len){
			expanded_size *= 2;
		}
		if(cstr_expand(s, expanded_size)){
			return;	
		}
	}
	strcat(s->_, str);
	s->len += len;
}

int  cstr_find(struct cstr * s, uint start, char * str){
	char * pch;
	if(start > s->len){
		return -1;	
	}
	pch = strstr(s->_ + start, str);
	if(pch){
		return pch - s->_;	
	}
	return -1;
}

void cstr_replace(struct cstr * s, char * target, char * replacement){
	uint re_len = strlen(replacement), tar_len = strlen(target);
	int index = cstr_find(s, 0, target);
	if(index != -1){
		if(s->len - tar_len + re_len > s->capacity){
			if(cstr_expand(s, s->len - tar_len + re_len)){
				return;
			}
		}
		if(re_len != tar_len){
			memmove(s->_ + index + re_len, s->_ + index + tar_len, s->len - (index + tar_len) );
		}
		strncpy(s->_ + index, replacement, re_len);
		s->len = s->len + re_len - tar_len;
		s->_[s->len] = '\0';
	}
}

void cstr_free(struct cstr * s){
	free(s->_);
	s->_ = NULL;
	s->len = 0;
	s->capacity = 0;
}



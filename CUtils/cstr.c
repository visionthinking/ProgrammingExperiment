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
	fprintf(stdout, "cstr_expand: Memory Error!");
	return -1;
}

void cstr_init(struct cstr * s){
	s->len = 0;
	s->capacity = CSTR_INIT_CAPACITY;
	s->_ = malloc(sizeof(char) * (s->capacity+1));
	if(!s->_){
		fprintf(stdout, "cstr_init: Memory Error!");
		s->capacity = 0;
		return;
	}
	s->_[0] = '\0';
}

void cstr_copy(struct cstr * s, struct cstr * src){
	cstr_init(s);
	cstr_append(s, src->_);
}

void cstr_append(struct cstr * s, char * str){
	uint len, expanded_size;
	if(str == NULL) return;
	len = strlen(str);
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

int  cstr_find_last(struct cstr * s, char * str){
	char * pch;
	uint len = strlen(str);
	if(len > s->len){
		return -1;	
	}
	if(len == 0){
		return 0;	
	}
	pch = s->_ + s->len - len;
	while(pch >= s->_){
		if(strncmp(pch, str, len) == 0){
			return pch - s->_;
		}
		pch -= 1;
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

void cstr_remove(struct cstr * s, uint start){
	if(start > s->len) return;
	s->_[start] = '\0';
	s->len = start;
}

void cstr_substr(struct cstr * s, uint start, uint len){
	if(start > s->len){
		cstr_remove(s, 0);
		return;
	}
	len = (s->len - start < len) ? s->len - start : len;
	memmove(s->_, s->_ + start, len);
	s->_[len] = '\0';
	s->len = len;
}

int  cstr_postfix(struct cstr * s, char * postfix){
	uint len = strlen(postfix);
	int pos;
	if(len == 0){
		return 0;	
	}
	if(len <= s->len){
		if((pos = cstr_find(s, s->len - len, postfix)) >= 0){
			return pos;
		}
	}
	return -1;
}

void cstr_trim(struct cstr * s){
	uint l = 0, r = s->len - 1;
	if(s->len == 0) return;
	while(l < r && s->_[l] <= 32) l+=1;
	while(r >= l && s->_[r] <= 32) r-=1;
	if (r < l) {
		cstr_remove(s, 0);
	}else{
		cstr_substr(s, l, r - l + 1);
	}
}

void cstr_free(struct cstr * s){
	if(s->_) free(s->_);
	s->_ = NULL;
	s->len = 0;
	s->capacity = 0;
}



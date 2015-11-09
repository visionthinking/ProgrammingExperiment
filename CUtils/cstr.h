/*
Author: RainVision
Date:   2015/11/09
*/

#ifndef __CSTR_H__
#define __CSTR_H__

#define uint unsigned int
#define CSTR_INIT_CAPACITY 256

struct cstr {
	char * _;
	uint len;
	
	uint capacity;
};

void cstr_init(struct cstr * s);
void cstr_append(struct cstr * s, char * str);
int  cstr_find(struct cstr * s, uint start, char * str);
void cstr_replace(struct cstr * s, char * target, char * replacement);
void cstr_free(struct cstr * s);

#endif

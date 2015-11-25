/*
Author: RainVision
Date:   2015/11/09
*/

#ifndef __CSTR_H__
#define __CSTR_H__

#define CSTR_INIT_CAPACITY 256
typedef unsigned int uint;

struct cstr {
	char * _;
	uint len;
	uint capacity;
};

typedef struct cstr * cstr_t;

/* should be cstr_delete after used */
cstr_t cstr_new();
void cstr_delete(cstr_t * s);
/* should be cstr_free after used */
void cstr_init(struct cstr * s);
void cstr_free(struct cstr * s);
/* should be cstr_free after used */
void cstr_copy(struct cstr * s, struct cstr * src);
void cstr_insert(struct cstr * s, uint start, char * str, uint len);
void cstr_append(struct cstr * s, char * str);
void cstr_nappend(struct cstr * s, char * str, uint len);
int  cstr_find(struct cstr * s, uint start, char * str);
int  cstr_find_last(struct cstr * s, char * str);
uint cstr_replace(struct cstr * s, char * target, char * replacement);
uint cstr_replace_all(struct cstr * s, char * target, char * replacement);
void cstr_remove(struct cstr * s, uint start);
void cstr_substr(struct cstr * s, uint start, uint len);
/* return the start position of postfix, otherwise -1. */
int  cstr_postfix(struct cstr * s, char * postfix);
/* trim blanks (ascii <= 0x20) */
void cstr_trim(struct cstr * s);

#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "cstr.h"

void test_cstr_append(){
	struct cstr s0;
	printf("test_cstr_append: \n");
	cstr_init(&s0);
	cstr_append(&s0, NULL);
	cstr_append(&s0, "   ");
	cstr_append(&s0, "hello");
	cstr_append(&s0, NULL);
	printf("[%s] len=%u capacity=%u\n\n", s0._, s0.len, s0.capacity);
	cstr_free(&s0);		
}

void test_cstr_nappend(){
	struct cstr s0;
	printf("test_cstr_nappend: \n");
	cstr_init(&s0);
	cstr_nappend(&s0, NULL, 0);
	cstr_nappend(&s0, "xxx", 3);
	cstr_nappend(&s0, "123", 1);
	cstr_nappend(&s0, NULL, 0);
	printf("[%s] len=%u capacity=%u\n\n", s0._, s0.len, s0.capacity);
	cstr_free(&s0);		
}

void test_cstr_copy(){
	struct cstr s0, s1;
	printf("test_cstr_copy: \n");
	cstr_init(&s0);
	cstr_init(&s1);
	cstr_append(&s1, "hello");
	cstr_copy(&s0, &s1);
	printf("[%s] len=%u capacity=%u\n\n", s0._, s0.len, s0.capacity);
	cstr_free(&s0);		
}

void test_cstr_postfix(){
	struct cstr s0;
	printf("test_cstr_postfix: \n");
	cstr_init(&s0);
	cstr_append(&s0, ".html");
	printf("postfix: %d\n", cstr_postfix(&s0, "tml"));
	printf("[%s] len=%u capacity=%u\n\n", s0._, s0.len, s0.capacity);
	cstr_free(&s0);
}

void test_cstr_find_last(){
	struct cstr s0;
	printf("test_cstr_find_last: \n");
	cstr_init(&s0);
	cstr_append(&s0, "");
	printf("find last: %d\n", cstr_find_last(&s0, ""));
	printf("[%s] len=%u capacity=%u\n\n", s0._, s0.len, s0.capacity);
	cstr_free(&s0);
}

void test_cstr_replace(){
	struct cstr s0;
	printf("test_cstr_replace: \n");
	cstr_init(&s0);
	cstr_append(&s0, "abc.xyz");
	cstr_replace(&s0, "c.x", "w");
	assert(strcmp(s0._, "abwyz") == 0);
	cstr_replace(&s0, s0._, "");
	printf("[%s] len=%u capacity=%u\n\n", s0._, s0.len, s0.capacity);
	cstr_free(&s0);
}

void test_cstr_replace_all(){
	struct cstr s0;
	printf("test_cstr_replace_all: \n");
	cstr_init(&s0);
	cstr_append(&s0, "10_3_8_211:8080");
	cstr_replace_all(&s0, "_", ".");
	assert(strcmp(s0._, "10.3.8.211:8080") == 0);
	printf("[%s] len=%u capacity=%u\n\n", s0._, s0.len, s0.capacity);
	cstr_free(&s0);
}

void test_cstr_substr(){
	struct cstr s0;
	printf("test_cstr_substr: \n");
	cstr_init(&s0);
	cstr_append(&s0, "abc.xyz");
	cstr_substr(&s0, 3000, 10000);
	printf("[%s] len=%u capacity=%u\n\n", s0._, s0.len, s0.capacity);
	cstr_free(&s0);
}


void test_cstr_trim(){
	struct cstr s0;
	printf("test_cstr_trim: \n");
	cstr_init(&s0);
	cstr_append(&s0, "\n");
	cstr_trim(&s0);
	printf("[%s] len=%u capacity=%u\n\n", s0._, s0.len, s0.capacity);
	cstr_free(&s0);
}

int main(void){
	test_cstr_append();
	test_cstr_nappend();
	test_cstr_copy();
	test_cstr_postfix();
	test_cstr_find_last();
	test_cstr_replace();
	test_cstr_replace_all();
	test_cstr_substr();
	test_cstr_trim();
	return 0;
}

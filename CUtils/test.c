#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cstr.h"

void test_cstr_append(){
	struct cstr s0;
	printf("test_cstr_append: \n");
	cstr_init(&s0);
	cstr_append(&s0, NULL);
	cstr_append(&s0, "   ");
	cstr_append(&s0, "hellohellohellohellohellohellohellohello");
	cstr_append(&s0, NULL);
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

int main(void){
	struct cstr s, s0, s1;
	int pos;
	
	test_cstr_append();
	test_cstr_copy();
	test_cstr_postfix();
	test_cstr_find_last();
	test_cstr_replace();
	test_cstr_substr();
	return 0;
}

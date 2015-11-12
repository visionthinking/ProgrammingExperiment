#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cstr.h"

int main(void){
	struct cstr s, s0;
	int pos;
	
	cstr_init(&s);
	printf("len=%u capacity=%u\n", s.len, s.capacity);
	
	cstr_append(&s, NULL);
	cstr_append(&s, "Hello!");
	printf("%s len=%u capacity=%u\n", s._, s.len, s.capacity);
	
	printf("%d\n", cstr_find(&s, 0, "lo"));
	
	cstr_replace(&s, "ll", "xxxxx");
	printf("[%s] len=%u capacity=%u\n", s._, s.len, s.capacity);
	
	cstr_substr(&s, 2, 5);
	printf("[%s] len=%u capacity=%u\n", s._, s.len, s.capacity);
	
	cstr_copy(&s0, &s);
	printf("[%s] len=%u capacity=%u\n", s0._, s0.len, s0.capacity);
	
	cstr_remove(&s0, 0);
	cstr_append(&s0, "http://10.3.8.211:8080/index/");
	if((pos = cstr_find(&s0, 0, "http://")) >= 0){
		cstr_substr(&s0, 7, s0.len);
	}
	printf("[%s] len=%u capacity=%u\n", s0._, s0.len, s0.capacity);
	
	cstr_free(&s);
	cstr_free(&s0);
	return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cstr.h"

int main(void){
	struct cstr s, s0, s1;
	int pos;
	
	cstr_init(&s);
	cstr_init(&s1);
	printf("len=%u capacity=%u\n", s.len, s.capacity);
	
	cstr_append(&s, NULL);
	cstr_append(&s, "   ");
	cstr_append(&s1, " 123 ");
	cstr_trim(&s);
	cstr_trim(&s1);
	printf("[%s] len=%u capacity=%u\n", s._, s.len, s.capacity);
	printf("[%s] len=%u capacity=%u\n", s1._, s1.len, s1.capacity);
	printf("%d\n", cstr_find(&s, 0, "123"));
	
	cstr_append(&s, "  Hello!  ");
	cstr_trim(&s);
	printf("[%s] len=%u capacity=%u\n", s._, s.len, s.capacity);
	
	printf("%d\n", cstr_find(&s, 0, "lo"));
	
	cstr_replace(&s, "ll", "xxxxx");
	printf("[%s] len=%u capacity=%u\n", s._, s.len, s.capacity);
	
	cstr_substr(&s, 2, 5);
	printf("[%s] len=%u capacity=%u\n", s._, s.len, s.capacity);
	
	cstr_copy(&s0, &s);
	printf("[%s] len=%u capacity=%u\n", s0._, s0.len, s0.capacity);
	
	cstr_remove(&s0, 0);
	cstr_append(&s0, "http://10.3.8.211:8080/xxxxx/index.html");
	if((pos = cstr_find(&s0, 0, "http://")) >= 0){
		cstr_substr(&s0, 7, s0.len);
	}
	printf("[%s] len=%u capacity=%u\n", s0._, s0.len, s0.capacity);
	
	pos = cstr_postfix(&s0, ".html");
	printf("postfix: %d\n", pos);
	cstr_remove(&s0, pos);
	printf("[%s] len=%u capacity=%u\n", s0._, s0.len, s0.capacity);
	
	pos = cstr_find_last(&s0, ":80");
	printf("cstr_find_last: %d\n", pos);
	
	cstr_replace(&s0, "10.3.8.211", "");
	printf("[%s] len=%u capacity=%u\n", s0._, s0.len, s0.capacity);
	
	cstr_free(&s);
	cstr_free(&s0);
	return 0;
}

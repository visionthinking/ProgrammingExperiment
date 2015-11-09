#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cstr.h"

int main(void){
	struct cstr s;
	cstr_init(&s);
	printf("len=%u capa=%u\n", s.len, s.capacity);
	cstr_append(&s, "Hello!");
	printf("%s len=%u capa=%u\n", s._, s.len, s.capacity);
	printf("%d\n", cstr_find(&s, 0, "lo!"));
	printf("%d\n", cstr_find(&s, 6, "lo!"));
	cstr_replace(&s, "lo!", "123123");
	printf("%s len=%u capa=%u\n", s._, s.len, s.capacity);
	cstr_free(&s);
	return 0;
}

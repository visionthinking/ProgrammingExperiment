#include "trie.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

inline 
ull get_char_bit(char x, ull index){
	return (x >> index) & 1;	
}

inline
ull get_word_bit(char * word, ull index){
	return get_char_bit(word[index / BITS_USED], index % BITS_USED);	
}

inline
ull get_global_bit(struct trie * mytrie, ull index){
	return get_char_bit(mytrie->words[index / BITS_USED], index % BITS_USED);
}

inline
ull get_node_bit(struct trie * mytrie, ull node_index, ull index){
	return get_word_bit(mytrie->words, mytrie->t[node_index].bit_start + index);	
}

void string_lower(char * str){
	for(;str[0];str++){
		str[0] = tolower(str[0]);
	}
}

inline
ull create_node(struct trie * mytrie, ull is_word_end, ull bit_start, ull bit_len, ull child0, ull child1){
	struct node * t = mytrie->t;
	ull tlen = mytrie->tlen;
	t[tlen].is_word_end = is_word_end;
	t[tlen].bit_start = bit_start;
	t[tlen].bit_len = bit_len;
	t[tlen].child[0] = child0;
	t[tlen].child[1] = child1;
	mytrie->tlen += 1;
	return tlen;	
}

void trie_insert(struct trie * mytrie, ull from, ull to){
	struct node * t = mytrie->t;
	ull i, j, x, y, current_node, current_bit, t1, t2;
	if(to < from) return;
	
	current_node = 0;
	current_bit = 0;
	from = from * BITS_USED;
	to = to * BITS_USED + 7;
	
	for(i=from;i<=to;i++){
		x = get_global_bit(mytrie, i);
		if(current_bit >= t[current_node].bit_len){
			if(t[current_node].child[x] == 0){
				//move to a leaf, insert
				t[current_node].child[x] = 
					create_node(mytrie, 
								1, 
								i, 
								to+1-i, 
								0, 0);
				return;		
			}
			current_node = t[current_node].child[x];
			current_bit = 0;
		}
		y = get_node_bit(mytrie, current_node, current_bit);
		if(x != y){
			//split from mid of node, one node -> three nodes
			t1 = t[current_node].child[0];
			t2 = t[current_node].child[1];
			
			t[current_node].child[x] = 
				create_node(mytrie, 
							1,
							i,
							to+1-i,
							0, 0);
			
			t[current_node].child[y] = 
				create_node(mytrie, 
							t[current_node].is_word_end, 
							t[current_node].bit_start + current_bit,
							t[current_node].bit_len - current_bit,
							t1, t2);
			
			t[current_node].is_word_end = 0;
			t[current_node].bit_len = current_bit;
			return;	
		}
		current_bit += 1;
	}
	if(current_bit == t[current_node].bit_len && t[current_node].is_word_end){
		//duplicated, do nothing
	}else{
		//new string have consumed, but stay in a node
		y = get_node_bit(mytrie, current_node, current_bit);
		
		t1 = t[current_node].child[0];
		t2 = t[current_node].child[1];
		
		t[current_node].child[1-y] = 0;
		t[current_node].child[y] = 
			create_node(mytrie, 
						t[current_node].is_word_end,
						t[current_node].bit_start + current_bit,
						t[current_node].bit_len - current_bit,
						t1, t2);
		t[current_node].is_word_end = 1;
		t[current_node].bit_len = current_bit;
	}
}

void trie_build_from_file(struct trie * mytrie){
	char * words = mytrie->words;
	ull cnt_line = 0, p = 0, len;
	FILE * in = fopen(mytrie->INDEX_FILE_NAME, "r");
	if(!in){
		printf("Index file doesn't exist.\n");
		exit(1);	
	}
	while(fgets(words + p, 1024, in)){
		if(++cnt_line % 1000000 == 0) printf("line: %10d  nodes: %10d\n", cnt_line, mytrie->tlen);
		
		len = strlen(words + p);
		while(words[p + len - 1] == '\n' || words[p + len - 1] == '\r'){
			words[p + len - 1] = 0;
			len -= 1;
		}
		//string_lower(words + p);
		trie_insert(mytrie, p, p + len - 1);
		p += len + 1;
	}
	fclose(in);	
}

int trie_init(struct trie * mytrie, char * index_file_name, size_t file_size_MB, uint max_nodes){
	mytrie->INDEX_FILE_SIZE_MB = file_size_MB;
	mytrie->MAX_NODES = max_nodes;
	strcpy(mytrie->INDEX_FILE_NAME, index_file_name);
	mytrie->tlen = 0;
	mytrie->words = (char*) malloc(sizeof(char) * 1024 * 1024 * (file_size_MB));
	mytrie->t = (struct node*) malloc(sizeof(struct node) * max_nodes);
	if(!mytrie->words || !mytrie->t){
		printf("Out of Memory\n");
		return 0;
	}
	create_node(mytrie, 0, 0, 0, 0, 0);
	
	trie_build_from_file(mytrie);
	return 1;
}

int trie_query(struct trie * mytrie, char * str){
	struct node * t = mytrie->t;
	ull i, x, current_node, current_bit, y, from, to;
	
	current_node = 0;
	current_bit = 0;
	from = 0;
	to = strlen(str) * BITS_USED - 1;
	
	for(i=from;i<=to;i++){
		x = get_word_bit(str, i);
		while(current_bit >= t[current_node].bit_len){
			if(t[current_node].child[x] == 0){
				return 0;
			}
			current_node = t[current_node].child[x];
			current_bit = 0;
		}
		y = get_node_bit(mytrie, current_node, current_bit);
		if(x != y){
			return 0;	
		}
		current_bit += 1;
	}
	if(t[current_node].is_word_end && current_bit == t[current_node].bit_len){
		return 1;
	}else{
		return 0;
	}
}




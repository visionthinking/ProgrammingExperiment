#include "bloom.h"
#include "hash.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define SLOT_BITS 32
#define SLOT_F 5

inline
void bf_set_bit(u32 * data, uint index){
	data[index / SLOT_BITS] |= (1 << (index % SLOT_BITS));
}

inline
int bf_get_bit(u32 * data, uint index){
	return (data[index / SLOT_BITS] >> (index % SLOT_BITS)) & 1;
}

int bloom_filter_init(struct bloom_filter * bf, uint bit_len){
	uint mem_bytes = 0;
	
	bf->hash_func_list[0] = RSHash;  
	bf->hash_func_list[1] = JSHash;  
	bf->hash_func_list[2] = PJWHash; 
	bf->hash_func_list[3] = ELFHash; 
	bf->hash_func_list[4] = BKDRHash;
	bf->hash_func_list[5] = SDBMHash;
	bf->hash_func_list[6] = DJBHash; 
	bf->hash_func_list[7] = DEKHash; 
	bf->hash_func_list[8] = BPHash;
	bf->hash_func_list[9] = FNVHash; 
	bf->hash_func_list[10] = APHash;
	bf->hash_func_list_len = 11;
	
	mem_bytes = sizeof(u32) * (bit_len / SLOT_BITS + 1);
	bf->bit_len = bit_len;
	bf->data = malloc(mem_bytes);
	memset(bf->data, 0, mem_bytes);
	printf("bit_len: %u mem: %uMb\n", bf->bit_len, mem_bytes / 1024 / 1024);
}                        
                         
void bloom_filter_insert(struct bloom_filter * bf, char * str, uint len){
	int i;
	uint bit_len = bf->bit_len;
	for(i=0;i<bf->hash_func_list_len;i+=1){
		bf_set_bit(bf->data, bf->hash_func_list[i](str, len) % bit_len);
	}
}                        
            
int bloom_filter_query(struct bloom_filter * bf, char * str, uint len){
	int i; 
	uint bit_len = bf->bit_len;
	for(i=0;i<bf->hash_func_list_len;i+=1){
		if(bf_get_bit(bf->data, bf->hash_func_list[i](str, len) % bit_len) == 0){
			return 0;	
		}
	}
	return 1;
}








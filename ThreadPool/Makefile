trie: main.o thread_pool.o
	gcc -o $@ $^ -lpthread
.c .o:
	gcc -c $<
clean:
	rm *.o
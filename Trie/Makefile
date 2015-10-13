trie: main.o trie.o mt_trie.o
	gcc -m64 -o $@ $^ -lpthread
.c .o:
	gcc -m64 -c $<
clean:
	rm *.o
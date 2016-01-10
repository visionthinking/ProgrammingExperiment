main: main.o
	g++ -std=c++11 -o $@ $^
%.o: %.cpp
	g++ -std=c++11 -c $<
clean:
	rm *.o
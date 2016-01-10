#include <iostream>
#include "graph.h"

using namespace std;

int main(int argc, char ** argv){
	const char * file_path = "test.txt";
	double q               = 0.15;
	int iter_num           = 10;
	if(argc >= 2){
		file_path = argv[1];
	}
	if(argc >= 3){
		q = atof(argv[2]);
	}
	if(argc >= 4){
		iter_num = atoi(argv[3]);
	}

	Graph g;
	g.loadEdgesFromFile(file_path);
	cout << "Nodes: " << g.getNodesNum() << endl;
	cout << "Edges: " << g.getEdgesNum() << endl;

	auto & cycles = g.findCycles(false);
	cout << "Cycles found: " << g.getCyclesNum() << endl;
	g.printCycles(cycles);

	cout << "PageRank: " << endl;
	g.calcPageRank(q, iter_num);
	g.printTopRanks(20);

	return 0;
}


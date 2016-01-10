#ifndef __GRAPH_H__
#define __GRAPH_H__

#include <stdio.h>
#include <assert.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;

class Graph {
public:
	Graph() : idx(0), edges(0) {};

	inline int getNodesNum(){
		return idx;
	};
	inline int getEdgesNum(){
		return edges;
	};

	void loadEdgesFromFile(const char * file_name){
		fstream fp(file_name);
		assert(fp);
		string line;
		while(getline(fp, line)){
			if(line.length() == 0) continue;
			stringstream ss;
			string url, url_link;
			ss << line;
			ss >> url >> url_link;
			int url_id  = getUrlIdx(url);
			int link_id = getUrlIdx(url_link);
			links[url_id].push_back(link_id);
			edges += 1;
		}
		fp.close();
	};

	const vector< vector<int> > & findCycles(bool make_output = true){
		cycles.clear();
		cycles_num = 0;
		vector< char > marks(getNodesNum(), 0);
		vector< find_cycle_stack* > s;
		
		for(int k=getNodesNum()-1; k>=0; k-=1){
			if(marks[k] != 0){
				continue;
			}
			s.push_back(new find_cycle_stack(k, 0));
			while(s.size() > 0){
				find_cycle_stack & curr = *s.back();
				const vector<int> & link = links[curr.id];
				if(curr.access < link.size()){
					// cout << "Node: " << curr.id << " Label: " << id_label[curr.id] << endl;
					// cout << "access: " << curr.access << " ";
					marks[curr.id] = -1;
					int i, n;
					for(i=curr.access; i<link.size(); i+=1){
						n = link[i];
						if(marks[n] == -1){
							if(make_output){
								vector<int> cycle;
								cycle.push_back(n);
								for (int j=s.size()-1; j>=0; j-=1){
									cycle.push_back(s[j]->id);
									if(s[j]->id == n){
										break;
									}
								}
								reverse(cycle.begin(), cycle.end());
								cycles.push_back(cycle);
							}
							cycles_num += 1;
						}
						if(marks[n] == 0){
							s.push_back(new find_cycle_stack(n, 0));
							break;
						}
					}
					curr.access = i + 1;
					// cout << curr.access << endl;
				}else{
					marks[curr.id] = 1;
					delete s.back();
					s.pop_back();
				}
			}	
		}
		return cycles;
	};

	void printCycles(const vector< vector<int> > & cycles){
		for (auto it = cycles.begin(); it != cycles.end(); ++it)	{
			cout << id_label[*(it->begin())];
			for (std::vector<int>::const_iterator i = (*it).begin() + 1; i != (*it).end(); ++i){
				cout << " -> " << id_label[*i];
			}
			cout << endl;
		}
	};

	inline int getCyclesNum(){
		return cycles_num;
	};

	const vector<double> & calcPageRank(double q = 0.15, int iter_num = 30){
		int N = getNodesNum();
		ranks.clear();
		ranks.resize(N, 1.0 / N);
		for(int iter = 0; iter < iter_num; iter += 1){
			// cout << "Round " << iter + 1 << endl;
			vector<double> contributes(N, 0.0);
			double no_out_rank_sum = 0.0;
			for (auto i = links.begin(); i != links.end(); ++i){
				const int url = i->first;
				const double rank = ranks[url];
				vector<int> & urls = i->second;
				int urls_num = urls.size();
				if(urls_num > 0){
					for (auto j = urls.begin(); j != urls.end(); ++j){
						int link = *j;
						contributes[link] += rank / urls_num;
					}
				}else{
					no_out_rank_sum += rank;
				}
			}
			for (auto j = contributes.begin(); j != contributes.end(); ++j){
				*j += no_out_rank_sum / N;
				*j = q / N + (*j) * (1 - q);
			}
			ranks = contributes;
		}
		return ranks;
	};

	void printTopRanks(int k){
		vector< NodeRank > v;
		for (int i = 0; i < ranks.size(); i+=1){
			NodeRank n;
			n.id = i;
			n.rank = ranks[i];
			v.push_back(n);
		}
		sort(v.begin(), v.end(), [](const NodeRank & a, const NodeRank & b){
			return a.rank > b.rank;
		});
		int len = v.size() < k ? v.size() : k;
		for (int i = 0; i < len; ++i){
			printf("%-8d: %f  ", v[i].id, v[i].rank);
			printf("page: %s\n", id_label[v[i].id].c_str());
		}
	};

private:
	int getUrlIdx(const string & url){
		unordered_map<string, int>::iterator found = label_id.find(url);
		if(found != label_id.end()){
			return found->second;
		}
		label_id[url] = idx;
		id_label[idx] = url;
		vector<int> v;
		links[idx] = v;
		return idx ++;
	};
	int idx;
	int edges;
	unordered_map<string, int> label_id;
	unordered_map<int, string> id_label;
	unordered_map< int, vector<int> > links;
	/* For finding cycles */
	struct find_cycle_stack {
		find_cycle_stack(int _id, int _access)
			: id(_id), access(_access) {};
		int id;
		int access;
	};
	vector< vector<int> > cycles;
	int cycles_num;

	/* For Page Rank */
	struct NodeRank {
		int id;
		double rank;
	};
	vector<double> ranks;
};

#endif
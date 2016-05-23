#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>
#include <algorithm>
#include <list>
#include "Ant.h"
//#include "Common.h"
#include "DPSP.h"
#include "Graph.h"
#include "GeneAlg.h"

#define INF 65535
#define POINT_NUM 600

const char *result_name;

using namespace std;

void g_cost_init();
void search_by_enum(Graph &G, int start, int end, vector<int> &demand, vector<int> &best);
void write_file(const char *name, vector<int> &point, const Graph &G);
int get_path_course(vector<int> &gene);

extern vector<int> demand;
set<int> s;
vector<int> best;
int best_dist = 65535;
	
Graph G;

int main(int argc, char *argv[])
{
	srand((unsigned)time(NULL));
	cout << "start time:" << clock()*1000/CLOCKS_PER_SEC << "ms"<< endl;


	int edge_num = 0;
	int start;
	int end;
	int temp;
	
	int a,b,c,d;
	char ch;

	ifstream ing(argv[1]);
	ifstream ind(argv[2]);
	result_name = argv[3];

//	ifstream ing("D://topo.csv");
//	ifstream ind("D://demand.csv");
//	result_name = "result.csv";

	g_cost_init();

 	while(ing >> a >> ch >> b >> ch >> c >> ch >> d)
	{
		G.vertex_add(b);
		G.vertex_add(c);
		if(max_ver < b)
		{
			max_ver = b;
		}
		if(max_ver < c)
		{
			max_ver = c;
		}
		edge_num++;
		out_point[b].push_back(c);
		G.edge_add(b,c,d,a);
		if(g_cost[b][c] > d) 
		{
			g_cost[b][c]= d;
		}
	}	
	ind >> start >> ch >> end >> ch;
	start_point = start;
	end_point = end;
	while(ind >> temp)
	{
		ind >> ch;	
		s.insert(temp);
		demand.push_back(temp);
	}
	if(!G.modify(start, end, s))
	{
		ofstream out(result_name);
		out << "NA" << endl;
		return 0;
	}
	if(demand.size() <= 4)
	{
//		cout << "enum" << endl;
		search_by_enum(G, start, end, demand, best);
		write_file(result_name, best, G);
//		cout << "end time: " << clock()*1000/CLOCKS_PER_SEC << "ms"<< endl;
		return 0;
	}
	else if(max_ver < 220)
	{	
		int dist_temp;
		vector<int> v;	
		while(1)
		{
			GeneAlg gene(G, demand, start, end);
			gene.iterative(v, dist_temp);
			if(best_dist > dist_temp)
			{
				best = v;
				gene.report();
				best_dist = dist_temp;
			}
			if(clock()*1000/CLOCKS_PER_SEC > 9000)
			{			
				break;
			}
		}
		
		cout << "end time: " << clock()*1000/CLOCKS_PER_SEC << "ms"<< endl;
		return 0;
	}
	else
	{
		ant_num = max_ver * 2 / 3;
		iter_num = edge_num / 10 + 1;
		cout << ant_num << endl;
		cout << iter_num << endl;
		DPSP dpsp;
		dpsp.init_data();
		dpsp.search();
		dpsp.report();
		cout << "end time: " << clock()*1000/CLOCKS_PER_SEC << "ms"<< endl;
	}
}

void search_by_enum(Graph &G, int start, int end, vector<int> &demand, vector<int> &best)
{
	int dist = 0;
	int dist_best = 65535;
	vector<int> result;
	if(demand.size() <= 7)
	{
		sort(demand.begin(), demand.end());
	}		
	do
	{
		result.clear();
		if(clock()*1000/CLOCKS_PER_SEC > 9000)
		{
			break;
		}
		if(G.search_path_withset(start, end, result, dist, demand))
		{
			if(dist<dist_best)
			{
				best = result;
				dist_best = dist;
			}
		}
	}while(next_permutation(demand.begin(), demand.end()));
}

void write_file(const char *name, vector<int> &point, const Graph &G)
{
	ofstream out(name);
	int edge;
	if(!point.empty())
	{
		for(vector<int>::iterator it=point.begin(); it!=point.end()-1; ++it)
		{
			if(it!=point.end()-2)
			{
				edge = G.get_edge_id(*it, *(it+1));
				out << edge << '|';
//				cout << edge << '|';			
			}
			else
			{
				edge = G.get_edge_id(*it, *(it+1));
				out << edge << endl;
//				cout << edge << endl;
			}
		}
	}	
	else
	{
		out << "NA" << endl;
	}
}

int get_path_course(vector<int> &gene)
{
	int sum = 0;
	if(gene.empty())
	{
		return 0;
	}
	for(vector<int>::iterator it=gene.begin(); it!=gene.end()-1; ++it)
	{
		sum += G.get_edge_cost(*it, *(it+1));
	}
	return sum;
}

void g_cost_init()
{
	for(int i=0; i<POINT_NUM; i++)
	{
		for(int j=0; j<POINT_NUM; j++)
		{
			if(i==j)
			{
				g_cost[i][j] = 0;
			}
			else
			{
				g_cost[i][j] = INF;
			}
		}
	}
	
}

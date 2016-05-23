#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>
#include <algorithm>
#include <list>
#include <vector>
#include <unordered_set>
#include <queue>
#include <iterator>
#include "DPSP_MIP.h"
#include "DPSP.h"
#include "Ant.h"

const char *result_name;

using namespace std;

void init();
void print(vector<int> &v, ostream &os);
void write_file(const char *name, vector<int> &point);
void search_with_MIP();
void search_with_ANT();
void pri_modify();

int start_point;
int end_point;
int max_ver = 0;
int edge_num = 0;
double g_cost[MAX][MAX];
int id[MAX][MAX];

int main(int argc, char *argv[])
{
	srand((unsigned)time(NULL));
	cout << "start time:" << clock()*1000/CLOCKS_PER_SEC << "ms"<< endl;

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

	init();

 	while(ing >> a >> ch >> b >> ch >> c >> ch >> d)
	{
		if(max_ver < b)
		{
			max_ver = b;
		}
		if(max_ver < c)
		{
			max_ver = c;
		}
		out_point[b].push_back(c);
		out_way[b].insert(c);
		in_way[c].insert(b);
		node.insert(b);
		node.insert(c);
		if(g_cost[b][c] == INF) 
		{
			edge_num++;
			edge_set[edge_num].start = b;
			edge_set[edge_num].end = c;
			edge_index[b][c] = edge_num;
			g_cost[b][c] = d;
			id[b][c] = a; 
		}
		else if(g_cost[b][c] > d)
		{
			edge_set[edge_index[b][c]].start = b;
			edge_set[edge_index[b][c]].end = c;
			g_cost[b][c] = d;
			id[b][c] = a;
		}
	}	
	ind >> start >> ch >> end >> ch;
	start_point = start;
	end_point = end;
	g_cost[start][end] = INF;
	g_cost[end][start] = INF;
	while(ind >> temp)
	{
		ind >> ch;	
		demand_node.insert(temp);
		demand.push_back(temp);
	}

//	pri_modify();
	if (max_ver < 500)
	{
		search_with_MIP();
	}
	else if(max_ver > 500 && demand_node.size() > 25)
	{
		search_with_MIP();
	}
	else 
	{		
		search_with_ANT();
	}
	return 0;

}

void search_with_ANT()
{
	ant_num = max_ver * 2 / 3;
	iter_num = edge_num / 10 + 1;
	cout << edge_num << endl;
	cout << ant_num << endl;
	DPSP dpsp;
	dpsp.init_data();
	dpsp.search();
	dpsp.report();
}
void search_with_MIP()
{
	vector<int> result;

	build_prob();
 
	do
	{
		get_solve(result);
	} while (!judge_result(result));
	
	cout << "path: ";

	write_file(result_name, result);
	glp_delete_prob(P);
	cout << "end time:" << clock() * 1000 / CLOCKS_PER_SEC << "ms" << endl;
}

void init()
{
	for(int i=0; i<MAX; i++)
	{
		for(int j=0; j<MAX; j++)
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

void print(vector<int> &v, ostream &os)
{
	for(vector<int>::iterator itv=v.begin();itv!=v.end(); ++itv)
	{
		os << *itv << " ";
	}
	os << endl;
}

void write_file(const char *name, vector<int> &point)
{
	ofstream out(name);
	if(!point.empty())
	{
		for(vector<int>::iterator it=point.begin(); it!=point.end()-1; ++it)
		{
			if(it!=point.end()-2)
			{
				out << id[*it][*(it+1)] << '|';
				cout << id[*it][*(it+1)] << '|';			
			}
			else
			{
				out << id[*it][*(it+1)] << endl;
				cout << id[*it][*(it+1)] << endl;
			}
		}
	}	
	else
	{
		out << "NA" << endl;
	}
}

void pri_modify()
{
	queue<int> q;
	unordered_set<int> unreach;
	unordered_set<int> visited;
	visited.insert(start_point);
	q.push(start_point);
	while (!q.empty())
	{
		for (auto elem : out_way[q.front()])
		{
			if (visited.find(elem) == visited.end() && elem != end_point)
			{
				q.push(elem);
				visited.insert(elem);
			}
		}
		q.pop();
	}
	for (auto it = node.begin(); it != node.end(); ++it)
	{
		if (visited.find(*it) == visited.end())
		{
			unreach.insert(*it);
		}
	}
	unreach.erase(end_point);
	for (auto it = unreach.begin(); it != unreach.end(); ++it)
	{
		for (auto elem : out_way[*it])
		{
			g_cost[*it][elem] = INF;
		}
		for (auto elem : in_way[*it])
		{
			g_cost[elem][*it] = INF;
		}
			
	}
}

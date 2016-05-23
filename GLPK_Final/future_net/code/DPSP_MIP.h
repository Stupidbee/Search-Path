#ifndef _DPSP_MIP_H
#define _DPSP_MIP_H

#include <iostream>
#include <cstring>
#include <fstream>
#include <cmath>
#include <vector>
#include <set>
#include <algorithm>
#include <unordered_set>
#include <utility>
#include "glpk.h"

#define MAX_EDGE 5000
#define MAX 600
#define INF 65535

using std::unordered_set;
using std::set;
using std::vector;

typedef struct
{
	int start;
	int end;
} EdgeType;

extern int solve_row[MAX_EDGE];
extern EdgeType edge_set[MAX_EDGE];

extern int edge_index[MAX][MAX];
extern double g_cost[MAX][MAX];
extern int id[MAX][MAX];
extern unordered_set<int> out_way[MAX];
extern unordered_set<int> in_way[MAX];

extern int solve[MAX][MAX];

extern int max_ver;
extern int edge_num;
extern glp_prob *P;
extern unordered_set<int> demand_node;
extern unordered_set<int> node;
extern int start_point;
extern int end_point;

void build_prob(void);
void get_solve(vector<int> &result);
void print(vector<int> &v, std::ostream &os);
void subset_del(vector<int> &src, unordered_set<int> &subset);
bool judge_result(vector<int> &result);

#endif
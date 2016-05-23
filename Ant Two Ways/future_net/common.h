#ifndef _COMMON_H
#define _COMMON_H

#include <vector>
#include <set>
#include <iostream>
#include <sstream>
#define POINT_NUM 2000

using namespace std;

extern const double DBQ;
extern const double ROU;
extern const double DB_MAX;

extern double g_trail[POINT_NUM][POINT_NUM];
extern double g_prob[POINT_NUM][POINT_NUM];
extern double g_distance_beta[POINT_NUM][POINT_NUM];
extern double g_cost[POINT_NUM][POINT_NUM];
extern int id[POINT_NUM][POINT_NUM];
extern vector<int> out_way[POINT_NUM];
extern vector<int> in_way[POINT_NUM];
extern set<int> demand_node1;
extern set<int> demand_node2;
extern bool g_solved;
extern int ant_num;
extern int iter_num;
extern int max_ver;
extern int edge_num_ant;
extern int start_point;
extern int end_point;
extern int flag;

void init();

#endif
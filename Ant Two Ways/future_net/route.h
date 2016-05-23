#ifndef __ROUTE_H__
#define __ROUTE_H__

#include "lib_io.h"
#include "DPSP.h"
#include "Ant.h"
#include "common.h"

void create_graph(char *topo[MAX_EDGE_NUM], int edge_num, char *demand[MAX_DEMAND_NUM], int demand_num);
void search_route(char *graph[MAX_EDGE_NUM], int edge_num, char *condition[MAX_DEMAND_NUM], int demand_num);
bool search_route_ANT(set<int> &demand, vector<int> &result, vector<int> &point);

#endif

#include "common.h"

const double INF = 99999999;
const double DBQ = 10.0;
const double ROU = 0.2;
const double DB_MAX = 10e9;

double g_trail[POINT_NUM][POINT_NUM];
double g_prob[POINT_NUM][POINT_NUM];
double g_distance_beta[POINT_NUM][POINT_NUM];
double g_cost[POINT_NUM][POINT_NUM];
int id[POINT_NUM][POINT_NUM];
vector<int> out_way[POINT_NUM];
vector<int> in_way[POINT_NUM];
set<int> demand_node1;
set<int> demand_node2;
bool g_solved;
int ant_num;
int iter_num;
int max_ver = 0;
int edge_num_ant = 0;
int start_point;
int end_point;
int flag = 1;

void init()
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